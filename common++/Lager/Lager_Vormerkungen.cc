// $Id: Lager_Vormerkungen.cc,v 1.66 2002/11/06 15:06:59 malte Exp $
/*  libcommonc++: ManuProC's main OO library Copyright (C) 2002 Adolf Petig
 *  GmbH & Co. KG written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#include "Lager_Vormerkungen.h"
#include "Lager.h"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBaum.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <unistd.h>
#include <Artikel/ArtikelBezeichnung.h> // debug
#include <Misc/relops.h>
#include <Misc/Trace.h>
#include <Artikel/ArtikelImLager.h>
#include <Auftrag/AufEintragZuMengenAenderung.h>

Lager_Vormerkungen::Lager_Vormerkungen(const AufEintrag& ae)
: AufEintrag(ae)
{
}

/*
void Lager_Vormerkungen::vormerken_oder_bestellen(int uid,
      AuftragBase::mengen_t vormerkmenge,
      AuftragBase::mengen_t bestellmenge,
      AuftragBase::st_tryUpdateEntry st_bool)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"Vormerkmenge=",vormerkmenge,
      "Bestellmenge=",bestellmenge);

  assert(vormerkmenge<=bestellmenge);

  if(vormerkmenge>mengen_t(0))
     artikel_vormerken_oder_schnappen(false,vormerkmenge,Artikel(),uid,false);      
  bestellmenge-=vormerkmenge;
  if(bestellmenge==AuftragBase::mengen_t(0)) return;

  // Vorgemerkte Menge mit späterem Lieferdatum
  ArtikelImLager AAL(instanz,Artikel(),getLieferdatum());
  AuftragBase::mengen_t menge_vorgemerkt;
  std::vector<AufEintrag> plan_auftraege;
  if(instanz->LagerInstanz())  {menge_vorgemerkt=AAL.getMengePlan();plan_auftraege=AAL.getPlanAuftraege();}
  else                          menge_vorgemerkt=0;

  if(menge_vorgemerkt<0) menge_vorgemerkt=0;
  AuftragBase::mengen_t Mv;
  if(bestellmenge <= menge_vorgemerkt) { Mv=bestellmenge; bestellmenge=0;}
  else {Mv=menge_vorgemerkt; bestellmenge -= menge_vorgemerkt; }

  if(Mv>mengen_t(0))
     artikel_vormerken_oder_schnappen(true,Mv,Artikel(),uid,true,plan_auftraege);
  if(bestellmenge==AuftragBase::mengen_t(0)) return;


  if(Instanz()->LagerInstanz())
    {
      AuftragBase AB(ppsInstanz::getProduktionsInstanz(Artikel()),AuftragBase::ungeplante_id);
      AB.tryUpdateEntry(bestellmenge,getLieferdatum(),Artikel(),OPEN,uid,*this);
    }
  else
    {
      InstanzAuftraegeAnlegen(*this,bestellmenge,uid,st_bool.automatisch_geplant);
    }
 return ;
}
*/


void Lager_Vormerkungen::artikel_vormerken_oder_schnappen(bool schnappen,AuftragBase::mengen_t menge,
      const ArtikelBase &artikel,int uid,bool reduce_old,
      std::vector<AufEintrag> dispo_auftrag)
{
  ManuProC::Trace _t(ManuProC::Tracer::Auftrag, __FUNCTION__,"Schnappen=",schnappen,"Menge=",menge,
      "Artikel=",artikel,
      "Reduce_Old=",reduce_old,"SizeOfDispoAufträgen=",dispo_auftrag.size());

  if(menge==AuftragBase::mengen_t(0)) return ;
  if(dispo_auftrag.empty()) 
   {
     assert(!schnappen);
     ArtikelImLager AIL(Instanz(),Artikel(),getLieferdatum());
     AuftragBase::mengen_t M = AIL.getMengeDispo();
     dispo_auftrag=AIL.getDispoAuftraege();
     assert(M>=menge);
   }
  // Bei den 2er (dispo_auftrag_id) die verfügbare Menge reduzieren
  // Bei den 1er (plan_auftrag_id) die  verfügbare Menge wegnehmen
  AuftragBase::mengen_t abmenge=menge;
  for(std::vector<AufEintrag>::reverse_iterator i=dispo_auftrag.rbegin();i!=dispo_auftrag.rend();++i)
   {
     if(schnappen) assert(i->Id()==AuftragBase::plan_auftrag_id);
     else          assert(i->Id()==AuftragBase::dispo_auftrag_id);
     AuftragBase::mengen_t use_menge;
     if(abmenge>i->getRestStk()) use_menge=i->getRestStk();
     else                        use_menge=abmenge;

     AuftragBase::mengen_t mt=i->updateStkDiffBase__(uid,-use_menge);
     assert(mt==-use_menge);

     if(schnappen)
      {
         AufEintragZuMengenAenderung::increase_parents__reduce_assingments(uid,*i,use_menge);
      }
     else
      {  if(!Instanz()->LagerInstanz())
            AufEintragZuMengenAenderung::change_parent(uid,*i,*this,use_menge);
      }
     abmenge-=use_menge;
     if(abmenge==AuftragBase::mengen_t(0)) break;
     assert(abmenge>AuftragBase::mengen_t(0));
   }
  if(Instanz()->LagerInstanz()) // neuen 1er anlegen
   {
     if(schnappen) assert(reduce_old);
     Planen(uid,menge,reduce_old,AuftragBase(Instanz()->Id(),AuftragBase::plan_auftrag_id),
            getLieferdatum()); 
   }
}        


/*
int ProdLager::Lieferzeit_in_Tagen()
{
  double dauer;
  if (instanz==ppsInstanz::INST_WEB)
   {
     Maschinen_Geschwindigkeit MG(artikel);
     dauer = menge_bestellt/MG.Speed_m_Tag();
   }
  return dauer;
}
*/

