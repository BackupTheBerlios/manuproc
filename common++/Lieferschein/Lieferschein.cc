/* $Id: Lieferschein.cc,v 1.30 2003/07/03 08:20:40 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include"Lieferschein.h"
#include <Misc/Transaction.h>
#include <Auftrag/selFullAufEntry.h>
#include <Artikel/Einheiten.h>
#include <Misc/FetchIStream.h>
//#include <Instanzen/Produziert.h>
#include <unistd.h> 
#include <Misc/TraceNV.h>

#ifdef MABELLA_EXTENSIONS
#include <Lager/FertigWaren.h>
#include <Artikel/ArtikelBase.h>
#endif


Lieferschein::Lieferschein(const LieferscheinBase &lsbase, const ManuProC::Datum &_lsdatum,
int _kdnr,int _rngid, int _paeckchen, int _pakete, const ManuProC::Datum &_geliefertam,
int _dpdlnr)
: LieferscheinBase(lsbase), lsdatum(_lsdatum), kunde(_kdnr), rngid(_rngid)
, geliefertam(_geliefertam)
#ifdef DPD_LIEFERSCHEINE
,dpdliefnr(_dpdlnr),paeckchen(_paeckchen),pakete(_pakete)
#endif
,notiz_valid(false)
,verknr(Kunde::none_id)
{}

static void unbestellteMengeProduzieren(cH_ppsInstanz instanz,
	const ArtikelBase &artikel,
	LieferscheinEntry::mengen_t menge,
	LieferscheinEntry &LE)
{  AufEintragBase neuerAEB;
   if (instanz!=ppsInstanzID::Kundenauftraege) 
      neuerAEB=AufEintrag::unbestellteMengeProduzieren(instanz,artikel,menge,getuid());
   LE.setZusatzInfo(neuerAEB,menge);
}

int Lieferschein::push_back(const ArtikelBase &artikel, int anzahl, 
		mengen_t mengeneinheit, int palette)
{  

   // Eine Instanz, die Lieferscheine schreibt, produziert auch selber (assert in ppsInstanz).
   // Wenn man einn Lieferschein schriebe, ohne daß diese Instanz 
   // selber produzierte, würde doppelt produziert werden.
   assert(Instanz()->Lieferschein());
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("artikel",artikel),
           NV("anzahl",anzahl),NV("mengeneinheit",mengeneinheit),NV("palette",palette));
   Transaction tr;
   int result=LieferscheinBase::none_id;

   AuftragBase::mengen_t menge(anzahl);
   Einheit e(artikel);
   if (!e.hatMenge()) mengeneinheit=0;
   else menge *= mengeneinheit.as_float();

#ifndef MABELLA_EXTENSIONS // auf keinen Fall nach offenen Aufträgen suchen

   SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Artikel_Planung_id
   			(instanz->Id(),getKunde()->Id(),artikel,AuftragBase::handplan_auftrag_id));
   SelectedFullAufList auftraglist(psel);

   if (auftraglist.aufidliste.begin()==auftraglist.aufidliste.end())
     // kann nicht abschreiben
   {  LieferscheinEntry LE=LieferscheinEntry::create(*this, artikel, anzahl,mengeneinheit,palette,false);
      unbestellteMengeProduzieren(Instanz(),artikel,menge,LE);
      result=LE.ZNr();
   }
   else if (menge<=auftraglist.aufidliste.begin()->getRestStk())
     // kann in einem Stueck abschreiben
   {  SelectedFullAufList::iterator i=auftraglist.aufidliste.begin();
      LieferscheinEntry LE=LieferscheinEntry::create(*this, *i,artikel, anzahl,mengeneinheit,palette,false);
      i->Produziert(menge,Id());
      result=LE.ZNr();
   }
   else
   // stueckeln (1*Lieferung, dann Zuordnung)
   {  LieferscheinEntry LE=LieferscheinEntry::create(*this,artikel,anzahl,mengeneinheit,palette,true);

      for (SelectedFullAufList::iterator i=auftraglist.aufidliste.begin();
     	        !!menge && i!=auftraglist.aufidliste.end(); ++i)
      {  AuftragBase::mengen_t abmenge=AuftragBase::min(menge,i->getRestStk());
         if (!abmenge) continue;
           
         LE.setZusatzInfo(*i,abmenge);
         i->Produziert(abmenge,Id());
           
         menge-=abmenge;
         if (!menge) break;
      }
      if(menge>0)
         // da ist noch ein Rest geblieben, setzt ZusatzInfo
         unbestellteMengeProduzieren(Instanz(),artikel,menge,LE);
      result=LE.ZNr();
   }
#else
 if(menge>0)
   {LieferscheinEntry LE=LieferscheinEntry::create(*this, artikel, anzahl,mengeneinheit,palette,false);
    unbestellteMengeProduzieren(Instanz(),artikel,menge,LE);
    if(Instanz() == ppsInstanzID::Kundenauftraege)
      {
       FertigWaren fw(artikel,(FertigWaren::enum_Aktion)'L',
			menge.as_int(),Id());
       if(menge < 0) fw.Einlagern(1);
       else if(menge > 0) fw.Auslagern(1);
      }
     result=LE.ZNr();
   }
#endif

   tr.commit();
   return result;
}

int Lieferschein::push_back(AufEintrag &aufeintrag,
		const ArtikelBase &artikel, int anzahl, 
		mengen_t menge, int palette)
{
   // Eine Instanz, die Lieferscheine schreibt, produziert auch selber (assert in ppsInstanz).
   // Wenn man einn Lieferschein schriebe, ohne daß diese Instanz 
   // selber produzierte, würde doppelt produziert werden.
   assert(Instanz()->Lieferschein());
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("AufEintrag",aufeintrag),
           NV("artikel",artikel),
           NV("Anzahl",anzahl),NV("Menge",menge),NV("Palette",palette));                     
 LieferscheinEntry LE=LieferscheinEntry::create(*this, aufeintrag ,artikel, anzahl,menge,palette);

 mengen_t mng;
 if(!menge) mng = anzahl;
 else mng= anzahl*menge;
 
 aufeintrag.Produziert(mng,Id());
 return LE.ZNr();
}

void Lieferschein::aufraumen() throw(SQLerror)
{
 Query("update lieferschein set rngid=0 where rngid is null "
// 		"and instanz=? "
 		"and not exists(select true from lieferscheinentry "
 		"where lieferscheinentry.lfrsid=lieferschein.lfrsid)") 
//   	<< Instanz()->Id()
	;
 SQLerror::test(__FILELINE__,100);
}


void Lieferschein::closeLfrs()
{
 Query("update lieferschein set rngid=0 where rngid is null "
 		"and (instanz,lfrsid) = (?,?)") 
   	<< Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}


#ifdef DPD_LIEFERSCHEINE

void Lieferschein::setDPDlnr(int d) const throw(SQLerror)
{
 Query("update lieferschein set dpdliefnr=? where (instanz,lfrsid) = (?,?)") 
   	<< Query::NullIf(d,Offen) << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setPakete(const int p) throw(SQLerror)
{  
 Query("update lieferschein set pakete=? where (instanz,lfrsid) = (?,?)") 
   	<< p << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setPaeckchen(const int p) throw(SQLerror)
{  
 Query("update lieferschein set paeckchen=? where (instanz,lfrsid) = (?,?)") 
   	<< p << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setGewichtNetto(const fixedpoint<1> i) throw(SQLerror)
{  
 Query("update lieferschein set netto_kg=? where (instanz,lfrsid) = (?,?)") 
   	<< i << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setGewichtBrutto(const fixedpoint<1> i) throw(SQLerror)
{  
 Query("update lieferschein set brutto_kg=? where (instanz,lfrsid) = (?,?)")
   << i << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}


void Lieferschein::setDPDDatum() const throw(SQLerror)
{
 Query("update lieferschein set geliefertam=now() "
 	"where (instanz,lfrsid)=(?,?)") << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}
#endif

int Lieferschein::maxZnr() throw(SQLerror)
{
 return (Query("select max(zeile) from lieferscheinentry where "
 	"(instanz,lfrsid) = (?,?)") 
 	<< Instanz()->Id() << Id()
 	).FetchOneMap<int>();
}

const Preis::rabatt_t Lieferschein::AufRabatt() const throw(SQLerror)
{
 return (Query("select max(rabatt) from "
 	"auftrag a join lieferscheinentry e "
   	"on (refauftragid=auftragid and a.instanz=e.instanz) where "
   	"(e.instanz,e.lfrsid)=(?,?)") << Instanz()->Id() << Id()
   	).FetchOneMap<int>();;
}

LieferscheinBase::mengen_t Lieferschein::StandardLaenge(const ArtikelBase artikel) throw (SQLerror)
{
 return (Query("select menge from lieferscheinentry e join lieferschein l "
   "on (lfrsid,instanz) where artikelid=? " // "and l.instanz=? "
   "order by datum desc limit 1") << artikel.Id() /* << Instanz()->Id() */
   ).FetchOneMap<LieferscheinBase::mengen_t>();
}

