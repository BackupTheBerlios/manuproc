//  DynamicEnums.h  created 8.7.2003 8:26:24 +2

#ifndef MANUPROC_DYNAMICENUMS_H
#define MANUPROC_DYNAMICENUMS_H

#define MANUPROC_DYNAMICENUMS_CREATED

namespace ManuProC {
 namespace DynamicEnums {
  namespace ArtikelTyp {
   enum enum_t {
    None=-1,Rohware=1,Griffe=2,Schaefte=3,Schraubenzieher=4,Verpackungen=5,Sortimente=6,
   };
  }

  namespace Einheiten {
   enum enum_t {
    None=-1, St_=0,Gew__kg=1,
   };
  }

  namespace Prozesse {
   enum enum_t {
    None=0,Aequivalenz_Gleichheit=2,Schmelzen=10,Giessen=11,Verpacken=12,Zusammenfuegen=13,Verarbeitung_Irgendetwas=31,
   };
  }

  namespace Waehrung {
   enum enum_t {
   None=-1,EUR=1,DM=2,USD=3,PND=4,
   };
  }

  namespace Instanzen {
   enum enum_t {
    None=-1,Kundenauftraege=1,Schraubenzieherlager=5,Werkstatt=10,Packerei=11,Giesserei=20,Schmelze=21,Rohwarenlager=30,Einkauf=35,
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
