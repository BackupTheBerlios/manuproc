#include <Ketten/Maschtyp.h>
#include<Artikel/ArtikelBase.h>

class Maschinen_Geschwindigkeit
{
      double speed_d; /* Meter/Tag */
      int bekannt;
   public:
      Maschinen_Geschwindigkeit():speed_d(100),bekannt(4){};
      Maschinen_Geschwindigkeit(ArtikelBase artikel, const string& maschtyp="");
      double Speed_m_Tag() const {return speed_d;}  /* Meter pro Tag */
      int Bekannt() const {return bekannt;}
         // 1 = Artikel+Maschine bekannt
         // 2 = Artikel auf anderer Maschine
         // 3 = Artikel noch nie gewebt 
         // 4 = Artikel noch nie gewebt UND neu Maschine
};
