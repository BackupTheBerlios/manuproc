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

// generated 2000/11/22 20:43:27 MET by jacek@mimi.
// using glademm V0.5.10
//
// newer (non customized) versions of this file go to auftrag_main.hh_glade

// you might replace
//    class Foo : public Foo_glade { ... };
// by
//    typedef Foo_glade Foo;
// if you didn't make any modifications to the widget

#ifndef _AUFTRAG_MAIN_HH
#  include "auftrag_main_glade.hh"
#  define _AUFTRAG_MAIN_HH
#include "glademm_support.hh"

#include<Artikel/Prozess.h>
#include<vector>

class auftrag_main : public auftrag_main_glade
{   
 bool interne_namen;
 int selectedaufzeile;
 int selectedaufid; 
 int showdeep;
 MyRow *selectedmyrow; 
 vector<cH_Prozess> prozlist;
        
        friend class auftrag_main_glade;
        void on_beenden_activate();
        void on_erfassen_activate();
        void on_neuladen_activate();
        void on_main_drucken_activate();
        void on_abschreiben_activate();
	void on_rechnung_activate();
        void on_main_kndbez_activate();
        void on_main_intbez_activate();
        void on_mainprint_button_clicked();
        void on_main_showtreebutton_clicked();
        void on_main_defaultattrbutton_clicked();
	void on_prozlistscombo_search(int *cont, GtkSCContext newsearch);
        void on_prozlistscombo_activate();

 void onRowSelect(int row, int col, GdkEvent* b);
 void onRowUnselect(int row, int col, GdkEvent* b);
 void showtree();
 void fillStamm(int *cont, GtkSCContext newsearch);

public:

 bool interneNamen() const { return interne_namen; }
 auftrag_main();

};
#endif
