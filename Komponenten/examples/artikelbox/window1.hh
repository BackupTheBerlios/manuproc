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

// generated 1999/11/17 16:24:15 MET by christof@petig.petig.de
// using glademm V0.5.4a
//
// newer (non customized) versions of this file go to window1.hh_glade

// you might replace
//    class Foo : public Foo_Glade { ... };
// by
//    typedef Foo_Glade Foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOW1_HH
#  include "window1_glade.hh"
#  define _WINDOW1_HH
#include <datewin.h>
#include <ArtikelBox.hh>
#include <Artikel/ArtikelBase.h>
#include <ProzessBox.hh>
#include <ExtBezSchema/ExtBezSchemaHandle.h>

class window1 : public window1_glade
{   
        friend class window1_glade;
        void on_Beenden_activate();
        void on_show_clicked();
	void on_show_complete();
	void on_reset();
	void andererKunde();
	void menu_cb(gpointer data);

   void on_activate_wwaehrung();
   void on_activate_wpreis();
public:
	window1();
};
#endif
