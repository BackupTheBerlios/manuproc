// $Id: maschinen_geschwindigkeit.hh,v 1.2 2002/07/05 12:35:01 christof Exp $
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
