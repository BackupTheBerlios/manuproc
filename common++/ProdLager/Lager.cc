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
#include "ProdLager.hh"
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/AuftragsEntryZuordnung.h>
#include <Auftrag/selFullAufEntry.h>

void Lager::abschreiben(ArtikelBase artikel,AuftragBase::mengen_t menge,
                        RL_LogTyp rl_typ,Jumbo_LogTyp jumbo_typ)
{
  assert( !((rl_typ==RLNone && jumbo_typ==None) ||
            (rl_typ!=RLNone && jumbo_typ!=None)) );
  ppsInstanz::ppsInstId instanz_abschreiben;
  bool lager_raus=false;
  bool lager_rein=false;
  if(rl_typ==RLEntnehmen || jumbo_typ==Auslagern) lager_raus=true;

  else if(rl_typ==RLEinlagern || jumbo_typ==Einlagern ||
                                 jumbo_typ==Einlagern_L ||
                                 jumbo_typ==Rest ||
                                 jumbo_typ==Rest_L ||
                                 jumbo_typ==Zurueck ) lager_rein=true;
      

  if(lager_raus)      instanz_abschreiben = instanz;
  else if(lager_rein) instanz_abschreiben = cH_ppsInstanz(instanz)->LagerFuer();
  try{
    SQLFullAuftragSelector AEBabschreiben_(SQLFullAuftragSelector::sel_Kunde_Artikel
                                    (instanz_abschreiben,Kunde::default_id,artikel));

    SelectedFullAufList AEBabschreiben(AEBabschreiben_);
    for(SelectedFullAufList::iterator i=AEBabschreiben.begin();i!=AEBabschreiben.end();++i)
      {
cout << "\nAufEintragBase: "<<i->Instanz()<<' '<<i->Id()<<' '<<i->ZNr()<<'\n'
   <<"Benötigt werden am "<<i->getLieferdatum().c_str()<<" noch "
   <<i->getRestStk()<<"   Menge:"<<menge<<'\n';

       AuftragBase::mengen_t abschreibmenge;       
       if(i->getRestStk() >= menge) abschreibmenge = menge;
       else                         abschreibmenge = i->getRestStk();

       i->abschreiben(abschreibmenge);
       if(lager_rein)      ProdLager(*i).artikel_vormerken(abschreibmenge);
       else if(lager_raus) ProdLager(*i).artikel_ausliefern(abschreibmenge);

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
  if(instanz==ppsInstanz::INST_NONE)
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
