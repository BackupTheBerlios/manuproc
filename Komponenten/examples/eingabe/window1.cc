// $Id: window1.cc,v 1.1 2001/04/23 08:36:50 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
// generated 1999/11/17 16:41:2 MET by christof@petig.petig.de
// using glademm V0.5.4a
//
// newer (non customized) versions of this file go to window1.cc_glade

// This file is for your program, I won't touch it again!

#include "window1.hh"
#include <gtk--/main.h>

void window1::rauf()
{   custom1->set_size(custom1->size()+1);
}

void window1::runter()
{   custom1->set_size(custom1->size()-1);
}

void window1::schliessen()
{   Gtk::Main::instance()->quit();
}

gint window1::schliessen2(GdkEventAny*a)
{   schliessen();
    return false;
}

window1::window1()
{  entry1->activate.connect(custom1->grab_focus.slot());
   custom1->activate.connect(entry2->grab_focus.slot());
   custom1->set_label(0,"Art");
   custom1->set_label(1,"Br");
   custom1->set_label(2,"Fb");
   custom1->set_label(3,"Aufm");
   custom1->set_label(4,"EAN");
}
