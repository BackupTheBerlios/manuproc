//  DynamicEnums.h  created 16.12.2002 13:02:31 +1

#ifndef MANUPROC_DYNAMICENUMS_H
#define MANUPROC_DYNAMICENUMS_H

#define MANUPROC_DYNAMICENUMS_CREATED

namespace ManuProC {
 namespace DynamicEnums {
  namespace ArtikelTyp {
   enum enum_t {
    None=-1,Jumborollen=0,Band=3,Garn=4,Spulen=5,Kartonagen=6,Sonstiges=7,Lohn=8,Farben___Pigmente=9,Etiketten=10,Veraltet_Fehler=11,
   };
  }

  namespace ArtikelTyp_Attributes {
   enum enum_t {
LagerArtikel=0,MitEAN=1,
   };
  }

  namespace Einheiten {
   enum enum_t {
    None=-1, St_=0,m=1,kg=2,Fd_km=3,h=4,
   };
  }

  namespace Prozesse {
   enum enum_t {
    None=0,Drucken=1,Faerben_410=2,Faerben_775=3,Faerben_795=4,Faerben_458=5,Faerben_760=6,Faerben_340=7,Faerben_247=8,Faerben_342=9,Faerben_786=10,Faerben_430=11,Rollen_25m=12,Rollen_20m=13,Rollen_2m=14,Rollen_5mDun=15,Rollen_Jumbo=16,Rollen_5m=17,Rollen_50m=18,Rollen_7m=19,Rollen_15m=20,Rollen_25mDu=21,Rollen_3m=22,Rollen_7mDun=23,Rollen_5mJoy=24,Rollen_7mJoy=25,Rollen_10m=26,Rollen_10y=27,Rollen_100y=28,Rollen_100m=29,Faerben=30,Verarbeitung=31,Rollen_2_5m=32,Rollen_15mDu=33,Rollen_2mDun=34,Rollen_2mB2k=35,Rollen_3mB2k=36,Rollen_2mSon=37,Aequivalenz=38,Rollen_50mFo=39,Rollen_25mKnorr=40,Rollen_3mDun=41,Weben=42,Schaeren=43,Verpacken=44,Giessen=45,Rollen=46,
   };
  }

  namespace Waehrung {
   enum enum_t {
    None=-1,DM=1,EUR=2,USD=3,PND=4,
   };
  }

  namespace Instanzen {
   enum enum_t {
    None=-1,Kundenauftraege=1,Faerberei=2,Druckerei=3,Weberei=4,Schaererei=5,_Garn__Einkauf=6,Spritzgiesserei=7,Bandlager=8,Rohwarenlager=9,Rollerei=10,
   };
  }

  namespace Kundengruppen {
   enum enum_t {
    None=-1,Kunden=1,Lieferanten=2,Personen=3,Privat=4,Lieferadresse=5,Rechnungsadresse=6,Auftragsadresse=7,
   };
  }

 }
}
#endif // MANUPROC_DYNAMICENUMS_H
