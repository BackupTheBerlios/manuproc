#include "ProdLager.hh"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBaum.h>
#include <Auftrag/AuftragBase.h>
//#include "maschinen_geschwindigkeit.hh"

#include <Artikel/ArtikelBezeichnung.h> // debug

ProdLager::ProdLager(const AufEintragBase& aeb)
: AEB(aeb)
{
  check_artikel_ist_lagerartikel();
}

void ProdLager::vormerken_oder_bestellen()
{
  AuftragBase::mengen_t menge_im_lager=artikel_auf_lager(AEB.Artikel());
cout << "\nProdLager: ";
cout << "Von Artikel "<<cH_ArtikelBezeichnung(AEB.ArtId())->Bezeichnung()<<
" ist "<<menge_im_lager<<" im Lager vorhanden\n";
cout <<"bestellt sind "<<AEB.getRestStk()<<'\n';

  if (AEB.getRestStk() <= menge_im_lager) // Artikel vormerken
    {
cout << "Lager reicht aus\n";
      artikel_vormerken(AEB.getRestStk());
    }
  else  
    { 
cout <<"Lager reicht nicht aus\n";
      if(menge_im_lager) artikel_vormerken(menge_im_lager);
      AuftragBase::mengen_t fehlende_menge=AEB.getRestStk()-menge_im_lager;
      cH_ppsInstanz bestellen_bei(ArtikelStamm(AEB.ArtId()).BestellenBei()->LagerFuer());
      AuftragBase AB(bestellen_bei);
cout << "tryUpdate Menge= "<<fehlende_menge<<'\n';
      AB.tryUpdateEntry(fehlende_menge,AEB.getLieferdatum(),
            AEB.ArtId(),AEB.getEntryStatus(),AEB,AEB.ZNr());
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

std::string ProdLager::artikel_lagername(const ArtikelBase& artikel)
{
  int instanz = ArtikelStamm(artikel).BestellenBei()->Id();
  std::string lagername;
  switch (instanz) {
      case ppsInstanz::INST_BANDLAGER : { lagername = "rohjumbo" ;  break;}
      case ppsInstanz::INST_ROHLAGER : { lagername = "rl_inhalt" ; break;}
   }
  return lagername;
}
 
void ProdLager::check_artikel_ist_lagerartikel()
{
  int instanz = ArtikelStamm(AEB.ArtId()).BestellenBei()->Id();
  if(instanz != ppsInstanz::INST_BANDLAGER &&
     instanz != ppsInstanz::INST_ROHLAGER )
    assert(!"ProdLager nur für Instanz INST_ROHLAGER und INST_ROHLAGER implementiert");
}
