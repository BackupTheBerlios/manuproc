/* $Id: ArtikelImLager.cc,v 1.5 2002/12/24 08:25:22 thoma Exp $ */
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

#include "ArtikelImLager.h"
#include <Misc/Trace.h>
#include <Auftrag/selFullAufEntry.h>
#include <Misc/relops.h>

ArtikelImLager::ArtikelImLager(const cH_ppsInstanz &instanz,const ArtikelBase &artikel,
   const Petig::Datum &datum) throw(SQLerror)
: menge_dispo_auftraege(0),menge_plan_auftraege(0)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     "Instanz=",instanz,"Artikel=",artikel);

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
//cout << *i<<'\t'<<i->getLieferdatum() <<' '<< datum<<'\n';
        if(i->getLieferdatum() > datum) continue;
//cout << "\tdispo OK\n";
cout << "LD A: "<<i->Artikel()<<'\t'<<i->getRestStk()<<' '<<*i<<'\n';
        menge_dispo_auftraege+=i->getRestStk();
        V_dispo_auftraege.push_back(*i);
      }
     else // AuftragBase::handplan_auftrag_id || plan_auftrag_id
      {
//cout<<"ART:"<<i->Artikel().Id() << i->Artikel()<<'\n';
//if(i->Artikel().Id()!=209813 &&
//   i->Artikel().Id()!=211007) continue;
        if(i->getLieferdatum() <= datum) continue; 
        if(!i->Instanz()->LagerInstanz()) continue;
cout << "LD B: "<<i->Artikel()<<'\t'<<i->getRestStk()<<' '<<*i<<'\n';
        menge_plan_auftraege+=i->getRestStk();
        V_plan_auftraege.push_back(*i);
      }
   }
 assert(menge_dispo_auftraege>=0);
 assert(menge_plan_auftraege>=0);
}

#include <Auftrag/AufEintragZuMengenAenderung.h>
void ArtikelImLager::reduce_in_dispo_or_plan(const bool dispo,const int uid,AuftragBase::mengen_t menge) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     "Menge=",menge);
  std::vector<AufEintrag> V=V_dispo_auftraege;
  if(!dispo) V=V_plan_auftraege;
  std::vector<AufEintrag>::const_reverse_iterator e=V.rend();
  for(std::vector<AufEintrag>::const_reverse_iterator i=V.rbegin();i!=e;++i)
   {
     AuftragBase::mengen_t M=AuftragBase::min(i->getRestStk(),menge);
//cout << "reduce_in_dispo_or_plan  "<<*i<<'\t'<<dispo<<'\t'<<M<<'\n';
     i->updateStkDiffBase__(uid,-M);
     if(!dispo)  AufEintragZuMengenAenderung::increase_parents__reduce_assingments(uid,*i,M);
     if(!i->Instanz()->LagerInstanz()) AufEintragZuMengenAenderung::reduce_zuordung_to_20000er_from_2er(uid,*i,M);
     menge-=M;
     if(menge==0) break;
   }
}

