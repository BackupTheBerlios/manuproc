// $Id: Materialeigenschaft.hh,v 1.2 2002/07/05 12:35:01 christof Exp $
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
