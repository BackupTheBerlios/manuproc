#include <Aux/Datum.h>
#include <Artikel/ArtikelBase.h>
#include <Aux/ppsInstanz.h>

class ProdInstanz {

      int auftragid;
      ppsInstanz::ppsInstId instanz;
      Petig::Datum bestell_datum, liefer_datum;
      ArtikelBase artikel;
      double menge_vorraetig,menge_bestellt;
      std::string status;
      std::string einheit;

      void artikel_auf_lager();
      std::string artikel_lagername();
      void artikel_vormerken();
      int Lieferzeit_in_Tagen(); 

   public:
//      ProdInstanz(Petig::Datum bd, ArtikelBase a);
      ProdInstanz(int _auftragid,ppsInstanz::ppsInstId _instanz,
            Petig::Datum _bestelldatum, ArtikelBase _artikel, 
         double _menge,std::string _status);

      double Menge_im_Lager() const {return menge_vorraetig;}
      std::string Einheit() const {return einheit;}
      Petig::Datum Lieferdatum() const {return liefer_datum;}

};
