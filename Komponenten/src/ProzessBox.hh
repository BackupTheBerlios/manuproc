/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// $Id: ProzessBox.hh,v 1.1 2001/04/23 08:36:50 christof Exp $

#ifndef PROZESS_BOX_HH
#define PROZESS_BOX_HH
#include <SearchCombo.h>
#include <Artikel/Prozess.h>
#include <Aux/Transaction.h>

// this should really work with the Prozess-cache?

class ProzessBox : public Gtk::HBox // we can't derive from SearchCombo since
			// we redefine activate !
{	Gtk::SearchCombo scombo;
	Prozess::ID prozessid;
	bool allow_empty:1;
	Transaction tr;

        void search_prozess(gboolean *cont, GtkSCContext newsearch);
        void combo_activated();
public:
	ProzessBox();
	const cH_Prozess get_value() const
	{  return cH_Prozess(prozessid); }
	void set_value(const cH_Prozess &p);
	void reset();
	void set_allow_empty(bool on)
	{  allow_empty=on; }
	
	SigC::Signal0<void> activate;
};
#endif
