// $Id: AuftragBase.cc,v 1.30 2003/03/13 08:19:54 christof Exp $
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
   {case (AufStatVal)UNCOMMITED : return "unbest�tigt"; 
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

// k�nnte eigentlich ersetzt werden IMHO
// wird aufgerufen von rein_ins_lager (+), ArtikelInternAbbestellen (-),
// 
void AuftragBase::dispo_auftrag_aendern(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,const mengen_t &menge,
         const ManuProC::Datum &datum,const AufEintragBase &kindAEB)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
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

int AuftragBase::PassendeZeile(const ManuProC::Datum lieferdatum,const ArtikelBase& artikel,
  AufStatVal status,unsigned uid) const throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,NV("Artikel",artikel),
         NV("LieferDatum",lieferdatum),NV("Status",status));
 // spezieller Fall f�r unbestellte Lieferungen
 if (status==CLOSED && Id()==plan_auftrag_id)
    create_if_not_exists(OPEN);
 else
 {  assert(status==OPEN || Instanz()->Id()==ppsInstanzID::Kundenauftraege);
    // Auftrag anlegen, wenn noch nicht da
    create_if_not_exists(status);
 }
 
 int znr=ManuProcEntity<>::none_id,neuznr=ManuProcEntity<>::none_id;
 mengen_t dummy=0;
 if(!existEntry(artikel,lieferdatum,znr,neuznr,dummy,status))
  { Auftrag A(*this);
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

 // dispo=> andere Richtung des Pfeils muss ge�ndert werden 
 if (!kein_dispo && altAEB.valid())
      AufEintragZu(NeuAEB).Neu(altAEB,deltamenge);

  tr.commit();
  return NeuAEB.ZNr();
}
