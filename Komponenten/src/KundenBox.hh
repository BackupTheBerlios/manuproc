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

// generated 2000/8/28 14:42:53 CEST by christof@puck.petig.de
// using glademm V0.5.9a
//
// newer (non customized) versions of this file go to KundenBox.hh_glade

// you might replace
//    class Foo : public Foo_glade { ... };
// by
//    typedef Foo_glade Foo;
// if you didn't make any modifications to the widget

#ifndef _KUNDENBOX_HH
#  include "KundenBox_glade.hh"
#  define _KUNDENBOX_HH

#include <Kunde/Kunde.h>
#include <Aux/Transaction.h>

class KundenBox : public KundenBox_glade
{	friend class KundenBox_glade;
        void Kundennr_activate();
        void Kundennr_search(gboolean *_continue, GtkSCContext newsearch);
        void Name_activate();
        void Name_search(gboolean *_continue, GtkSCContext newsearch);
        static gint try_grab_focus(GtkWidget *w,gpointer gp);

	Kunde::ID id;
	Transaction tr,tr2;
   bool einschraenkung_b;

public:
	KundenBox();
   void setLabel(std::string nr,std::string name);

	Kunde::ID get_value() const 
	{  return id; }
	void set_value(Kunde::ID i) {  set_value(cH_Kunde(i)); }
	void set_value(const cH_Kunde &k);
	void reset();

   void Einschraenken_b(bool an);
	
	SigC::Signal0<void> activate;
};
#endif
