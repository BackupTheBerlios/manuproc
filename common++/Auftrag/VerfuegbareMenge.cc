/* $Id: VerfuegbareMenge.cc,v 1.8 2003/08/11 15:05:26 christof Exp $ */
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifdef PETIG_EXTENSIONS
#define MENGE_KLAUEN
#endif

VerfuegbareMenge::VerfuegbareMenge(const cH_ppsInstanz &_instanz,const ArtikelBase &artikel,
   const Petig::Datum &_datum) throw(SQLerror)
: ArtikelBase(artikel), menge_dispo_auftraege(0),menge_plan_auftraege(0),
	datum(_datum), instanz(_instanz)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     NV("Instanz",instanz),NV("Artikel",artikel));

#warning SQL Anfrage optimierbar CP
// wahrscheinlich wäre ein selector 2er (und im Lager 1er) bez. Datum
// sinnvoll !!!
  SelectedFullAufList auftraglist=SelectedFullAufList(SQLFullAuftragSelector::
     sel_Kunde_Artikel(instanz->Id(),Kunde::eigene_id,artikel));
  for (SelectedFullAufList::const_iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     // Nur wenn die freie Menge VOR dem Liefertermin frei wird AE verwenden
     if     (i->Id()==AuftragBase::none_id) ;
     else if(i->Id()==AuftragBase::ungeplante_id) ;
     else if(i->Id()==AuftragBase::dispo_auftrag_id)
      {
        assert(i->getRestStk()==i->getStueck());
        ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,i->getLieferdatum());
        if(i->getLieferdatum() > datum) continue;
	ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,i->getRestStk(),*i);
        menge_dispo_auftraege+=i->getRestStk();
        V_dispo_auftraege.push_back(*i);
      }
     else // AuftragBase::handplan_auftrag_id || plan_auftrag_id
      {
        if(i->getLieferdatum() <= datum) continue; 
        if(!i->Instanz()->LagerInstanz()) continue;
#ifdef MENGE_KLAUEN        
	ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,i->getRestStk(),*i);
        menge_plan_auftraege+=i->getRestStk();
        V_plan_auftraege.push_back(*i);
#endif        
      }
   }
 assert(menge_dispo_auftraege>=0);
 assert(menge_plan_auftraege>=0);
}

#include <Auftrag/AufEintragZuMengenAenderung.h>

AuftragBase::mengen_t VerfuegbareMenge::reduce_in_dispo_or_plan(const bool dispo,const int uid,AuftragBase::mengen_t menge,const AufEintragBase &ElternAEB) 
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
     i->updateStkDiffBase__(uid,-M);
     if(!dispo)  
        i->MengeNeubestellen(uid,M);
     if(!instanz->LagerInstanz())
     // diese nichtssagenden Aufrufe gefallen mir nicht CP
     {  AufEintragZuMengenAenderung::freie_dispomenge_verwenden(uid,*i,M,ElternAEB);
     }

     wieviel_geschafft+=M;
     menge-=M;
     if(menge==0) break;
   }
   if (wieviel_geschafft!=0 && instanz->LagerInstanz())
   {  AuftragBase vormerkungen(instanz,AuftragBase::plan_auftrag_id);
      vormerkungen.BestellmengeAendern(wieviel_geschafft,datum,ArtikelBase(*this),OPEN,uid,
			ElternAEB);
   }
   return wieviel_geschafft;
}

