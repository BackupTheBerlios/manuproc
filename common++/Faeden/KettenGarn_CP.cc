#include <Faeden/KettenGarn.h>
#include <Faeden/Faden.hh>
#include <Ketten/ArtikelGang.h>

#warning diese API unterstützt keine Kombinationen!!!

std::vector<KettenGarn_CP> KettenGarn_CP::Load(ArtikelGang const &ag, unsigned laenge)
{  Fadenliste fdl;
   fdl.Load(ag.art,Bindung::GlobaleListe());
   std::vector<KettenGarn_CP> result;

   // ermitteln!
   unsigned new_scheibe=20;
   for (Fadenliste::const_iterator i=fdl.begin(); i!=fdl.end(); ++i)
   {  KettenGarn_CP x;
      if (i->kettscheibe<1) continue;
      x.index=i->kettscheibe;
      x.zeile=i->zeilennummer;
      x.kettenzahl=ag.gaenge;
      x.faeden=i->anzahl;
      x.art=i->material;
      x.laenge=laenge;
      while (x.faeden>i->max_fadenzahl)
      {  unsigned tmp=x.faeden;
         x.faeden=i->max_fadenzahl;
         x.index=new_scheibe++;
         result.push_back(x);
         x.faeden=tmp-i->max_fadenzahl;
      }
      x.index=i->kettscheibe;
      result.push_back(x);
   }
   // sortieren?
   // zusammenfassen?
   return result;
}
