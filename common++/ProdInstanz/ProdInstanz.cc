#include "ProdInstanz.hh"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBaum.h>
#include "maschinen_geschwindigkeit.hh"

ProdInstanz::ProdInstanz(const AufEintragBase& aeb)
: AEB(aeb)
{
  double menge_im_lager artikel_auf_lager(aeb.Artikel());
  if (aeb.getRestStk() <= menge_im_lager) // Artikel vormerken
    {
      artikel_vormerken();
    }
  else 
    { 
      int instanz = ArtikelStamm(artikel).BestellenBei()->Id();
#warning TODO
      switch (instanz) {
        case 8 : {  ;  break;}
        case 9 : {  ; break;}
        assert(!"ProdInstanz nur für Instanz 8 und 9 (=Lager) implementiert");
         }

/*
      ArtikelBaum AB(artikel);
      for (ArtikelBaum::const_iterator ab=AB.begin();ab!=AB.end();++ab)
       {
         ArtikelBase AB(ab->rohartikel);
         ArtikelStamm AS(AB);
         double menge = menge_bestellt*ab->menge;
         ppsInstanz::ID bestellen_bei=AS.BestellenBei()->Id();
         ProdInstanz PI(auftragid,bestellen_bei,bestell_datum,AB,menge,status); 
       }
*/
    }
}


int ProdInstanz::Lieferzeit_in_Tagen()
{
  double dauer;
  if (instanz==ppsInstanz::INST_WEB)
   {
     Maschinen_Geschwindigkeit MG(artikel);
     dauer = menge_bestellt/MG.Speed_m_Tag();
   }
  return dauer;
}

std::string ProdInstanz::artikel_lagername(const ArtikelBase& artikel)
{
  int instanz = ArtikelStamm(artikel).BestellenBei()->Id();
  std::string lagername;
  switch (instanz) {
      case 8 : { lagername = "rohjumbo" ;  break;}
      case 9 : { lagername = "rl_inhalt" ; break;}
      default : lagername =""; einheit=""; 
         assert(!"ProdInstanz nur für Instanz 8 und 9 (=Lager) implementiert");
   }
  return lagername;
}
 
