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

#include "Lager.hh"
#include "Lager_Vormerkungen.hh"
#include <Auftrag/AufEintrag.h>
#include <Auftrag/AuftragsEntryZuordnung.h>
#include <Auftrag/selFullAufEntry.h>

void Lager::abschreiben(ArtikelBase artikel,AuftragBase::mengen_t menge,
                        bool lager_rein,
                        ppsInstanz::ppsInstId abschreib_instanz,bool AuftragAbschreiben)
{
  assert(menge>=0);
  if(abschreib_instanz==ppsInstanz::None) 
   {
     if(!lager_rein) abschreib_instanz = instanz;
     else            abschreib_instanz = cH_ppsInstanz(instanz)->LagerFuer();
   }
  try{
    SQLFullAuftragSelector AEBabschreiben_(SQLFullAuftragSelector::sel_Kunde_Artikel
                                    (abschreib_instanz,Kunde::default_id,artikel));

    SelectedFullAufList AEBabschreiben(AEBabschreiben_);

    for(SelectedFullAufList::iterator i=AEBabschreiben.begin();i!=AEBabschreiben.end();++i)
      {
//cout << "\nAufEintrag: "<<i->Instanz()->Name()<<' '<<i->Id()<<' '<<i->ZNr()<<'\n'
//   <<"Benötigt werden am "<<i->getLieferdatum().c_str()<<" noch "
//   <<i->getRestStk()<<"   Menge:"<<menge<<'\n';

       AuftragBase::mengen_t abschreibmenge;       
       if(i->getRestStk() >= menge) abschreibmenge = menge;
       else                         abschreibmenge = i->getRestStk();


       if(AuftragAbschreiben)  i->abschreiben(abschreibmenge);

       if(lager_rein) Lager_Vormerkungen(*i).artikel_vormerken(abschreibmenge);
//       else           Lager_Vormerkungen(*i).artikel_ausliefern(abschreibmenge);
       else           (*i).abschreiben(abschreibmenge);

       menge-=abschreibmenge;
       if(!menge) break;
      }
   } catch(SQLerror &e)
     { std::cout << e <<'\n';}
}




Lager::Lager(ppsInstanz::ppsInstId _instanz)
: instanz(_instanz)
{
  check_lager_is_valid();
}


void Lager::check_lager_is_valid()
{
  if(cH_ppsInstanz(instanz)->LagerInstanz()==ppsInstanz::INST_NONE)
     assert(!"Lager::check_lager_is_valid: Kein gültiges Lager\n");
}

std::list<Lager::st_menge_sortiert>  Lager::getMengeSorted(ArtikelBase artikel)
{
 std::map<LagerPlatz,Lager::st_wo_ist_wieviel_rest> M=getPositionSorted(artikel);  
 std::list<st_menge_sortiert> L;
 for(std::map<LagerPlatz,Lager::st_wo_ist_wieviel_rest>::const_iterator i=M.begin();i!=M.end();++i)
  {
   L.push_back(st_menge_sortiert(i->first,i->second.volleRollenMeter,
                        i->second.volleRollen,
                        i->second.restRollenMeter,
                        i->second.restRollen,
                        i->second.GesamtGewichtetMeter));
  }
 L.sort(Sort());
 return L;
}



std::map<LagerPlatz,Lager::st_wo_ist_wieviel_rest> Lager::getPositionSorted(ArtikelBase artikel)
{
  std::vector<st_wo_ist_wieviel> V=getPosition(artikel);
  std::map<LagerPlatz,std::vector<st_wo_ist_wieviel> > M;
  // sortierung nach Position
  for(std::vector<st_wo_ist_wieviel>::const_iterator i=V.begin();i!=V.end();++i)
     M[i->position].push_back(st_wo_ist_wieviel(i->position,i->meter,i->rest,i->code));
  std::map<LagerPlatz,st_wo_ist_wieviel_rest> MR;
  const static double restefaktor=0.3;
  for(std::map<LagerPlatz,std::vector<st_wo_ist_wieviel> >::const_iterator i=M.begin();i!=M.end();++i)
   {
     for(std::vector<st_wo_ist_wieviel>::const_iterator j=i->second.begin();j!=i->second.end();++j)
      {
        if(j->rest)
         {
           MR[i->first].add_restRolle() ;
           MR[i->first].add_restRolleMeter(j->meter*restefaktor) ;
           MR[i->first].add_gesamtMeter(j->meter*restefaktor) ;
         }
        else 
         {
           MR[i->first].add_volleRolle() ;
           MR[i->first].add_volleRolleMeter(j->meter) ;
           MR[i->first].add_gesamtMeter(j->meter) ;
         }
      }
   }
  return MR;
}
