#include <Aux/Datum.h>
//#include <Artikel/ArtikelBase.h>
#include <Auftrag/AufEintragBase.h>
#include <Aux/ppsInstanz.h>

class ProdInstanz {

      AufEintragBase AEB;

      double artikel_auf_lager(const ArtikelBase& artikel);
      std::string artikel_lagername(const ArtikelBase& artikel);
      void artikel_vormerken();
      int Lieferzeit_in_Tagen(); 

   public:
      ProdInstanz(const AufEintragBase&);

};
