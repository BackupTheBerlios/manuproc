#include "Lager_Vormerkungen.h"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBaum.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintragZu.h>
//#include "maschinen_geschwindigkeit.hh"
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>


#include <Artikel/ArtikelBezeichnung.h> // debug

Lager_Vormerkungen::Lager_Vormerkungen(const AufEintrag& ae)
: AufEintrag(ae)
{
  assert ( Instanz()->LagerFuer()!=ppsInstanzID::None);
}

void Lager_Vormerkungen::vormerken_oder_bestellen()
{
  AuftragBase::mengen_t menge_im_lager=artikel_auf_lager(ArtId());
//cout << "Von Artikel "<<cH_ArtikelBezeichnung(ArtId())->Bezeichnung()<<
//" ist "<<menge_im_lager<<" im Lager vorhanden\n";
//cout <<"bestellt sind "<<getRestStk()<<' '<<getStueck()<<'\n';
  if (getRestStk() <= menge_im_lager) // Artikel vormerken
    {
//cout << "Lager reicht aus für"<<getRestStk()<<'\n';
      artikel_vormerken(getRestStk());
    }
  else  
    { 
//cout <<"Lager reicht nicht aus\n";
      if(menge_im_lager<0) menge_im_lager=0;
      if(menge_im_lager) artikel_vormerken(menge_im_lager);
      //'artikel_vormerken' reduziert die Menge des AufEintrags 
      //AuftragBase::mengen_t fehlende_menge = getStueck()-menge_im_lager;

//cout << "tryUpdate Menge= "<<getStueck()<<' '<<getRestStk()<<' '<<menge_im_lager<<' '<<'\n';
//cout << "Mange die fehlt: "<<fehlende_menge<<'\n';
      AuftragBase AB(Instanz()->LagerFuer(),AuftragBase::ungeplante_id);
      AB.tryUpdateEntry(getRestStk(),getLieferdatum(),
            ArtId(),getEntryStatus(),*this);
    }
}



void Lager_Vormerkungen::artikel_vormerken(AuftragBase::mengen_t menge)
{
  if(menge==AuftragBase::mengen_t(0)) return ;
  Planen(menge,AuftragBase(Instanz()->Id(),LagerAuftragsId));
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

void Lager_Vormerkungen::reduziere_ungeplant(AuftragBase::mengen_t menge)
{
  assert(menge>0);
  std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_list_ungeplant(); 
  assert(L.begin()==++L.end()); // Lager kann nur bei EINER Instanz geplant sein
  AufEintrag(L.begin()->AEB).updateStkDiff__(-menge);
  AuftragBase::mengen_t mt=AufEintragZu(*this).setMengeDiff__(L.begin()->AEB,-menge);
  assert(mt==-menge);
}


void Lager_Vormerkungen::freigegeben_menge_neu_verplanen(cH_ppsInstanz instanz,
               const ArtikelBase& artikel,AuftragBase::mengen_t menge)
{
//cout << "Lager für ARtikel: "<<cH_ArtikelBezeichnung(artikel)->Bezeichnung()
//<<"\tMenge: "<<menge<<'\n';
  assert(instanz->LagerInstanz());
  assert(menge>=0);
  if(menge==AuftragBase::mengen_t(0)) return;
  SQLFullAuftragSelector sel(SQLFullAuftragSelector::sel_Artikel_Planung(instanz->Id(),artikel,false)); // false=ungeplant
  SelectedFullAufList auftraglist=SelectedFullAufList(sel);
  for (SelectedFullAufList::const_iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t fehlende_menge = i->getRestStk();
     if(fehlende_menge==AuftragBase::mengen_t(0)) continue;
     if(fehlende_menge>=menge)
      {
//cout << "Freigegeben und KOMPLETT neu vorgemerkt für: "<<AufEintragBase(*i)<<' '<<fehlende_menge<<' '<< menge <<'\n';;
        Lager_Vormerkungen(*i).artikel_vormerken(menge);
        Lager_Vormerkungen(*i).reduziere_ungeplant(menge);
        return;
      }
     else
      {
//cout << "Freigegeben und TEILWEISE neu vorgemerkt für: "<<AufEintragBase(*i)<<' '<< menge << ' '<<fehlende_menge <<'\n';;
        Lager_Vormerkungen(*i).artikel_vormerken(fehlende_menge);
        Lager_Vormerkungen(*i).reduziere_ungeplant(fehlende_menge);
        menge-=fehlende_menge;
      }
     assert(menge>=AuftragBase::mengen_t(0));
     if(menge==AuftragBase::mengen_t(0)) return;
   }
//if(auftraglist.empty()) cout << "Kein Auftrag zum neuverplanen\n";
}

