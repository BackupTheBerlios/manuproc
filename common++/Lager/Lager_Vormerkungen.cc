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
//cout << vormerkmenge<<'\t'<<bestellmenge<<'\n';
//if(vormerkmenge>bestellmenge) return;
  assert(vormerkmenge<=bestellmenge);
  std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >  dispo_auftrag; 
//  AuftragBase::mengen_t menge_im_lager=artikel_auf_lager(dispo_auftrag);

//cout << "Vormerken: "<<*this<<' '<<menge_im_lager<<'\t'
//<<bestellmenge<<' '<<vormerkmenge<<' '<<(bestellmenge <= menge_im_lager)<<'\n';

  artikel_vormerken(vormerkmenge,Artikel(),uid,dispo_auftrag);      
  bestellmenge-=vormerkmenge;
  if(bestellmenge==AuftragBase::mengen_t(0)) return;

  // Vorgemerkte Menge mit späterem Lieferdatum
  AuftragBase::mengen_t menge_vorgemerkt=artikel_auf_lager(dispo_auftrag,false);
//cout << instanz->Name() <<"\tMENGE VORGEMERKT = "<<menge_vorgemerkt<<'\n';      
  if(menge_vorgemerkt<0) menge_vorgemerkt=0;
  AuftragBase::mengen_t Mv;
  if(bestellmenge <= menge_vorgemerkt) { Mv=bestellmenge; bestellmenge=0;}
  else {Mv=menge_vorgemerkt; bestellmenge -= menge_vorgemerkt; }

  artikel_schnappen(Mv,Artikel(),uid,dispo_auftrag);

//cout << "Restmenge = "<<bestellmenge<<'\n';
//cout << "Nu aber" <<Instanz()->LagerFuer()<<' '<<bestellmenge
//<<' '<<menge_im_lager<<'\n';
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

void Lager_Vormerkungen::artikel_vormerken(AuftragBase::mengen_t menge,
      const ArtikelBase &artikel,int uid,
      std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dispo_auftrag,
      bool reduce_old)
{
//cout << "VorM\t"<<menge<<'\n';
  if(menge==AuftragBase::mengen_t(0)) return ;

  if(dispo_auftrag.empty()) 
   {
     AuftragBase::mengen_t M = artikel_auf_lager(Artikel(),Instanz(),
            dispo_auftrag,getLieferdatum(),AuftragBase::dispo_auftrag_id);
     assert(M>=menge);
   }
  // Bei den 2er (dispo_auftrag_id) die verfügbare Menge reduzieren
  AuftragBase::mengen_t abmenge=menge;
  for(std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >::reverse_iterator i=dispo_auftrag.rbegin();i!=dispo_auftrag.rend();++i)
   {
     assert(i->first.Id()==AuftragBase::dispo_auftrag_id);
     AuftragBase::mengen_t use_menge;
     if(abmenge>i->second) use_menge=i->second;
     else                  use_menge=abmenge;

     AuftragBase::mengen_t mt=i->first.updateStkDiffBase__(uid,-use_menge);
     assert(mt==-use_menge);

     if(!Instanz()->LagerInstanz())
       {
//cout << *this<<'\t'<<i->first<<' '<<i->second<<'\t'<<' '<<use_menge<<'\t'<<'\n';
         move_menge_from_dispo_to_plan(uid,i->first,i->second);
       }
     abmenge-=use_menge;
     if(abmenge==AuftragBase::mengen_t(0)) break;
     assert(abmenge>AuftragBase::mengen_t(0));
   }
  if(Instanz()->LagerInstanz()) // neuen 1er anlegen
   {
     Planen(uid,menge,reduce_old,AuftragBase(Instanz()->Id(),AuftragBase::plan_auftrag_id),
            getLieferdatum()); 
   }
}        


