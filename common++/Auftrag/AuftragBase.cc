// $Id: AuftragBase.cc,v 1.37 2003/06/19 12:03:58 jacek Exp $
/*  pps: ManuProC's production planning system
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

#include <Auftrag/AuftragBase.h>
#include <Auftrag/Auftrag.h>
#include<stdio.h>
#include <Misc/Transaction.h>
#include<Misc/Changejournal.h>
#include <Auftrag/auftrag_status.h>
#include "AufEintrag.h"
#include <Artikel/ArtikelStamm.h>
#include <Auftrag/VerfuegbareMenge.h>
#include <Auftrag/AufEintragZu.h>
#include <Misc/TraceNV.h>
#include <Lager/Lager.h>

std::string AuftragBase::str() const
{
  return instanz->Name()+"|"+itos(Id());
}

std::ostream &operator<<(std::ostream &o,const AuftragBase &a)
{
  return  o<<a.Instanz()->Name()<< "(" << a.Id() <<")";
}

const std::string AuftragBase::getStatusStr(AufStatVal a)
{
 switch(a)
   {case (AufStatVal)UNCOMMITED : return "unbestätigt"; 
    case (AufStatVal)OPEN : return "offen"; 
    case (AufStatVal)CLOSED : return "fertig";
    case (AufStatVal)STORNO : return "storno";
    case (AufStatVal)NOSTAT : ;
   }
 return "-";
}

const AuftragBase::ID AuftragBase::PlanId_for(const cH_ppsInstanz &instanz)
{
  if(instanz->LagerInstanz()) return AuftragBase::plan_auftrag_id;
  return AuftragBase::handplan_auftrag_id;
}

AuftragBase::mengen_t AuftragBase::min(const mengen_t &x,const mengen_t &y)
{ mengen_t m; if(x<=y) m=x; else m=y; if(m<0) m=0; return m;}
//{ if(x<=y) return x; return y; }
AuftragBase::mengen_t AuftragBase::max(const mengen_t &x,const mengen_t &y)
{ if(x>=y) return x; return y; }

bool AuftragBase::editierbar() const
{
  if     (Id() == dispo_auftrag_id) return false;
  else if(Id() == plan_auftrag_id)  return false;
  else if(Id() == ungeplante_id)    return false;
  else if(Id() == none_id)          return false;
  return true;
}

// könnte eigentlich ersetzt werden IMHO
// wird aufgerufen von rein_ins_lager (+), ArtikelInternAbbestellen (-),
// 
void AuftragBase::dispo_auftrag_aendern(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,const mengen_t &menge,
         const ManuProC::Datum &datum,const AufEintragBase &kindAEB)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,
     NV("Artikel",artikel),NV("Menge",menge));
   AuftragBase(instanz,AuftragBase::dispo_auftrag_id)
   	.BestellmengeAendern(menge,datum,artikel,OPEN,uid,kindAEB);
}

#if __GNUC__ > 2  // fragt nicht ...
const AuftragBase::ID AuftragBase::handplan_auftrag_id,
	AuftragBase::dispo_auftrag_id,
	AuftragBase::plan_auftrag_id,
	AuftragBase::invalid_id,
	AuftragBase::none_id,
	AuftragBase::ungeplante_id;
#endif

const UniqueValue::value_t AuftragBase::trace_channel=ManuProC::Tracer::channels.get();
bool AuftragBase::tolerate_inconsistency;

int AuftragBase::PassendeZeile(const ManuProC::Datum lieferdatum,const ArtikelBase& artikel,
  AufStatVal status,unsigned uid) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Artikel",artikel),
         NV("LieferDatum",lieferdatum),NV("Status",status));
 mengen_t dummy=0;
 int znr=existEntry(artikel,lieferdatum,status,dummy);
 if(znr==none_id)
  { // spezieller Fall für unbestellte Lieferungen
    if (status==CLOSED && Id()==plan_auftrag_id)
       create_if_not_exists(OPEN);
    else
    {  assert(status==OPEN || Instanz()->Id()==ppsInstanzID::Kundenauftraege);
       // Auftrag anlegen, wenn noch nicht da
       create_if_not_exists(status);
    }
    Auftrag A(*this);
    AufEintragBase newaeb=A.push_back(0,lieferdatum,artikel,status,uid,false);
    znr=newaeb.ZNr();
  }
 return znr;
}

// wird von ArtikelInternNachbestellen, dispo_auftrag_aendern verwendet
int AuftragBase::BestellmengeAendern(mengen_t deltamenge, 
  const ManuProC::Datum lieferdatum,const ArtikelBase& artikel,
  AufStatVal status,int uid,const AufEintragBase& altAEB,
  st_BestellmengeAendern st_bool) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Artikel",artikel),
         NV("Menge",deltamenge),NV("AltAEB",altAEB),
         NV("LieferDatum",lieferdatum),
         NV("Status",status));
 Transaction tr;
 Query::Execute("lock table auftragentry in exclusive mode");
 SQLerror::test(__FILELINE__);  

 bool kein_dispo= Id()!=dispo_auftrag_id;
 AufEintragBase NeuAEB(*this,PassendeZeile(lieferdatum,artikel,status,uid));

 // ist die Reihenfolge hier wichtig?
 // rekursion nur falls kein DispoAuftrag
 AufEintrag(NeuAEB).MengeAendern(uid,deltamenge,kein_dispo,
 	kein_dispo?altAEB:AufEintragBase(),ManuProC::Auftrag::r_Anlegen);

 // dispo=> andere Richtung des Pfeils muss geändert werden 
 if (!kein_dispo && altAEB.valid())
      AufEintragZu(NeuAEB).Neu(altAEB,deltamenge);

  tr.commit();
  return NeuAEB.ZNr();
}

// aufid sollte string sein (es ist immerhin youraufnr gemeint)
AuftragBase::AuftragBase(cH_ppsInstanz _instanz, ID aufid,Kunde::ID kid) throw(SQLerror)
: instanz(_instanz),auftragid(none_id)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
 Query("select auftragid,verknr from auftrag "
 	"where instanz=? and kundennr=? and youraufnr=?")
 	<< instanz->Id() << kid << aufid
 	>> auftragid >> verknr;
}

ArgumentList &operator<<(ArgumentList &q, const AuftragBase &ab)
{  return q << ab.InstanzID() << ab.Id();
}

bool AuftragBase::Exists() const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
 return (Query("select exists(select true from auftrag "
 		"where (instanz,auftragid)=(?,?))").lvalue()
 	<< *this)
 	.FetchOne<bool>();
}

void AuftragBase::create_if_not_exists(AufStatVal status,Kunde::ID kunde) const
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Status",status),NV("Kunde",kunde));
  if(Exists()) return;
  Query("insert into auftrag (instanz,auftragid,kundennr,stat) "
  	"values (?,?,?,?)").lvalue()
  	<< *this << kunde << status;
 SQLerror::test(__FILELINE__);
}


void AuftragBase::setStatusAuftragBase(AufStatVal st) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Status",st));
 Query("update auftrag set stat=? where (instanz,auftragid)=(?,?)").lvalue()
 	<< st << *this;
 SQLerror::test(__FILELINE__"Status setzen");
}


void AuftragBase::setRabatt(const rabatt_t auftragsrabatt) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this);
 Query("update auftrag set rabatt=? where (instanz,auftragid)=(?,?)").lvalue()
 	<< auftragsrabatt << *this;
 SQLerror::test(__FILELINE__"Rabatt setzen");
}

int AuftragBase::existEntry(const ArtikelBase& artikel, const ManuProC::Datum& lieferdatum,
                             AufStatVal status, mengen_t& menge_out) const throw()
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,
         NV("Artikel",artikel),NV("LieferDatum",lieferdatum),NV("Status",status));
   int ZEILENNR;
 try
 {  Query("select zeilennr,bestellt from auftragentry "
	"where (instanz,auftragid)= (?,?) and "
        "lieferdate=? and artikelid=? and status=? limit 1").lvalue()
        << *this << lieferdatum << artikel.Id() << status
        >> ZEILENNR >> menge_out;
     return ZEILENNR;
 } catch (SQLerror &e)
 {  return none_id;
 }
}

