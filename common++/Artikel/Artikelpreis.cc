// $Id: Artikelpreis_sql.pgcc,v 1.19 2003/06/05 12:48:15 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include "Artikel/Artikelpreis.h"
#include <Artikel/ArtikelBezeichnung.h>
#include <Misc/CacheStatic.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/Prozesspreis.h>
#include <Misc/Transaction.h>
#include <Misc/TraceNV.h>

#include <unistd.h>
#include <sys/types.h>
#include <algo.h>
#include <functional>
#include <Misc/Event.h>

#ifndef SIGC1_2
#include <sigc++/func_slot.h>
#endif

namespace // index_t and payload_t should not be globally visible
{

struct index_t
{  Kunde::ID liste;
   ArtikelBase::ID art;
   index_t(Kunde::ID l, ArtikelBase::ID a) : liste(l), art(a) {} 
   bool operator==(const index_t &b) const 
   { return liste==b.liste && art==b.art; }
   bool operator<(const index_t &b) const 
   { return liste<b.liste || (liste==b.liste && art<b.art); }
};


class Preis_lseq : public unary_function<std::pair<int,Preis>,bool>
{
 int bestellmenge;
public:
 explicit Preis_lseq (const int m) : bestellmenge(m) {}
 bool operator() (const std::pair<int,Preis> &p) const 
	{ return p.first <= bestellmenge; }
};


struct payload_t
{  
 typedef std::map<int,Preis> Pmap;

   Pmap preis;
   bool errechnet;
   bool gefunden;
   payload_t(bool gef, bool e=true)
   	: errechnet(e), gefunden(gef) {}
   std::pair<int,Preis> get_preis(int bestellmenge=1)
	{ Pmap::reverse_iterator p=find_if(preis.rbegin(),preis.rend(),
			Preis_lseq(bestellmenge));
	  if(p!=preis.rend()) return (*p);
	  else return std::pair<int,Preis>(1,Preis());
	}
   payload_t() : errechnet(false),gefunden(false) {}
};

typedef CacheStatic<index_t,payload_t> cache_t;

static cache_t cache;

static void PreisGeaendert_cb(const std::string &key, const std::string &value)
{  std::string::size_type komma=key.find(',');
   if (komma==std::string::npos) return;
   int liste=atoi(key.substr(0,komma).c_str());
   int artikel=atoi(key.substr(komma+1).c_str());
   Artikelpreis::UnCache(PreisListe::ID(liste),ArtikelBase(artikel));
}

static void callback_registrieren()
{  ManuProC::Event::connect("artikelpreis",SigC::slot(&PreisGeaendert_cb));
}

namespace {
struct dummy
{	dummy() { callback_registrieren(); }
};
}

static dummy dummy2;

Artikelpreis::Artikelpreis(const PreisListe::ID liste,const ArtikelBase &a,
				int bestellmenge)
	: errechnet(false), gefunden(false),
	  gefunden_in(ManuProcEntity<>::none_id),mindestmenge(1)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__, 
	NV("Liste",liste), NV("Artikel",a), NV("Menge",bestellmenge));
   payload_t *cached=cache.lookup(index_t(liste,a.Id()));
   if (cached) { std::pair<int,Preis> p=cached->get_preis(bestellmenge);
		 setPreis(a.Id(),p.second,liste,
				cached->errechnet);
		 mindestmenge=p.first;
	 	 gefunden=cached->gefunden;
 		 ManuProC::Trace(trace_channel,__FILELINE__,NV("cache",static_cast<const Preis&>(*this)));
		}
   else
   {
      Query q("select preis, mindestmenge, preismenge, waehrung"
	      	" from artikelpreise where (artikelid,kundennr)=(?,?)");
	// kein order by ??? CP	      	
      q << a.Id() << liste;
      payload_t pyl(!Query::Code(), false);
      FetchIStream is;
      if (!Query::Code())
      {  while ((q>>is).good())
         { geldbetrag_t PREIS;
      	   preismenge_t PREISMENGE;
           int WAEHRUNG;
           int MINDESTMENGE;
           is >> PREIS >> FetchIStream::MapNull(MINDESTMENGE,1) 
           	>> FetchIStream::MapNull(PREISMENGE,1) >> WAEHRUNG;
           is.ThrowIfNotEmpty(__FUNCTION__);
	  pyl.preis[MINDESTMENGE]=
		Preis(PREIS,Waehrung::ID(WAEHRUNG),PREISMENGE);
	 }

	  std::pair<int,Preis> p=pyl.get_preis(bestellmenge);
	  setPreis(a.Id(),p.second,liste,false);
	  mindestmenge=p.first;
	  ManuProC::Trace(trace_channel,__FILELINE__,NV("db",static_cast<const Preis&>(*this)));
      }
         
      cache.Register(index_t(liste,a.Id()), pyl);
   }
}

void Artikelpreis::UnCache(const PreisListe::ID liste,const ArtikelBase &ab)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__, NV("liste",liste), NV("artikel",ab));
   cache.deregister(index_t(liste,ab.Id()));
};

