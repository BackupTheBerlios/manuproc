/* $Id: VerfuegbareMenge.cc,v 1.12 2004/09/06 10:36:01 christof Exp $ */
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2004 Adolf Petig GmbH & Co. KG
 *  written by Malte Thoma, Christof Petig
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

#include "VerfuegbareMenge.h"
#include <Misc/TraceNV.h>
#include <Auftrag/selFullAufEntry.h>
#include <Misc/relops.h>
#include <Artikel/ArtikelStamm.h>

#ifdef PETIG_EXTENSIONS
//#define MENGE_KLAUEN
#endif

static AuftragBase::mengen_t FreieMenge2er(const AufEintrag &zweier)
{  if (!zweier.Instanz()->LagerInstanz()) return zweier.getRestStk();

    // nach nachbestellten 2ern suchen ...
#if 0    
    AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel
            (zweier,AufEintragZu::list_kinder,AufEintragZu::list_unsorted));
    ManuProC::Trace(trace_channel, __FUNCTION__,zweier,
          NV("Menge 2er",zweier.getRestStk()),
          NV("Anz. Kinder",MapArt.size()));
    // Optimierung
    if (MapArt.empty()) return zweier.getRestStk();
    return zweier.getRestStk()-Summe(MapArt[Artikel()]);
#else // schneller
    AufEintragZu::list_t nachbestellt = AufEintragZu::get_Referenz_list
                    (zweier,AufEintragZu::list_kinder,
                   AufEintragZu::list_ohneArtikel,AufEintragZu::list_unsorted);
    if (nachbestellt.empty()) return zweier.getRestStk();
    return zweier.getRestStk()-Summe(nachbestellt);
#endif
}

VerfuegbareMenge::VerfuegbareMenge(const cH_ppsInstanz &_instanz,const ArtikelBase &artikel,
   const Petig::Datum &_datum) throw(SQLerror)
: ArtikelBase(artikel), menge_dispo_auftraege(),menge_plan_auftraege(),
	datum(_datum), instanz(_instanz)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     NV("Instanz",instanz),NV("Artikel",artikel));

#ifndef MENGE_KLAUEN // Optimization (Lager: nur ein 2er)
  if (instanz->LagerInstanz())
  {  AuftragBase::mengen_t dummy=0;
     int znr_2er=AuftragBase(instanz,AuftragBase::dispo_id)
          .existEntry(artikel,LagerBase::Lagerdatum(),OPEN,dummy);
     if (znr_2er==AuftragBase::none_id) return;
     AufEintragBase AEB2er(make_value(AuftragBase(instanz,AuftragBase::dispo_id)),znr_2er);
     menge_dispo_auftraege=FreieMenge2er(AEB2er);
     V_dispo_auftraege.push_back(AEB2er);
     return;
  }
#endif

#warning SQL Anfrage optimierbar CP
// wahrscheinlich wäre ein selector 2er (und im Lager 1er) bez. Datum
// sinnvoll !!!
  SelectedFullAufList auftraglist=SelectedFullAufList(SQLFullAuftragSelector::
#ifdef MENGE_KLAUEN  
     sel_Kunde_Artikel(instanz->Id(),Kunde::eigene_id,artikel));
#else
     sel_Artikel_Planung_id(instanz->Id(),Kunde::eigene_id,artikel,AuftragBase::dispo_id));
#endif
  for (SelectedFullAufList::const_iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     // Nur wenn die freie Menge VOR dem Liefertermin frei wird AE verwenden
     if     (i->Id()==AuftragBase::none_id) ;
     else if(i->Id()==AuftragBase::ungeplante_id) ;
     else if(i->Id()==AuftragBase::dispo_id)
      {
        assert(i->getRestStk()==i->getStueck());
        ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,i->getLieferdatum());
        if(i->getLieferdatum() > datum) continue;
        AuftragBase::mengen_t freieMenge=FreieMenge2er(*i);
	ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,freieMenge,*i);
        menge_dispo_auftraege+=freieMenge;
        V_dispo_auftraege.push_back(*i);
      }
     else // AuftragBase::handplan_auftrag_id || plan_auftrag_id
      {
#ifdef MENGE_KLAUEN        
        if(i->getLieferdatum() <= datum) continue; 
        if(!i->Instanz()->LagerInstanz()) continue;
	ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,i->getRestStk(),*i);
        menge_plan_auftraege+=i->getRestStk();
        V_plan_auftraege.push_back(*i);
