// $Id: Lager_Vormerkungen.cc,v 1.54 2002/10/04 13:29:27 malte Exp $
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
//#include "maschinen_geschwindigkeit.hh"
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <unistd.h>


#include <Artikel/ArtikelBezeichnung.h> // debug

Lager_Vormerkungen::Lager_Vormerkungen(const AufEintrag& ae)
: AufEintrag(ae)
{
//  assert ( Instanz()->LagerFuer()!=ppsInstanzID::None);
}

void Lager_Vormerkungen::vormerken_oder_bestellen(int uid,
      AuftragBase::mengen_t vormerkmenge,
      AuftragBase::mengen_t bestellmenge)
{
//cout << "Vormerken: "<<*this<<'\n';
//cout <<"\tMenge zum VORMERKEN="<<vormerkmenge<<'\n';
//cout <<"\tMenge zum BESTELLEN="<<bestellmenge<<'\n';
  assert(vormerkmenge<=bestellmenge);
  std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >  dispo_auftrag; 
//  AuftragBase::mengen_t menge_im_lager=artikel_auf_lager(dispo_auftrag);

  if(vormerkmenge>mengen_t(0))
     artikel_vormerken_oder_schnappen(false,vormerkmenge,Artikel(),uid,dispo_auftrag,false);      
  bestellmenge-=vormerkmenge;
  if(bestellmenge==AuftragBase::mengen_t(0)) return;

//cout << "\tRestmenge: "<<bestellmenge<<'\n';

  // Vorgemerkte Menge mit sp�terem Lieferdatum
  AuftragBase::mengen_t menge_vorgemerkt=artikel_auf_lager(dispo_auftrag,false);
//cout << instanz->Name() <<"\tMENGE VORGEMERKT = "<<menge_vorgemerkt<<'\n';      
  if(menge_vorgemerkt<0) menge_vorgemerkt=0;
  AuftragBase::mengen_t Mv;
  if(bestellmenge <= menge_vorgemerkt) { Mv=bestellmenge; bestellmenge=0;}
  else {Mv=menge_vorgemerkt; bestellmenge -= menge_vorgemerkt; }

  if(Mv>mengen_t(0))
{
//cout <<*this<<"\tMenge zum SCHNAPPEN="<<Mv<<'\n';
     artikel_vormerken_oder_schnappen(true,Mv,Artikel(),uid,dispo_auftrag,true);
}
//cout << "\t\tRestmenge = "<<bestellmenge<<'\n';
  if(bestellmenge==AuftragBase::mengen_t(0)) return;

  if(Instanz()->LagerFuer()!=ppsInstanzID::None)
    {
      AuftragBase AB(Instanz()->LagerFuer(),AuftragBase::ungeplante_id);
      AB.tryUpdateEntry(bestellmenge,getLieferdatum(),Artikel(),getEntryStatus(),uid,*this);
    }
  else
    {
      InstanzAuftraegeAnlegen(Artikel(),ZNr(),getLieferdatum(),getEntryStatus(),uid,bestellmenge);
    }
 return ;
}

