// $Id: AuftragBase.cc,v 1.28 2003/03/10 14:44:14 christof Exp $
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
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     NV("Artikel",artikel),NV("Menge",menge));
   AuftragBase(instanz,AuftragBase::dispo_auftrag_id)
   	.BestellmengeAendern(menge,datum,artikel,OPEN,uid,kindAEB);
}

// Ins Lager gekommene Menge neu vormerken
   // wird aufgerufen wenn Menge ins Lager kommt (LagerBase::rein_ins_lager)
   // und gibt reservierte Menge zurück
   // sollte Aufträge als produziert markieren
void AuftragBase::menge_neu_verplanen(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,
         const mengen_t &menge,const ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("Instanz",instanz),
   NV("Artikel",artikel),NV("Menge",menge),NV("Reason",reason));

//  assert(reason==ManuProC::Auftrag::r_Anlegen || reason==ManuProC::Auftrag::r_Planen||
//        reason==ManuProC::Auftrag::r_Closed || 
  assert(reason==ManuProC::Auftrag::r_Produziert);
  assert(instanz->LagerInstanz());
  assert(menge>=0);
  if(menge==0) return;
  
  Transaction tr;

  SQLFullAuftragSelector sel(SQLFullAuftragSelector::
      sel_Artikel_Planung_id(instanz->Id(),
      Kunde::eigene_id,artikel,AuftragBase::ungeplante_id));
  SelectedFullAufList auftraglist=SelectedFullAufList(sel);
  
  mengen_t m=menge;
  for (SelectedFullAufList::iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t M=AuftragBase::min(i->getRestStk(),m);
     if (!m) continue;
//     i->Produziert_0er(M);
//     i->abschreiben(
     // ich brauche hier keinen Artikel, ist das dann richtig?
     AufEintragZu::list_t eltern 
     	= AufEintragZu::get_Referenz_list(*i,AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
     // woher bekomme ich die passenden Eltern?
     // unser Datum gibt dann auch den 1er an.
     for (AufEintragZu::list_t::const_iterator j=eltern.begin();
     		j!=eltern.end();++j)
     {  // Sortierung egal?
        AuftragBase::mengen_t m2=AuftragBase::min(j->Menge,M);
        if (!m2) continue;

	AuftragBase ab1(instanz,AuftragBase::plan_auftrag_id);
	AufEintragBase aeb1(ab1,ab1.PassendeZeile(i->getLieferdatum(),artikel,OPEN,uid));
        i->ProduziertNG(uid,m2,j->AEB,aeb1);
        AufEintrag(aeb1).MengeAendern(uid,m2,false,j->AEB,ManuProC::Auftrag::r_Produziert);

        m-=m2;
        if(!m) break;
     }
     // Zuordnung erniedrigen, 1er erzeugen, 
     // 0er abbestellen, aber wo wird der 1er erhöht?
     // 0er nicht abbestellen sondern als Produziert markieren!!!
     // i->updateStkDiffInstanz__(uid,-M,AufEintragBase(),reason);
     // produziert markieren
//     @@@
     if(!m) break;
   }
   
   if (m!=0)
   {  AuftragBase::dispo_auftrag_aendern(uid,instanz,artikel,m,LagerBase::Lagerdatum(),AufEintragBase());
   }
#if 0 // na ... wozu soll das CP
     ManuProC::st_produziert sp(artikel,menge,uid);
     
     if(ppsInstanz::getProduktionsInstanz(artikel)->PlanungsInstanz())
        (*this)->Produziert(sp,ManuProC::Auftrag::r_Produziert);
     else if(!ppsInstanz::getProduktionsInstanz(artikel)->ProduziertSelbst())
           ppsInstanz::getProduktionsInstanz(artikel)->Produziert(sp);
#endif
   tr.commit();
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

