#include "ProdInstanz.hh"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBaum.h>
#include "maschinen_geschwindigkeit.hh"

/*
ProdInstanz::ProdInstanz(Petig::Datum bd, ArtikelBase a)
: bestell_datum(bd), artikel(a), menge_bestellt(0)
{
}
*/

ProdInstanz::ProdInstanz(int _auftragid, ppsInstanz::ppsInstId _instanz,
   Petig::Datum _bestelldatum, ArtikelBase _artikel, double _menge,
   std::string _status)
: auftragid(_auftragid), instanz(_instanz), bestell_datum(_bestelldatum), 
   artikel(_artikel), menge_vorraetig(0), menge_bestellt(_menge),
   status(_status)
{
  artikel_auf_lager();
  if (menge_bestellt >= Menge_im_Lager()) // Artikel vormerken
    {
      artikel_vormerken();
    }
  else // Rohware(n) bestellen
    { 
      ArtikelBaum AB(artikel);
      for (ArtikelBaum::const_iterator ab=AB.begin();ab!=AB.end();++ab)
       {
         ArtikelBase AB(ab->rohartikel);
         ArtikelStamm AS(AB);
         double menge = menge_bestellt*ab->menge;
         ppsInstanz::ppsInstId bestellen_bei=AS.BestellenBei()->Id();
         ProdInstanz PI(auftragid,bestellen_bei,bestell_datum,AB,menge,status); 
       }
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
