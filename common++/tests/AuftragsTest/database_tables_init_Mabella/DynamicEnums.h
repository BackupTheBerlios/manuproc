//  DynamicEnums.h  created 26.11.2003 16:56:20 +1

#ifndef MANUPROC_DYNAMICENUMS_H
#define MANUPROC_DYNAMICENUMS_H

#define MANUPROC_DYNAMICENUMS_CREATED

namespace ManuProC {
 namespace DynamicEnums {
  namespace ArtikelTyp {
   enum enum_t {
    gewebtes_Band=0,gefaerbtes_Band=1,bedrucktes_Band=2,aufgemachtes_Band=3,Garn_per_kg=4,Spulen=5,Kartonagen=6,Sonstiges=7,Lohn=8,Farben___Pigmente=9,Etiketten=10,Garn_per_km=11,Musterkarten=12,
   };
  }

  namespace Einheiten {
   enum enum_t {
    None=-1, St_=0,m=1,kg=2,Fd_km=3,DM=4,Euro=5,Pfund=6,
   };
  }

  namespace Prozesse {
   enum enum_t {
    None=0,Drucken=1,Faerben_410=2,Faerben_775=3,Faerben_795=4,Faerben_458=5,Faerben_760=6,Faerben_340=7,Faerben_247=8,Faerben_342=9,Faerben_786=10,Faerben_430=11,Rollen_25m=12,Rollen_20m=13,Rollen_2m=14,Rollen_5mDun=15,Rollen_Jumbo=16,Rollen_5m=17,Rollen_50m=18,Rollen_7m=19,Rollen_15m=20,Rollen_25mDu=21,Rollen_3m=22,Rollen_7mDun=23,Rollen_5mJoy=24,Rollen_7mJoy=25,Rollen_10m=26,Rollen_10y=27,Rollen_100y=28,Rollen_100m=29,Faerben=30,Verarbeitung=31,Rollen_2_5m=32,Rollen_15mDu=33,Rollen_6m=34,Rollen_2m_I_=35,Rollen_2m_II_=36,Rollen_2m_III_=37,Rollen_2m_IV_=38,Rollen_3m_IV_=39,
   };
  }

  namespace Waehrung {
   enum enum_t {
   None=-1,EUR=1,DM=2,USD=3,PND=4,
   };
  }

  namespace Instanzen {
   enum enum_t {
    None=-1,Kundenauftraege=1,Fertigwarenlager=2,Einkauf=3,
   };
  }

  namespace Kundengruppen {
   enum enum_t {
    None=-1,Kunden=1,Lieferanten=2,Verkaeufer=3,Personen=4,Retif=5,Sonstige=6,Lieferadresse=7,Rechnungsadresse=8,Auftragsadresse=9,Firma=10,Institution=11,Personal=12,
   };
  }

 }
}
#endif // MANUPROC_DYNAMICENUMS_H