void Lager_Vormerkungen::artikel_vormerken_oder_schnappen(bool schnappen,AuftragBase::mengen_t menge,
      const ArtikelBase &artikel,int uid,
      std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dispo_auftrag,
      bool reduce_old)
{
  if(menge==AuftragBase::mengen_t(0)) return ;
//cout << "VorM\t"<<menge<<'\t'<< dispo_auftrag.size()<<'\n';
  if(dispo_auftrag.empty()) 
   {
     assert(!schnappen);
     AuftragBase::mengen_t M = artikel_auf_lager(Artikel(),Instanz(),
            dispo_auftrag,getLieferdatum(),AuftragBase::dispo_auftrag_id);
     assert(M>=menge);
   }
  // Bei den 2er (dispo_auftrag_id) die verf�gbare Menge reduzieren
  // Bei den 1er (plan_auftrag_id) die  verf�gbare Menge wegnehmen
  AuftragBase::mengen_t abmenge=menge;
//cout << "VorM\t"<<menge<<'\t'<< dispo_auftrag.size()<<'\n';
  for(std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >::reverse_iterator i=dispo_auftrag.rbegin();i!=dispo_auftrag.rend();++i)
   {
     if(schnappen) assert(i->first.Id()==AuftragBase::plan_auftrag_id);
     else          assert(i->first.Id()==AuftragBase::dispo_auftrag_id);
     AuftragBase::mengen_t use_menge;
     if(abmenge>i->second) use_menge=i->second;
     else                  use_menge=abmenge;

//cout << i->first<<'\t'<<AufEintrag(i->first).getStueck()<<' '
//<<AufEintrag(i->first).getRestStk()<<'\n';
     AuftragBase::mengen_t mt=i->first.updateStkDiffBase__(uid,-use_menge);
     assert(mt==-use_menge);

     if(schnappen)
      {
        reduce_old_plan_auftrag(uid,i->first,use_menge);
      }
     else
      {  if(!Instanz()->LagerInstanz())
{
//cout <<"REDUCE: "<<*this<<'\t'<<i->first<<'\t'<<use_menge<<'\n';
            move_menge_from_dispo_to_plan(uid,i->first,use_menge);
}
      }
     abmenge-=use_menge;
     if(abmenge==AuftragBase::mengen_t(0)) break;
     assert(abmenge>AuftragBase::mengen_t(0));
   }
  if(Instanz()->LagerInstanz()) // neuen 1er anlegen
   {
     if(schnappen) assert(reduce_old);
//cout << "PP1\t"<<*this<<'\t'<<getRestStk()<<' '<<menge<<'\t'<<'\n';
     Planen(uid,menge,reduce_old,AuftragBase(Instanz()->Id(),AuftragBase::plan_auftrag_id),
            getLieferdatum()); 
//cout << "PP2\t"<<*this<<'\t'<<getRestStk()<<' '<<menge<<'\n';
   }
}        