#ifdef DPD_LIEFERSCHEINE
const ManuProC::Datum Lieferschein::getMaxZahlziel() const throw(SQLerror)
{
 return Query("SELECT max(a.zahlungsziel) from "
 	"auftrag a join lieferscheinentry e "
   	"on (refauftragid=auftragid and a.instanz=e.instanz) "
   	"where (e.instanz,e.lfrsid)=(?,?)")
   	<< Instanz()->Id() << Id()
   	).FetchOneMap<ManuProC::Datum>();
}
#endif

const std::string Lieferschein::Notiz() const throw(SQLerror)
{
// ManuProC::Trace _t(ManuProC::Tracer::Lieferschein, __FUNCTION__,str());
 if(notiz_valid) return notiz;

 Query("select notiz from lieferschein where (instanz,lfrsid)=(?,?)")
 	<< Instanz()->Id() << Id()
 	>> FetchIStream::MapNull(notiz);
 notiz_valid=true;
 return notiz;
}

void Lieferschein::Notiz(const std::string n) throw(SQLerror)
{
// ManuProC::Trace _t(ManuProC::Tracer::Lieferschein, __FUNCTION__,str());

 Query("update lieferschein set notiz=? where (instanz,lfrsid)=(?,?)")
 	<< n 
 	<< Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);

 notiz=n;
}

#ifdef MABELLA_EXTENSIONS
void Lieferschein::setVerknr(const Kunde::ID vknr) throw(SQLerror)
{
 Query("update lieferschein set verknr=? where"
	" (instanz,lfrsid)=(?,?)")
	<< Query::NullIf(vknr,Kunde::none_id)
	<< Instanz()->Id()
	<< Id();
 SQLerror::test(__FILELINE__);

 verknr=vknr;
}
#endif

void Lieferschein::setDatum(const ManuProC::Datum &d) throw(SQLerror)
{  
 Query("update lieferschein set geliefertam=? where 
 	(instanz,lfrsid) = (?,?)") 
 	<< d << Instanz()->Id() << Id() ;
 SQLerror::test(__FILELINE__);
}
