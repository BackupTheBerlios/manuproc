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

#ifndef _RL_LAGERPLATZ_HH_
#define _RL_LAGERPLATZ_HH_
#include <string>
#include <Aux/ppsInstanz.h>

class LagerPlatz
{
     const static int INOPOS=-1;
     enum e_darstellung{eString,eInt};
     
     ppsInstanz::ppsInstId instanz;
     e_darstellung darstellung;
     std::string s_position;
     int i_position;

     void DarstellungSetzen();
     bool eString_valid();
     bool eInt_valid();

  public:
     LagerPlatz() : i_position(0){}
     LagerPlatz(ppsInstanz::ppsInstId i, std::string p)
         : instanz(i), s_position(p) ,i_position(INOPOS) 
            {DarstellungSetzen();}
     LagerPlatz(ppsInstanz::ppsInstId i, int p)
         : instanz(i), s_position(""), i_position(p)
            {DarstellungSetzen();}

     std::string SPosition() const 
         {if(darstellung==eString) return s_position;
          else assert(!"Falsche Position für diese Darstellung.");
          abort();}
     int  IPosition() const 
         {if(darstellung==eInt) return i_position;
          else  assert(!"Falsche Position für diese Darstellung."); 
          abort();}

     bool valid()
         {if(darstellung==eString) return eString_valid();
          else if(darstellung==eInt) return eInt_valid(); 
          abort();
         }

     bool operator<(const LagerPlatz& b) const
      {
        if(darstellung==eInt) return IPosition()<b.IPosition();
        else return SPosition()<b.SPosition();
        abort();
      }

};

#endif