void Lager_Vormerkungen::artikel_schnappen(AuftragBase::mengen_t menge,const ArtikelBase &artikel,int uid,std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dispo_auftrag)
{
  if(menge==AuftragBase::mengen_t(0)) return ;

  // Bei den 1er (plan_auftrag_id) die verfügbare Menge wegnehmen
  AuftragBase::mengen_t abmenge=menge;
//cout << "artikel_schnappen\t"<<dispo_auftrag.size()<<'\n';
  for(std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >::reverse_iterator i=dispo_auftrag.rbegin();i!=dispo_auftrag.rend();++i)
   {
     assert(i->first.Id()==AuftragBase::PlanId_for(instanz));
     AuftragBase::mengen_t use_menge;
     if(abmenge>i->second) use_menge=i->second;
     else                  use_menge=abmenge;
               
     AuftragBase::mengen_t mt=i->first.updateStkDiffBase__(uid,-use_menge);
     assert(mt==-use_menge);

     // Zuordnung von Elternliste an alten 1er reduzieren
     std::list<AufEintragZu::st_reflist> L=AufEintragZu(i->first).get_Referenz_list(i->first);
     AuftragBase::mengen_t M=use_menge;
     for(std::list<AufEintragZu::st_reflist>::iterator j=L.begin();j!=L.end();++j)
       {
         AuftragBase::mengen_t m;
         if(j->Menge >= M) m=M;
         else              m=j->Menge;
         AufEintragZu(j->AEB).setMengeDiff__(i->first,-m);
         AufEintrag(j->AEB).updateStkDiff__(uid,m,true);
         M-=m;
         if(M==AuftragBase::mengen_t(0)) break;
       }         
     abmenge-=use_menge;
     if(abmenge==AuftragBase::mengen_t(0)) break;
     assert(abmenge>AuftragBase::mengen_t(0));
    }
  if(Instanz()->LagerInstanz()) // neuen 1er anlegen
   {
     Planen(uid,menge,true,AuftragBase(Instanz()->Id(),AuftragBase::plan_auftrag_id),
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


void Lager_Vormerkungen::freigegeben_menge_neu_verplanen(cH_ppsInstanz instanz,
               const ArtikelBase& artikel,AuftragBase::mengen_t menge,int uid)
{
//cout << "Lager für Artikel: "<<cH_ArtikelBezeichnung(artikel)->Bezeichnung()
//<<"\tMenge: "<<menge<<'\n';
  assert(instanz->LagerInstanz());
  assert(menge>=0);
  if(menge==AuftragBase::mengen_t(0)) return;

  SQLFullAuftragSelector sel(SQLFullAuftragSelector::sel_Artikel_Planung_id(instanz->Id(),artikel,AuftragBase::ungeplante_id));
  SelectedFullAufList auftraglist=SelectedFullAufList(sel);
  for (SelectedFullAufList::const_iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t fehlende_menge = i->getRestStk();
     if(fehlende_menge==AuftragBase::mengen_t(0)) continue;
     if(fehlende_menge>=menge)
      {
//cout << "Freigegeben und KOMPLETT neu vorgemerkt für: "<<AufEintragBase(*i)<<' '<<fehlende_menge<<' '<< menge <<'\n';;
        std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dummy;
        Lager_Vormerkungen(*i).artikel_vormerken(menge,i->Artikel(),uid,dummy,true);
        return;
      }
     else
      {
//cout << "Freigegeben und TEILWEISE neu vorgemerkt für: "<<AufEintragBase(*i)<<' '<< menge << ' '<<fehlende_menge <<'\n';;
        std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dummy;
        Lager_Vormerkungen(*i).artikel_vormerken(fehlende_menge,i->Artikel(),uid,dummy,true);
        menge-=fehlende_menge;
      }
     assert(menge>=AuftragBase::mengen_t(0));
     if(menge==AuftragBase::mengen_t(0)) return;
   }
//if(auftraglist.empty()) cout << "Kein Auftrag zum neuverplanen\n";
}


AuftragBase::mengen_t  Lager_Vormerkungen::artikel_auf_lager(std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >  &dispo_auftrag,bool freie_menge)
{
/*
  if(freie_menge)
     return artikel_auf_lager(Artikel(),Instanz(),dispo_auftrag);
  else 
     return artikel_auf_lager(Artikel(),Instanz(),dispo_auftrag,getLieferdatum());
*/
  int id=AuftragBase::dispo_auftrag_id;
  if(!freie_menge) id=AuftragBase::PlanId_for(instanz);
  return artikel_auf_lager(Artikel(),Instanz(),dispo_auftrag,getLieferdatum(),id);
}


AuftragBase::mengen_t  Lager_Vormerkungen::artikel_auf_lager(const ArtikelBase &artikel,cH_ppsInstanz instanz,std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > &dispo_auftrag,const Petig::Datum &datum,int auftragsid)
{
  assert(auftragsid==AuftragBase::dispo_auftrag_id||auftragsid==AuftragBase::PlanId_for(instanz));
  // der Selector holt nur die Aufträge mit dem Status OPEN, 
  // geordnet nach Lieferdatum
  SelectedFullAufList auftraglist=SelectedFullAufList(SQLFullAuftragSelector::
      sel_Artikel_Planung_id(instanz->Id(),artikel,auftragsid)); 
  AuftragBase::mengen_t menge;
  std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >V_dispo_auftrag;
  for (SelectedFullAufList::const_iterator j=auftraglist.begin();j!=auftraglist.end();++j)
   {
     if(j->Id()==AuftragBase::dispo_auftrag_id) 
      { assert(j->getRestStk()==j->getStueck());
        if(j->getLieferdatum() < datum)
         { // Nur wenn die freie Menge VOR dem Liefertermin frei wird
           menge+=j->getStueck();
           V_dispo_auftrag.push_back(pair<AufEintragBase,AuftragBase::mengen_t>(*j,j->getStueck()));
         }
      }
     else if(j->Id()==AuftragBase::PlanId_for(instanz)) 
      {
        if(datum < j->getLieferdatum()) 
         {
           menge+=j->getRestStk();        
           V_dispo_auftrag.push_back(pair<AufEintragBase,AuftragBase::mengen_t>(*j,j->getRestStk()));
         }
      }
     else assert(!"Never get here");
   }
  if(menge>0)
      dispo_auftrag=V_dispo_auftrag;
  return menge;
}

void Lager_Vormerkungen::move_menge_from_dispo_to_plan(int uid,AufEintragBase dispo_aeb,mengen_t menge)
{
  assert(Id()==AuftragBase::ungeplante_id);
  AufEintrag dae(dispo_aeb);
  split(uid,menge,dae.getLieferdatum(),true);
  int dummy;
  mengen_t dummy_menge;
  int znr2;
  existEntry(Artikel(),dae.getLieferdatum(),znr2,dummy,dummy_menge,OPEN);
  AufEintragBase verplanter_aeb(*this,znr2);

  // Kinder des 2er (dispo) Auftrags
  std::list<AufEintragZu::st_reflist> K=AufEintragZu(dispo_aeb).get_Referenz_list(dispo_aeb,true);
  for(std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
   {
//cout << "Move Menge\t"<<dispo_aeb<<' '<<i->AEB<<' '<<verplanter_aeb<<'\n';
     AufEintragZu(dispo_aeb).setMengeDiff__(i->AEB,-menge);
     AufEintragZu(verplanter_aeb).setMengeDiff__(i->AEB,menge);
   }
}
