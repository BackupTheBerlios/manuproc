#include <Artikel/ArtikelBase.h>
#include <Aux/fixedpoint.h>

class Materialeigenschaft
{
      double g_10km;
      int pfprokg;
      fixedpoint<2> schaerlohn;
   public: 
      Materialeigenschaft(const ArtikelBase a);
      double Gewicht_g_10km() const {return g_10km;} // entspricht dTex
      int Preis_pf_kg() const {return pfprokg;}
      fixedpoint<2> Schaerlohn_pf_faden() const {return schaerlohn;}
};
