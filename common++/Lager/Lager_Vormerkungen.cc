#include "Lager_Vormerkungen.hh"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBaum.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AuftragsEntryZuordnung.h>
//#include "maschinen_geschwindigkeit.hh"
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>


#include <Artikel/ArtikelBezeichnung.h> // debug

Lager_Vormerkungen::Lager_Vormerkungen(const AufEintrag& ae)
: AufEintrag(ae)
{
  assert ( Instanz()->LagerFuer()!=ppsInstanz::None);
}

void Lager_Vormerkungen::vormerken_oder_bestellen()
{
  AuftragBase::mengen_t menge_im_lager=artikel_auf_lager(ArtId());
//cout << "Von Artikel "<<cH_ArtikelBezeichnung(ArtId())->Bezeichnung()<<
//" ist "<<menge_im_lager<<" im Lager vorhanden\n";
//cout <<"bestellt sind "<<getRestStk()<<' '<<getStueck()<<'\n';
  if (getRestStk() <= menge_im_lager) // Artikel vormerken
    {
//cout << "Lager reicht aus\n";
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
      AuftragBase AB(Instanz()->LagerFuer());
      //AB.tryUpdateEntry(fehlende_menge,getLieferdatum(),
      AB.tryUpdateEntry(getRestStk(),getLieferdatum(),
            ArtId(),getEntryStatus(),*this,ZNr());
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


void Lager_Vormerkungen::freigegeben_menge_neu_verplanen(cH_ppsInstanz instanz,
               const ArtikelBase& artikel,AuftragBase::mengen_t menge)
{
  assert(instanz->LagerInstanz());
  assert(menge>=0);
  if(menge==AuftragBase::mengen_t(0)) return;
  SQLFullAuftragSelector sel(SQLFullAuftragSelector::sel_Artikel_Planung(instanz->Id(),artikel,false)); // false=ungeplant
  SelectedFullAufList auftraglist=SelectedFullAufList(sel);
  for (SelectedFullAufList::const_iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t fehlende_menge = i->getRestStk();
     if(fehlende_menge>=menge)
      {
cout << "Freigegeben und KOMPLETT neu vorgemerkt: "<< menge <<'\n';;
        Lager_Vormerkungen(*i).artikel_vormerken(menge);
        return;
      }
     else
      {
cout << "Freigegeben und TEILWEISE neu vorgemerkt: "<< menge << ' '<<fehlende_menge <<'\n';;
        Lager_Vormerkungen(*i).artikel_vormerken(fehlende_menge);
        menge-=fehlende_menge;
      }
   }
}

