// $Id: AuftragBase.cc,v 1.18 2002/12/19 16:22:20 thoma Exp $
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
#include <Aux/Transaction.h>
#include<Aux/Changejournal.h>
#include <Auftrag/auftrag_status.h>
#include "AufEintrag.h"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelImLager.h>
#include <Auftrag/AufEintragZu.h>
#include <Aux/Trace.h>
#include <Lager/Lager.h>

std::string AuftragBase::str() const
{
  return instanz->Name()+"|"+itos(Id());
}

std::ostream &operator<<(std::ostream &o,const AuftragBase &a)
{
  return  o<<a.Instanz()->Name()<< "(" << a.Id() <<")";
}


/*
FetchIStream& operator>>(FetchIStream& is,AuftragBase::mengen_t &menge)
{
  is >> menge;
  return is;
}
*/


void AuftragBase::BaumAnlegen(const AufEintrag& AE,int uid,bool setInstanzAuftraege) const
{
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,
      "Artikel=",AE.Artikel(),
      "Status=",AE.getEntryStatus(),"LieferDatum=",AE.getLieferdatum(),
      "Menge=",AE.getStueck(),"setInstanzAuftrag=",setInstanzAuftraege);
  if(Instanz()==ppsInstanzID::Kundenauftraege)
   {
      cH_ppsInstanz i=ppsInstanz::getBestellInstanz(AE.Artikel());
      if (i!=ppsInstanzID::None && i!=ppsInstanzID::Kundenauftraege)
        {
           AuftragBase AB(ppsInstanz::getBestellInstanz(AE.Artikel()),AuftragBase::ungeplante_id);
           AB.tryUpdateEntry(AE.getStueck(),AE.getLieferdatum(),AE.Artikel(),AE.getEntryStatus(),uid,AE);
        }
   }
  else if(setInstanzAuftraege)
     InstanzAuftraegeAnlegen(AE,AE.getStueck(),uid);
}


/* 'AE.getStueck()' ist die GESAMTMenge 
   'menge' ist die AKTUELLE geänderte Menge
*/
void AuftragBase::InstanzAuftraegeAnlegen(const AufEintrag& AE,mengen_t menge,int uid,bool automatisch_geplant) const
{
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,
      "Artikel=",AE.Artikel(),
      "Status=",AE.getEntryStatus(),"LieferDatum=",AE.getLieferdatum(),
      "Menge=",menge);
   ArtikelBaum AB(AE.Artikel());
   for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
    {
      AuftragBase neuAuftrag(ppsInstanz::getBestellInstanz(i->rohartikel),ungeplante_id);
      // Instanzen, die selber Produzieren dürfen bei einer automatischen
      // Auftragsplanung (AuftragID=plan_auftrag_id=1)
      // NICHT erhöt werden.
      if(neuAuftrag.Instanz()->ProduziertSelbst() && automatisch_geplant)
         continue;

      Petig::Datum newdate=AE.getLieferdatum()-Instanz()->ProduktionsDauer();
      st_tryUpdateEntry st(automatisch_geplant);
      neuAuftrag.tryUpdateEntry(i->menge*menge,newdate,
                 i->rohartikel,AE.getEntryStatus(),uid,AE,st);
    }
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

void AuftragBase::dispo_auftrag_aendern(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,const mengen_t &menge,
         const ManuProC::Datum &datum,const AufEintragBase &kindAEB)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     "Artikel=",artikel,"Menge=",menge);
   AuftragBase da(instanz,AuftragBase::dispo_auftrag_id);
   int znr=-1,newznr=-1;
   AuftragBase::mengen_t oldmenge;
   bool alt=da.existEntry(artikel,datum,znr,newznr,oldmenge,OPEN);
   if(alt)
     {
      AuftragBase::mengen_t mt=AufEintragBase(da,znr).updateStkDiffBase__(uid,menge);
      assert(mt==menge);
     }
   else
     {
      Auftrag A(da);
      znr=A.push_back(menge,datum,artikel,OPEN,uid,false).ZNr();
     }
   if(kindAEB.valid())
      AufEintragZu(class AufEintragBase(da,znr)).setMengeDiff__(kindAEB,menge);
}


void AuftragBase::menge_neu_verplanen(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,
         const mengen_t &menge,const ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"Instanz=",instanz,
   "Artikel=",artikel,"Menge=",menge,
     "Reason=",reason);

  assert(instanz->LagerInstanz());
  assert(menge>=0);
  if(menge==0) return;

  SQLFullAuftragSelector sel(SQLFullAuftragSelector::
      sel_Artikel_Planung_id(instanz->Id(),
      Kunde::eigene_id,artikel,AuftragBase::ungeplante_id));
  SelectedFullAufList auftraglist=SelectedFullAufList(sel);
  
  mengen_t m=menge;
  for (SelectedFullAufList::iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t M=AuftragBase::min(i->getRestStk(),m);
     ArtikelImLager AIL(instanz,i->Artikel(),i->getLieferdatum());
     i->artikel_vormerken_oder_schnappen(false,M,i->Artikel(),uid,reason,AIL.getDispoAuftraege());
     if(reason==ManuProC::Auftrag::r_Anlegen || reason==ManuProC::Auftrag::r_Planen||
        reason==ManuProC::Auftrag::r_Closed)
            i->updateStkDiffInstanz__(uid,-M,reason);
     m-=M;
     if(menge==AuftragBase::mengen_t(0)) return;
   }
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

