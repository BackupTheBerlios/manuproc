// $Id: AuftragBase.cc,v 1.10 2002/11/22 15:31:05 christof Exp $
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
#include<stdio.h>
#include <Aux/Transaction.h>
#include<Aux/Changejournal.h>
#include <Auftrag/auftrag_status.h>
#include "AufEintrag.h"
#include <Artikel/ArtikelStamm.h>
#include <Auftrag/AufEintragZu.h>
#include <Aux/Trace.h>

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
   ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,*this,
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
   ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,*this,
      "Artikel=",AE.Artikel(),
      "Status=",AE.getEntryStatus(),"LieferDatum=",AE.getLieferdatum(),
      "Menge=",menge);
   ArtikelBaum AB(AE.Artikel());
//cout << cH_ArtikelBezeichnung(artikel)->Bezeichnung()<<'\n';
   for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
    {
//cout << '\t'<<cH_ArtikelBezeichnung(i->rohartikel)->Bezeichnung()<<'\n';
      AuftragBase neuAuftrag(ppsInstanz::getBestellInstanz(i->rohartikel),ungeplante_id);
      // Instanzen, die selber Produzieren dürfen bei einer automatischen
      // Auftragsplanung (AuftragID=plan_auftrag_id=1)
      // NICHT erhöt werden.
      if(neuAuftrag.Instanz()->ProduziertSelbst() && automatisch_geplant)
         continue;

//cout << neuAuftrag.Instanz()->Name()<<'\t'
//<<neuAuftrag.Instanz()->ProduziertSelbst()<<'\t'<<automatisch_geplant<<'\n';

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
{ if(x<=y) return x; return y; }
AuftragBase::mengen_t AuftragBase::max(const mengen_t &x,const mengen_t &y)
{ if(x>=y) return x; return y; }

            
            
bool AuftragBase::editierbar() const
{
  if     (Id() == dispo_auftrag_id) return false;
  else if(ID() == plan_auftrag_id)  return false;
  else if(ID() == ungeplante_id)    return false;
  else if(Id() == none_id)          return false;
  return true;
}