Artikelpreis::Artikelpreis(const cH_Kunde &k,const ArtikelBase &a, 
				int bestellmenge)
	: errechnet(false), gefunden(false), gefunden_in(ManuProcEntity<>::none_id)
{  Artikelpreis ap;

   for (std::list<std::pair<int,PreisListe::ID> >::const_iterator i=k->Preislisten().begin();
   	i!=k->Preislisten().end();++i)
   {  ap=Artikelpreis(i->second,a,bestellmenge);
      if (ap.Gefunden()) { *this=ap; return; }
   }

   // oder für alle?   	
#ifdef PETIG_EXTENSIONS // errechnen
         Preis p;
         ArtikelBaum AB(a);
         for (ArtikelBaum::const_iterator i=AB.begin(); i!=AB.end(); ++i)
         {  p+= Prozesspreis(k->preisliste(), (*i).erzeugung->Id()) 
         	+ (*i).menge * Artikelpreis(k, (*i).rohartikel);
         }
         setPreis(a.Id(),p,ManuProcEntity<>::none_id,true);
#endif         
}


const Artikelpreis Artikelpreis::create(const PreisListe::ID liste,
		const Preis &p, const ArtikelBase &a,
		int mindmenge,
		std::vector<std::string> ins_all_komp,
		bool newstaffel) throw(SQLerror)
{
 int UID;
 int MINDESTMENGE=mindmenge;

 std::vector<int> to_insert; // Artikel, alter Preis


 if(ins_all_komp.size())
   {cH_ArtikelBezeichnung ab(a);
    cH_ExtBezSchema ebz=ab->getExtBezSchema();
    std::string query;
    std::string artbez_tabelle;
    artbez_tabelle=" artbez_"+itos(ebz->Typ().Id())+"_"+itos(ebz->Id());

    query="select id from "+artbez_tabelle+" where ";
    for(std::vector<std::string>::const_iterator s=ins_all_komp.begin();
	s!=ins_all_komp.end(); ++s)
	{
	 ExtBezSchema::BezKomp bk=(*ebz)[*s];
	   query+=bk.spaltenname+"='"+
	   ab->Komponente(bk.folgenr_in_sig,bk.signifikanz)+"' and ";
	}


    query+=" not exists (select true from artikelpreise p "
	"where "+artbez_tabelle+".id=p.artikelid and p.kundennr=?"
	" and p.mindestmenge=?)";

    if(newstaffel) // no new articles; only new preisstaffel for existsing
      query+=" and exists (select true from artikelpreise p "
	"where "+artbez_tabelle+".id=p.artikelid and p.kundennr=?)";

    Query qu(query); 
    qu << liste << mindmenge;

    if(newstaffel) qu << liste;

    qu.FetchArray(to_insert);
    
   }
 else
   {
   to_insert.push_back(a.Id());	
   }

 Transaction tr;
 
 UID=int(getuid());

 for(std::vector<int>::const_iterator i=to_insert.begin();
	i!=to_insert.end(); ++i)
   {
    Query("insert into preis_change_journal (artikelid,prlsnr,zeitpunkt,"
		"preis_alt,preis_neu,uid,mindestmenge) values "
		"(?,?,now(),null,?,?,?)")
	<< (*i) << liste << p.Wert() << UID << MINDESTMENGE;
    SQLerror::test(__FILELINE__);	

    Query("insert into artikelpreise "
 	"(artikelid,kundennr,preis,preismenge,waehrung,mindestmenge) values "
 	"(?,?,?,?,?,?)")
 	<< (*i) << liste << p.Wert() << p.PreisMenge() << p.getWaehrung()->Id() << MINDESTMENGE;
    SQLerror::test(__FILELINE__);	
 }

 tr.commit();
  
 return Artikelpreis(liste,a,mindmenge);
}


void Artikelpreis::changePreis(const Preis &p, int newmindmenge) throw(SQLerror)
{
 int ARTIKELID=artikel;
 geldbetrag_t PREIS_ALT;
 int UID;
 int MINDESTMENGE=mindestmenge;
 int NEWMINDMENGE=newmindmenge==0 ? mindestmenge : newmindmenge; 


 if(gefunden_in == PreisListe::none_id) return;

 UID=int(getuid());

 Transaction tr;

 Query("select preis from artikelpreise where "
	"artikelid=? and kundennr=? and mindestmenge=?")
	<< ARTIKELID << gefunden_in << MINDESTMENGE
	>> PREIS_ALT;

 Query("insert into preis_change_journal "
	"(artikelid,prlsnr,zeitpunkt,preis_alt,preis_neu,uid,mindestmenge) "
	"(select artikelid, ?, now(), preis, ?, ?, ? "
	"from artikelpreise where kundennr=? and "
	"mindestmenge=? and "
	"artikelid in "
	"(select a.id from artbez_3_1 a join artbez_3_1 b "
	"on ( a.artikel=b.artikel and "
		"a.breite=b.breite and "
		"a.aufmachung=b.aufmachung "
		"and b.id=?)"
	"))")
	<< gefunden_in << p.Wert() << UID << NEWMINDMENGE
	<< gefunden_in << MINDESTMENGE 
	<< ARTIKELID;
 SQLerror::test(__FILELINE__);	
 
 Query("update artikelpreise set "
 	"preis=?,preismenge=?,waehrung=?,mindestmenge=? "
	"where kundennr=? and "
	"mindestmenge=? and artikelid in "
	"(select a.id from artbez_3_1 a join artbez_3_1 b "
	"on (	a.artikel=b.artikel and "
		"a.breite=b.breite and "
		"a.aufmachung=b.aufmachung and "
		"b.id=?)"
	")")
	<< p.Wert() << p.PreisMenge() << p.getWaehrung()->Id() 
	<< NEWMINDMENGE
	<< gefunden_in << MINDESTMENGE 
	<< ARTIKELID;
 SQLerror::test(__FILELINE__);	
 
 mindestmenge=NEWMINDMENGE;

 tr.commit();
 getPreis()=p;
}