void Lager_Vormerkungen::reduce_old_plan_auftrag(int uid,AufEintragBase aeb,mengen_t menge)
{
  // Zuordnung von Elternliste an alten 1er reduzieren
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(aeb).get_Referenz_list(aeb);
  AuftragBase::mengen_t M=menge;
  for(std::list<AufEintragZu::st_reflist>::iterator j=L.begin();j!=L.end();++j)
    {
      AuftragBase::mengen_t m;
      if(j->Menge >= M) m=M;
      else              m=j->Menge;
      AufEintragZu(j->AEB).setMengeDiff__(aeb,-m);
      AufEintrag(j->AEB).updateStkDiff__(uid,m,true,AufEintragBase::r_Produziert);
      M-=m;
      if(M==AuftragBase::mengen_t(0)) break;
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


void Lager_Vormerkungen::freigegeben_menge_neu_verplanen(cH_ppsInstanz instanz,
               const ArtikelBase& artikel,AuftragBase::mengen_t menge,int uid,AufEintragBase::e_reduce_reason reason)
{
//cout << "Lager f�r Artikel: "<<cH_ArtikelBezeichnung(artikel)->Bezeichnung()
//<<"\tMenge: "<<menge<<'\n';
  assert(instanz->LagerInstanz());
  assert(menge>=0);
  if(menge==AuftragBase::mengen_t(0)) return;

  SQLFullAuftragSelector sel(SQLFullAuftragSelector::
      sel_Artikel_Planung_id(instanz->Id(),
      ManuProC::DefaultValues::EigeneKundenId,
      artikel,AuftragBase::ungeplante_id));
  SelectedFullAufList auftraglist=SelectedFullAufList(sel);
  for (SelectedFullAufList::iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t fehlende_menge = i->getRestStk();
     if(fehlende_menge==AuftragBase::mengen_t(0)) continue;
     AuftragBase::mengen_t M;

     if(fehlende_menge>=menge) M=menge;
     else M=fehlende_menge;
     menge-=M;
     std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dummy;
     Lager_Vormerkungen(*i).artikel_vormerken_oder_schnappen(false,M,i->Artikel(),uid,dummy,true);
     if(reason==AufEintragBase::r_Standard ||
        reason==AufEintragBase::r_Closed) 
            i->updateStkDiffInstanz__(uid,-M,reason/*,0,0*/);
     if(menge==AuftragBase::mengen_t(0)) return;
   }
//if(auftraglist.empty()) cout << "Kein Auftrag zum neuverplanen\n";
}


AuftragBase::mengen_t  Lager_Vormerkungen::artikel_auf_lager(std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >  &dispo_auftrag,bool freie_menge)
{
  int id=AuftragBase::dispo_auftrag_id;
  if(!freie_menge) 
   { 
      if(!instanz->LagerInstanz()) return 0;
      id=AuftragBase::PlanId_for(instanz);
      assert(id==plan_auftrag_id);
   }
  return artikel_auf_lager(Artikel(),Instanz(),dispo_auftrag,getLieferdatum(),id);
}


AuftragBase::mengen_t  Lager_Vormerkungen::artikel_auf_lager(const ArtikelBase &artikel,cH_ppsInstanz instanz,std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > &dispo_auftrag,const Petig::Datum &datum,int auftragsid)
{
  assert(auftragsid==AuftragBase::dispo_auftrag_id||auftragsid==AuftragBase::PlanId_for(instanz));
  // der Selector holt nur die Auftr�ge mit dem Status OPEN, 
  // geordnet nach Lieferdatum
  SelectedFullAufList auftraglist=SelectedFullAufList(SQLFullAuftragSelector::
      sel_Artikel_Planung_id(instanz->Id(),ManuProC::DefaultValues::EigeneKundenId,artikel,auftragsid)); 
  AuftragBase::mengen_t menge;
  std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >V_dispo_auftrag;
  for (SelectedFullAufList::const_iterator j=auftraglist.begin();j!=auftraglist.end();++j)
   {
     // Nur wenn die freie Menge VOR dem Liefertermin frei wird
//cout << "Sel: "<<j->Id()<<' '<<j->Instanz()->Name()<<'\t'<<j->getLieferdatum() <<">="
//<<datum<<'\t'<<j->getRestStk()<<'\n';

     if(j->Id()==AuftragBase::dispo_auftrag_id)
      { assert(j->getRestStk()==j->getStueck());
        if(j->getLieferdatum() > datum) continue;  }
     else 
      { if(j->getLieferdatum() <= datum) continue; }

     menge+=j->getRestStk();        
     V_dispo_auftrag.push_back(pair<AufEintragBase,AuftragBase::mengen_t>(*j,j->getRestStk()));
   }
  if(menge>0)
      dispo_auftrag=V_dispo_auftrag;
//cout << "\t"<<menge<<'\n';
  return menge;
}

void Lager_Vormerkungen::move_menge_from_dispo_to_plan(int uid,AufEintragBase dispo_aeb,mengen_t menge)
{
  assert(Id()==AuftragBase::ungeplante_id);
  AufEintrag dae(dispo_aeb);
  std::list<AufEintragZu::st_reflist> K=AufEintragZu(dispo_aeb).get_Referenz_list(dispo_aeb,true);
//cout << "Move to dispo "<<*this<<'\t'<<dispo_aeb<<'\n';
//  assert(K.size()==1);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
   {
     assert(//i->AEB.Id()!=AuftragBase::ungeplante_id &&
            i->AEB.Id()!=AuftragBase::dispo_auftrag_id &&
            i->AEB.Id()!=AuftragBase::plan_auftrag_id);
//cout << "\t"<<*this<<'\t'<<i->AEB<<'\t'<<menge<<'\n';
     AufEintragZu(*this).Neu(i->AEB,menge); // Neu macht auch update
     AufEintragZu(dispo_aeb).Neu(i->AEB,-menge); // Neu macht auch update
   }
return;
}

/*
void Lager_Vormerkungen::move_menge_from_dispo_to_plan2(int uid,AufEintragBase dispo_aeb,mengen_t menge)
{
  assert(Id()==AuftragBase::ungeplante_id);
  AufEintrag dae(dispo_aeb);
  std::list<AufEintragZu::st_reflist> K=AufEintragZu(dispo_aeb).get_Referenz_list(dispo_aeb,true);
//cout << "Move to dispo "<<*this<<'\t'<<menge<<'\t'<<dispo_aeb<<'\n';
//  assert(K.size()==1);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
   {
//cout << "\t"<<*this<<'\t'<<i->AEB<<'\t'<<menge<<'\n';
     AufEintragZu(*this).Neu(i->AEB,menge); // Neu macht auch update
     AufEintragZu(dispo_aeb).Neu(i->AEB,-menge); // Neu macht auch update
   }
return;
}
*/
