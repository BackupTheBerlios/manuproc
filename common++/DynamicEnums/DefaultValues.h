/* $Id: DefaultValues.h,v 1.6 2002/10/04 13:57:48 thoma Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef _DEFAULT_VALUES_H
#define _DEFAULT_VALUES_H

#include <DynamicEnums/DynamicEnums.h>


/////////////////////////////////////////////////////////////////////////

namespace ManuProC {
 namespace DefaultValues {

// nach dem ersten Durchlauf muß in diesem Verzeichnis 'make recreate' 
// ausgeführt werden. Erst dann werden die Firmenspezifischen Defaultvalues 
// verwendet.
#ifdef MANUPROC_DYNAMICENUMS_CREATED 

  const static int EigeneKundenId=1;
  const static int DefaultKundenId=1;
  const static ManuProC::DynamicEnums::Instanzen::enum_t Instanz= ManuProC::DynamicEnums::Instanzen::Kundenauftraege;
  const static ManuProC::DynamicEnums::Instanzen::enum_t KundenInstanz= ManuProC::DynamicEnums::Instanzen::Kundenauftraege;


#ifdef MABELLA_EXTENSIONS
  const static ManuProC::DynamicEnums::ArtikelTyp::enum_t  ArtikelTyp=ManuProC::DynamicEnums::ArtikelTyp::aufgemachtes_Band;
//  const static ManuProC::DynamicEnums::ArtikelTyp::enum_t  ArtikelTyp=ManuProC::DynamicEnums::aufgemachtes_Band;
  const static ManuProC::DynamicEnums::Prozesse::enum_t  Prozess=ManuProC::DynamicEnums::Prozesse::Verarbeitung;
#elif defined PETIG_EXTENSIONS
  const static ManuProC::DynamicEnums::ArtikelTyp::enum_t  ArtikelTyp=ManuProC::DynamicEnums::ArtikelTyp::Band;
  const static ManuProC::DynamicEnums::Prozesse::enum_t  Prozess=ManuProC::DynamicEnums::Prozesse::Verarbeitung;
#else
  const static ManuProC::DynamicEnums::ArtikelTyp::enum_t  ArtikelTyp = ManuProC::DynamicEnums::ArtikelTyp::Schraubenzieher;
  const static ManuProC::DynamicEnums::Prozesse::enum_t  Prozess=ManuProC::DynamicEnums::Prozesse::Verarbeitung_Irgendetwas;
#endif
#else
  const static int EigeneKundenId=1;
  const static int DefaultKundenId=1;
  const static ManuProC::DynamicEnums::Instanzen::enum_t Instanz= ManuProC::DynamicEnums::Instanzen::enum_t(1);
  const static ManuProC::DynamicEnums::Instanzen::enum_t KundenInstanz= ManuProC::DynamicEnums::Instanzen::enum_t(1);

  const static ManuProC::DynamicEnums::ArtikelTyp::enum_t  ArtikelTyp=ManuProC::DynamicEnums::ArtikelTyp::enum_t(1);
  const static ManuProC::DynamicEnums::Prozesse::enum_t  Prozess=ManuProC::DynamicEnums::Prozesse::enum_t(1);
#endif

 }
}


////////////////////////////////////////////////////////////////////////////

#ifndef PETIG_EXTENSIONS
   #define DPD_LIEFERSCHEINE
   #define EAN_AUSDRUCKEN_IN_PPS
#endif

#ifndef MABELLA_EXTENSIONS
   #define INSTANZEN_VERWALTUNG
#endif


#endif
