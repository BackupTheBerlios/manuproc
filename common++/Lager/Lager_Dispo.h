/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <Aux/ppsInstanz.h>
#include <Artikel/ArtikelBase.h>
#include <Aux/fixedpoint.h>

class Lager_Dispo 
{
  public:
      typedef fixedpoint<0> mengen_t;
  private:
      cH_ppsInstanz instanz;
      ArtikelBase artikel;
      mengen_t max_menge,min_menge,optimale_menge;
      void setMengen();

   public:
      Lager_Dispo(cH_ppsInstanz i,ArtikelBase a) ;

      void setMaxMenge(mengen_t m) {max_menge=m; setMengen();}
      void setMinMenge(mengen_t m) {min_menge=m; setMengen();}
      void setOptimaleMenge(mengen_t m) {optimale_menge=m; setMengen();}

      cH_ppsInstanz Instanz() const {return instanz;}
      ArtikelBase Artikel() const {return artikel;}
      mengen_t MaxMenge() const {return max_menge;}    
      mengen_t MinMenge() const {return min_menge;}    
      mengen_t OptimaleMenge() const {return optimale_menge;}    
};
