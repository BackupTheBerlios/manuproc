// $Id: DynEnums_default.h,v 1.3 2002/05/06 13:41:23 christof Exp $

#ifndef MANUPROC_DYNAMICENUMS_H
#define MANUPROC_DYNAMICENUMS_H

namespace ManuProC {
 namespace DynamicEnums {
  namespace ArtikelTyp {
   enum enum_t {
   };
  }

  namespace Einheiten {
   enum enum_t {
    St_=0
   };
  }

  namespace Prozesse {
   enum enum_t {
    None=0, Verarbeitung=1
   };
  }

  namespace Waehrung {
   enum enum_t {
    EUR=1
   };
  }

  namespace Instanzen {
   enum enum_t {
    None=0,Kundenauftraege=1
   };
  }

 }
}
#endif // MANUPROC_DYNAMICENUMS_H
