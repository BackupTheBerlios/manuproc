/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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
// using guimm V0.5.9a
//
// newer (non customized) versions of this file go to PreisBox.hh_gui

// you might replace
//    class Foo : public Foo_gui { ... };
// by
//    typedef Foo_gui Foo;
// if you didn't make any modifications to the widget

#ifndef _INTSTRINGBOX_HH
#  include "IntStringBox_gui.hh"
#  define _INTSTRINGBOX_HH

#include <Aux/Transaction.h>

class IntStringBox : public IntStringBox_gui
{	
  friend class IntStringBox_gui;
        void int_activate();
        void int_search(gboolean *_continue, GtkSCContext newsearch);
        void string_activate();
        void string_search(gboolean *_continue, GtkSCContext newsearch);
        void stringz_activate();
        void stringz_search(gboolean *_continue, GtkSCContext newsearch);
        static gint try_grab_focus(GtkWidget *w,gpointer gp);

	int id;
	Transaction tr,tr2,tr3;
   bool einschraenkung_b;
protected:
   std::string _tabelle_,_string_,_stringz_,_int_;

public:
	IntStringBox();
   void setLabel(const std::string &nr,const std::string &name,const std::string& namez="");
   void setLabel(long int nr);

	int get_value() const { return id; }
	void set_value(int i,const std::string &s,const std::string &sz="") ;
	virtual void set_value(int i)=0 ;
   void hide_int(bool b);
   void show_stringz(bool b);
   void stringz_set_sensitive(bool b);
	void reset();
   void clear() {reset();}

//   void Einschraenken_b(bool an);
	
	SigC::Signal0<void> activate;
};
#endif
