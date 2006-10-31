// $Id: Auftrag.cc,v 1.27 2006/10/31 16:03:06 christof Exp $
/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include"AuftragFull.h"
#include<Misc/Ausgabe_neu.h>
#include <Misc/TraceNV.h>
#include <unistd.h>
#include<Misc/Changejournal.h>
#include <Misc/Transaction.h>

std::string Auftrag::getAuftragidToStr() const
{ return ID2string(auftragid); }


void Auftrag::setStatusAuftrag_(AufStatVal st) throw(SQLerror)
{
 setStatusAuftragBase(st);
 status=st;
}

void Auftrag::setRabatt(const rabatt_t _auftragsrabatt) throw(SQLerror)
{
  AuftragBase::setRabatt(_auftragsrabatt);
  auftragsrabatt=_auftragsrabatt;
}

Auftrag::ID Auftrag::Copy(AuftragCopy &ac) throw(SQLerror)
{
// Auftrag a(Anlegen(Instanz()), ac.kundennr, ac.yanr);
  return none_id;
}

Auftrag::youraufnr_cache_t Auftrag::youraufnr_cache;

std::string Auftrag::getYourAufNr(const AuftragBase &ab)
{  youraufnr_cache_t::const_iterator i=youraufnr_cache.find(ab);
   if (i!=youraufnr_cache.end()) return i->second;
   try
   {  std::string yan;
      Query("select youraufnr from auftrag where (instanz,auftragid)=(?,?)")
   	<< ab
   	>> Query::Row::MapNull(yan);
      youraufnr_cache[ab]=yan;
      return yan;
   }
   catch (SQLerror &e)
   {  // erzeugt implizit einen Leerstring
      return youraufnr_cache[ab];
   }
}

void Auftrag::insert(unsigned zeilennr, const mengen_t bestellt, 
  const ManuProC::Datum lieferdatum,const ArtikelBase& artikel,
  const AufStatVal status,const bool setInstanzAuftraege,
  const Preis& preis,const fixedpoint<2> rabatt,
  const cH_PreisListe &preisliste) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,
   NV("Menge",bestellt),NV("Datum",lieferdatum),
   NV("Artikel",artikel),
   NV("Status",status),NV("InstanzAuftr�eSetzen",setInstanzAuftraege));
 assert(Instanz()!=ppsInstanzID::Kundenauftraege || setInstanzAuftraege==true);
  // wieso auch nicht ... CP

 if(status!=getStatus() && status==OPEN) 
    setStatusAuftragBase(AufStatVal(status));

 Query("insert into auftragentry (instanz,auftragid, zeilennr,"
	"bestellt,geliefert,lieferdate,artikelid,status,"
	"preis,rabatt,preismenge,preisliste,lastedit_uid)"
	"values (?,?,?, "
	"?,?,?,?,?,"
	"?,?,?,?,?)")
	<< *this << zeilennr
	<< bestellt << 0 << lieferdatum << artikel << status
	<< preis.Wert() << rabatt << preis.PreisMenge() 
		<< Query::NullIf(preisliste->Id(),PreisListe::none_id)
		<< getuid();

 if(Instanz()==ppsInstanzID::Kundenauftraege)
  { pps_ChJournalEntry::newChange(AufEintragBase(*this,zeilennr),
                        artikel,bestellt,bestellt);
  }

#ifdef MABELLA_EXTENSIONS
    cH_Kunde kd(kundennr);
    fixedpoint<2> provsatz;
    provsatz = kd->getProvSatz_Artikel(artikel,rabatt);
    Query("update auftragentry set provsatz=?"
      	" where (instanz,auftragid,zeilennr)=(?,?,?)")
	<< provsatz
      	<< *this << zeilennr;
      	SQLerror::test(__FILELINE__);
#endif
 // Kundenauftr�e legen automatisch einen Auftrag bei der entsprechenden Instanz 
 // fr denselben Artikel an.
 if(status==OPEN && setInstanzAuftraege)
  // Nur offene Auftr�e werden nach unten bestellt
   {  AufEintrag AE(make_value(AufEintragBase(*this,zeilennr)));
      AE.Verzeigern(AE.getRestStk());
   }
}

AufEintragBase Auftrag::push_back(const mengen_t bestellt, 
  const ManuProC::Datum lieferdatum,const ArtikelBase& artikel,
  const AufStatVal status,const bool setInstanzAuftraege,
  const Preis& preis,const fixedpoint<2> rabatt,
  const cH_PreisListe &preisliste,
  const ArtikelBase& component_from) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,
   NV("Menge",bestellt),NV("Datum",lieferdatum),
   NV("Artikel",artikel),
   NV("Status",status),NV("InstanzAuftr�eSetzen",setInstanzAuftraege));
 assert(Instanz()!=ppsInstanzID::Kundenauftraege || setInstanzAuftraege==true);
  // wieso auch nicht ... CP
 int ZEILENNR;

 Transaction tr;
 Query("lock table auftragentry in exclusive mode");