#endif        
      }
   }
 assert(menge_dispo_auftraege>=0);
 assert(menge_plan_auftraege>=0);
 ManuProC::Trace(AuftragBase::trace_channel,"=",NV("dispo",menge_dispo_auftraege),
           NV("plan",menge_plan_auftraege));
}

#include <Auftrag/AufEintragZuMengenAenderung.h>

AuftragBase::mengen_t VerfuegbareMenge::reduce_in_dispo_or_plan(const bool dispo,AuftragBase::mengen_t menge,const AufEintragBase &ElternAEB) 
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     instanz,ArtikelBase(*this),dispo?"dispo":"plan",NV("menge",menge),NV("ElternAEB",ElternAEB));
  std::vector<AufEintrag> &V=dispo?V_dispo_auftraege:V_plan_auftraege;

  AuftragBase::mengen_t wieviel_geschafft=0;
  std::vector<AufEintrag>::reverse_iterator e=V.rend();
  for(std::vector<AufEintrag>::reverse_iterator i=V.rbegin();i!=e;++i)
   {
     AuftragBase::mengen_t M=AuftragBase::min(i->getRestStk(),menge);
     ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,*i,M);
     if (!M) continue;

     // oder erst nachbestellen, dann (Lager)vormerkung freigeben ??
     if(!dispo)  
        i->MengeNeubestellen(M);
     else // DispoAuftrag
     {  i->MengeAendern(-M,false,AufEintragBase());
        ArtikelStamm astamm(i->Artikel());
        if (instanz->LagerInstanz() && astamm.getMindBest()>0
            && i->getRestStk()<astamm.getMindBest())
        {  AuftragBase::mengen_t differenz=AuftragBase::mengen_t(astamm.getMindBest())-i->getRestStk();
           i->MengeAendern(differenz,false,AufEintragBase());
           ppsInstanz::ID next=i->Instanz()->NaechsteInstanz(astamm);
           assert(next!=ppsInstanzID::None);
           AufEintrag::ArtikelInternNachbestellen(cH_ppsInstanz(next),differenz,datum,i->Artikel(),*i);
        }
     }
     if(!instanz->LagerInstanz())
     // notwendig?
     {  AufEintragZuMengenAenderung::freie_dispomenge_verwenden(*i,M,ElternAEB);
     }

     wieviel_geschafft+=M;
     menge-=M;
     if(menge==0) break;
   }
   if (wieviel_geschafft!=0 && instanz->LagerInstanz())
   {  AuftragBase vormerkungen(instanz,AuftragBase::plan_id);
      vormerkungen.BestellmengeAendern(wieviel_geschafft,datum,ArtikelBase(*this),OPEN,
			ElternAEB);
   }
   return wieviel_geschafft;
}

#ifndef MENGE_KLAUEN // Optimization (Lager: nur ein 2er)
VerfuegbareMenge::VerfuegbareMenge(const AufEintrag &lager_zweier)
: ArtikelBase(lager_zweier.Artikel()), menge_dispo_auftraege(),
        menge_plan_auftraege(),
	datum(lager_zweier.getLieferdatum()), instanz(lager_zweier.Instanz())
{ ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("lager_zweier",lager_zweier));

  assert (instanz->LagerInstanz());
  menge_dispo_auftraege=FreieMenge2er(lager_zweier);
  V_dispo_auftraege.push_back(lager_zweier);
}
#endif
