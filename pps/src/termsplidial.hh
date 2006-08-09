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

#ifndef _TERMSPLIDIAL_HH
#  include "termsplidial_glade.hh"
#  define _TERMSPLIDIAL_HH

#include<Misc/Datum.h>

class auftrag_bearbeiten;

class termsplidial : public termsplidial_glade
{	auftrag_bearbeiten *auftragbearbeiten;
        friend class termsplidial_glade;
        void on_okbutton_clicked();
        void on_cancbutton_clicked();

public:
	termsplidial(auftrag_bearbeiten *ab, const ManuProC::Datum &val);
	~termsplidial();
        ManuProC::Datum getLiefDatum() 
		{ // guint tag,monat,jahr;
		  // termin_neu->get_date(jahr,monat,tag);
  		  return termin_neu->get_value();
		  // return ManuProC::Datum(tag,monat+1,jahr);
		}

        int getMenge() { return menge_neu->get_value_as_int();}
};
#endif