//ECPGdebug(true,stdout);
 Query("select max(zeilennr)+1 from auftragentry "
	"where (instanz,auftragid)= (?,?)")
	<< *this
	>> Query::Row::MapNull(ZEILENNR,1);
 insert(ZEILENNR,bestellt,lieferdatum,artikel,status,setInstanzAuftraege,
  	preis,rabatt,preisliste);
  	
 if(component_from.valid())
   {
    ExtraColumns ec("auftragentry","instanz","auftragid","zeilennr");
              ec << instanz->Id() << Id() << ZEILENNR;
    if(ec.hasColumn("component_from"))
      Query("update auftragentry set component_from = ? where"
            " (instanz,auftragid,zeilennr)= (?,?,?)")
       << component_from.Id() << instanz->Id() << Id() << ZEILENNR;	
   } 
  	
 tr.commit();
 return AufEintragBase(*this,ZEILENNR);
}


void Auftrag::Label(unsigned int lid) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);

 ExtraColumns ec("auftrag","instanz","auftragid");
 ec << *this;
 
 if(!ec.hasColumn("label")) return;
 
 Transaction tr;
 
 Query("update auftrag set label=? where (instanz,auftragid)=(?,?)")
   << lid << *this;
 SQLerror::test(__FILELINE__);

 tr.commit();

 labelid=lid;
}

void Auftrag::Notiz(const std::string &n) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
 Query("update auftrag set notiz=? where (instanz,auftragid)=(?,?)")
   << n
   << *this;
 notiz=n;
}

const std::string Auftrag::Notiz() const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
 if(notiz_valid) return notiz;

 Query("select notiz from auftrag where (instanz,auftragid)=(?,?)")
	<< *this
	>> notiz;
 notiz_valid=true;
 return notiz;
}

void Auftrag::setYourAufNr(const std::string &yanr) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);

 Query q("update auftrag set youraufnr=trim(?) where (instanz,auftragid)=(?,?)");

 q << Query::NullIf(yanr,"") << *this;
 SQLerror::test(__FILELINE__);

 std::string new_yaid;
 Query("select youraufnr from auftrag where (instanz,auftragid)=(?,?)")
	<< *this >> Query::Row::MapNull(new_yaid);

 youraufnr_cache[*this]=new_yaid;

/*
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
 exec sql begin declare section;
 int AUFTRAGID=Id();
 int INSTANZ=int(Instanz());
 char YOURAUFNR[21];
 exec sql end declare section;

 youraufnr=yanr;
 youraufnr_cache[*this]=youraufnr;

 strncpy0(YOURAUFNR,youraufnr.c_str(),20);

 exec sql update auftrag set youraufnr= nullif(:YOURAUFNR,'')
	where auftragid= :AUFTRAGID and instanz = :INSTANZ;
 SQLerror::test("setYourAufNr: update youraufnr");
*/
}

Auftrag::ID Auftrag::getIdFromYourAufId(ppsInstanz::ID instanz, 
			const std::string &youraufid, 
			Kunde::ID kundennr, bool fuzzy) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,instanz,youraufid,kundennr);
 if (kundennr==Kunde::none_id)
   return (Query("select auftragid from auftrag where (instanz,youraufnr)=(?,?)")
	<< instanz << youraufid)
	.FetchOne<int>();
  Query q("select auftragid from auftrag where (instanz,kundennr,youraufnr)=(?,?,?)");
  q << instanz << kundennr << youraufid;
  if (!fuzzy || q.LinesAffected()) return q.FetchOne<int>();

  try
  { return (Query("select auftragid from auftrag where (instanz,kundennr)=(?,?) "
  		"and youraufnr like ?")
        << instanz << kundennr << (youraufid+"%")).FetchOne<int>();
  }
  catch (...) {}
  // nur offenen Aufträge nehmen wenn dadurch eindeutig
  return (Query("select auftragid from auftrag where (instanz,kundennr)=(?,?) "
                "and exists(select true from auftragentry "
                " where (auftragentry.instanz,auftragentry.auftragid,auftragentry.status)="
                 "(auftrag.instanz,auftrag.auftragid,?)) "
  		"and youraufnr like ?")
        << instanz << kundennr << OPEN
        << (youraufid+"%")).FetchOne<int>();
}

Auftrag::ID Auftrag::getIdFromYourAufId(const char *youraufid) throw(SQLerror)
{ return getIdFromYourAufId(Instanz(),youraufid);
}



const ManuProC::Datum Auftrag::sent_at() const throw(SQLerror)
{
 ManuProC::Datum sent;
 Query("select sent_at from auftrag where (instanz,auftragid)=(?,?)")
	<< *this >> sent;
 return sent;
}

const std::string Auftrag::sent_to() const throw(SQLerror)
{
 std::string sent;
 Query("select sent_to from auftrag where (instanz,auftragid)=(?,?)") 
	<< *this >> sent;
 return sent;
}

void Auftrag::Set_sent_at() const throw(SQLerror)
{
 Query("update auftrag set sent_at=now() where (instanz,auftragid)=(?,?)") 
	<< *this;
}


void Auftrag::Set_sent_to(const std::string t) const throw(SQLerror)
{
 Query("update auftrag set sent_to=? where (instanz,auftragid)=(?,?)") 
	<< t << *this;
}


