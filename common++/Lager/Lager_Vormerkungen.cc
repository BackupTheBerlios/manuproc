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

void Lager_Vormerkungen::vormerken_oder_bestellen(int uid)
{
  std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >  dispo_auftrag; 
  AuftragBase::mengen_t menge_im_lager=artikel_auf_lager(dispo_auftrag);
  if (getRestStk() <= menge_im_lager) // Artikel vormerken
    {
      artikel_vormerken(getRestStk(),Artikel(),uid,dispo_auftrag);
    }
  else  
    { 
      if(menge_im_lager<0) menge_im_lager=0;
      if(menge_im_lager) artikel_vormerken(menge_im_lager,Artikel(),uid,dispo_auftrag);

      // Vorgemerkte Menge mit späterem Lieferdatum
      AuftragBase::mengen_t menge_vorgemerkt=artikel_auf_lager(dispo_auftrag,false);
//cout << instanz->Name() <<"\tMENGE VORGEMERKT = "<<menge_vorgemerkt<<'\n';      
      if(menge_vorgemerkt<0) menge_vorgemerkt=0;
      if(getRestStk() <= menge_vorgemerkt)
       {
         artikel_schnappen(getRestStk(),Artikel(),uid,dispo_auftrag);
       }
      else if(menge_vorgemerkt > AuftragBase::mengen_t(0)) 
       { 
         artikel_schnappen(menge_vorgemerkt,Artikel(),uid,dispo_auftrag);
       }
      if(getRestStk()==AuftragBase::mengen_t(0)) return;


      if(Instanz()->LagerFuer()!=ppsInstanzID::None)
       {
         AuftragBase AB(Instanz()->LagerFuer(),AuftragBase::ungeplante_id);
         AB.tryUpdateEntry(getRestStk(),getLieferdatum(),
            ArtId(),getEntryStatus(),uid,*this);
       }
      else
       {
        AuftragBase::mengen_t m=getRestStk();
        InstanzAuftraegeAnlegen(ArtId(),ZNr(),getLieferdatum(),getEntryStatus(),uid,m);
       }
    }
}

void Lager_Vormerkungen::artikel_vormerken(AuftragBase::mengen_t menge,const ArtikelBase &artikel,int uid,std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dispo_auftrag)
{
  if(menge==AuftragBase::mengen_t(0)) return ;

  if(dispo_auftrag.empty()) 
   {
     AuftragBase::mengen_t M = artikel_auf_lager(artikel,instanz,dispo_auftrag);
     assert(M>=menge);
   }
  // Bei den 2er (dispo_auftrag_id) die verfügbare Menge reduzieren
  AuftragBase::mengen_t abmenge=menge;
  for(std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >::iterator i=dispo_auftrag.begin();i!=dispo_auftrag.end();++i)
   {
     assert(i->first.Id()==AuftragBase::dispo_auftrag_id);
     AuftragBase::mengen_t use_menge;
     if(abmenge>i->second) use_menge=i->second;
     else                  use_menge=abmenge;

     AuftragBase::mengen_t mt=i->first.updateStkDiffBase__(uid,-use_menge);
     assert(mt==-use_menge);
     if(!Instanz()->LagerInstanz())
       {
         std::list<AufEintragZu::st_reflist> L=AufEintragZu(i->first).
                                        get_Referenz_list(i->first,true);
         assert(L.size()==1); // 2er haben GENAU einen Elternauftrag
         AufEintragZu(*this).Neu(L.begin()->AEB,use_menge);
         AuftragBase::mengen_t mtr=updateStkDiffBase__(uid,-use_menge);
         assert(mtr==-use_menge);
       }
     abmenge-=use_menge;
     if(abmenge==AuftragBase::mengen_t(0)) break;
     assert(abmenge>AuftragBase::mengen_t(0));
   }
  if(Instanz()->LagerInstanz()) // neuen 1er anlegen
   {
     Planen(uid,menge,AuftragBase(Instanz()->Id(),AuftragBase::plan_auftrag_id),
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
     Planen(uid,menge,AuftragBase(Instanz()->Id(),AuftragBase::plan_auftrag_id),
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


void Lager_Vormerkungen::reduziere_ungeplant(int uid,AuftragBase::mengen_t menge)
{
  assert(menge>0);
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_list_ungeplant(); 
  assert(L.begin()==++L.end()); // Lager kann nur bei EINER Instanz geplant sein
  AufEintrag(L.begin()->AEB).updateStkDiff__(uid,-menge,true);
  AuftragBase::mengen_t mt=AufEintragZu(*this).setMengeDiff__(L.begin()->AEB,-menge);
  assert(mt==-menge);
}


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
        Lager_Vormerkungen(*i).artikel_vormerken(menge,i->Artikel(),uid,dummy);
//        Lager_Vormerkungen(*i).reduziere_ungeplant(uid,menge);
        return;
      }
     else
      {
//cout << "Freigegeben und TEILWEISE neu vorgemerkt für: "<<AufEintragBase(*i)<<' '<< menge << ' '<<fehlende_menge <<'\n';;
        std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > dummy;
        Lager_Vormerkungen(*i).artikel_vormerken(fehlende_menge,i->Artikel(),uid,dummy);
//        Lager_Vormerkungen(*i).reduziere_ungeplant(uid,fehlende_menge);
        menge-=fehlende_menge;
      }
     assert(menge>=AuftragBase::mengen_t(0));
     if(menge==AuftragBase::mengen_t(0)) return;
   }
//if(auftraglist.empty()) cout << "Kein Auftrag zum neuverplanen\n";
}


AuftragBase::mengen_t  Lager_Vormerkungen::artikel_auf_lager(std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >  &dispo_auftrag,bool freie_menge)
{
  if(freie_menge)
     return artikel_auf_lager(Artikel(),Instanz(),dispo_auftrag);
  else 
     return artikel_auf_lager(Artikel(),Instanz(),dispo_auftrag,getLieferdatum());
}


AuftragBase::mengen_t  Lager_Vormerkungen::artikel_auf_lager(const ArtikelBase &artikel,cH_ppsInstanz instanz,std::vector<pair<AufEintragBase,AuftragBase::mengen_t> > &dispo_auftrag,const Petig::Datum &datum)
{
  int id=AuftragBase::dispo_auftrag_id;
  if(datum!=Lager::Lagerdatum()) id=AuftragBase::PlanId_for(instanz);
  // der Selector holt nur die Aufträge mit dem Status OPEN, 
  // geordnet nach Lieferdatum
  SelectedFullAufList auftraglist=SelectedFullAufList(SQLFullAuftragSelector::
      sel_Artikel_Planung_id(instanz->Id(),artikel,id)); 
  AuftragBase::mengen_t menge;
  std::vector<pair<AufEintragBase,AuftragBase::mengen_t> >V_dispo_auftrag;
  for (SelectedFullAufList::const_iterator j=auftraglist.begin();j!=auftraglist.end();++j)
   {
     if(j->Id()==AuftragBase::dispo_auftrag_id) 
      { assert(j->getRestStk()==j->getStueck());
        menge+=j->getStueck();
        V_dispo_auftrag.push_back(pair<AufEintragBase,AuftragBase::mengen_t>(*j,j->getStueck()));
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
