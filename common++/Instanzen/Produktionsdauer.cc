// $Id: Produktionsdauer.cc,v 1.5 2002/10/04 13:57:48 thoma Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2002 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <Instanzen/Produktionsdauer.h>


int Produktionsdauer::Tage(cH_ppsInstanz instanz,const ArtikelBase &artikel)
{
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
  switch(instanz->Id())
   {
     case ppsInstanzID::Faerberei  : // zum Erweitern
     case ppsInstanzID::Druckerei  : 
     case ppsInstanzID::Weberei    : 
     case ppsInstanzID::Schaererei : 
     case ppsInstanzID::Rollerei   : 
     case ppsInstanzID::Spritzgiesserei : 
     case ppsInstanzID::_Garn__Einkauf : 
     default : instanz->ProduktionsDauer();
   }
#endif
}
