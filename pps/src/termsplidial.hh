/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// generated 2000/12/5 22:26:59 MET by jacek@mimi.
// using glademm V0.5.10
//
// newer (non customized) versions of this file go to termsplidial.hh_glade

// you might replace
//    class Foo : public Foo_glade { ... };
// by
//    typedef Foo_glade Foo;
// if you didn't make any modifications to the widget

#ifndef _TERMSPLIDIAL_HH
#  include "termsplidial_glade.hh"
#  define _TERMSPLIDIAL_HH

#include<Aux/Datum.h>

class termsplidial : public termsplidial_glade
{   
        
        friend class termsplidial_glade;
        void on_okbutton_clicked();
        void on_cancbutton_clicked();

public:

        ManuProC::Datum getLiefDatum() 
		{ guint tag,monat,jahr;
		  termin_neu->get_date(&jahr,&monat,&tag);
  		  return ManuProC::Datum(tag,monat+1,jahr);
		}

        int getMenge() { return menge_neu->get_value_as_int();}
};
#endif