void Artikelpreis::remove(const PreisListe::ID liste,const ArtikelBase &a,
	std::vector<std::string> del_all_komp,
	int mindmenge, bool from_all_lists) throw(SQLerror)
{
 int ARTIKELID=a.Id();
 geldbetrag_t PREIS;
 int UID=getuid();
 int MINDESTMENGE=mindmenge;
 int PRLSNR=liste;

// typedef std::pair<ArtikelBase::ID,int> UniqPreis; // artikel, mindestmenge

 std::vector<std::pair<UniqPreis,geldbetrag_t> > to_delete; // Artikel, alter Preis
 std::string query;
 std::string artbez_tabelle;

 if(!del_all_komp.empty())
   {cH_ArtikelBezeichnung ab(a);
    cH_ExtBezSchema ebz=ab->getExtBezSchema();
    artbez_tabelle=" artbez_"+itos(ebz->Typ().Id())+"_"+itos(ebz->Id());
    query="select id,mindestmenge,p.kundennr,preis from "+artbez_tabelle+" a join artikelpreise p "
	" on (id=artikelid ";

   if(!from_all_lists)
	query+=" and p.kundennr="+itos(liste);

    if(MINDESTMENGE)
	query+=" and p.mindestmenge="+itos(MINDESTMENGE);

    query+=") where ";
    for(std::vector<std::string>::const_iterator s=del_all_komp.begin();
	s!=del_all_komp.end(); ++s)
	{
	 ExtBezSchema::BezKomp bk=(*ebz)[*s];
	   query+=bk.spaltenname+"='"+
	   ab->Komponente(bk.folgenr_in_sig,bk.signifikanz)+"' and ";
	}
    query+=" true "; // damit am Ende kein einsames "and" stehen bleibt

    Query(query).FetchArray(to_delete);
    SQLerror::test(__FILELINE__);
   }
 else
   {
   std::string q="select id,mindestmenge,kundennr,preis from artikelpreise where "
	"artikelid=? ";
   if(!from_all_lists)
	q+=" and kundennr="+itos(PRLSNR);

   if(MINDESTMENGE)
   	q+=" and mindestmenge="+itos(MINDESTMENGE);

   Query qu(q);
   qu << ARTIKELID;

   qu.FetchArray(to_delete);

   SQLerror::test(__FILELINE__);

//   to_delete.push_back(std::pair<int,geldbetrag_t>(ARTIKELID,PREIS));	
   }



 Transaction tr;

 for(std::vector<std::pair<UniqPreis,geldbetrag_t> >::const_iterator i=to_delete.begin();
	i!=to_delete.end(); ++i)
   {
    ARTIKELID=(*i).first.artid;
    PREIS=(*i).second;
    MINDESTMENGE=(*i).first.mindmenge;
    PRLSNR=(*i).first.plid;

    Query("insert into preis_change_journal (artikelid,prlsnr,zeitpunkt,"
   		"preis_alt,preis_neu,uid,mindestmenge) values "
   		"(?,?,now(),?,0.0,?,?)")
   		<< ARTIKELID << PRLSNR << PREIS << UID << MINDESTMENGE;
    SQLerror::test(__FILELINE__);	
   	
    Query("delete from artikelpreise "
   	"where (kundennr,artikelid,mindestmenge)=(?,?,?)")
	<< PRLSNR << ARTIKELID << MINDESTMENGE;
    SQLerror::test(__FILELINE__);	
  }

 tr.commit();
 
}

};

FetchIStream &operator>>(FetchIStream &is,
	std::pair<Artikelpreis::UniqPreis,Preis::geldbetrag_t> &ag)
{  return is >> ag.first.artid >> ag.first.mindmenge
	>> ag.first.plid >> ag.second;
}

const UniqueValue::value_t Artikelpreis::trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channel_e("DEBUG_PREIS",Artikelpreis::trace_channel);

