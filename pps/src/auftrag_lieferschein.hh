/*  pps: ManuProC's ProductionPlanningSystem
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

// generated 2001/1/26 10:08:28 CET by jacek@mimi.(none)
// using glademm V0.5_11a
//
// newer (non customized) versions of this file go to auftrag_lieferschein.hh_glade

// you might replace
//    class Foo : public Foo_glade { ... };
// by
//    typedef Foo_glade Foo;
// if you didn't make any modifications to the widget

#ifndef _AUFTRAG_LIEFERSCHEIN_HH
#  include "auftrag_lieferschein_glade.hh"
#  define _AUFTRAG_LIEFERSCHEIN_HH
#include "glademm_support.hh"

#include<Lieferschein/LieferscheinVoll.h>
#include<tclistrowdata.h>
#include<Auftrag/AufEintragBase2.h>

class auftrag_lieferschein : public auftrag_lieferschein_glade
{   
        
        friend class auftrag_lieferschein_glade;
        void on_liefer_close();
        void on_liefer_neu();
        void on_lief_save();
        void on_lief_preview();
        void on_lief_print();
        void on_liefnr_activate();
	void on_liefnr_search(int *_continue, GtkSCContext newsearch);
        void on_lieferkunde_activate();
        void on_liefdate_activate();
        void on_liefermenge_activate();
        void on_newlieferentry_ok();
        void on_newlieferentryall_ok();
        void on_selectrow_offauf(int row, int col, GdkEvent* b);
        void on_unselectrow_offauf(int row, int col, GdkEvent* b);
        void on_Palette_activate();
        void on_artikelbox_activate();

	void on_selectrow_lieferschein(int row, int col, GdkEvent* b);
	void on_unselectrow_lieferschein(int row, int col, GdkEvent* b);
        void liefzeile_delete();

	AufEintragBase2 auftragentry;
	ArtikelBase artikel;
	TCListRowData *selectedrow;
	
	void clear_offauf();
	
public:
	auftrag_lieferschein();
	Kunde::ID getKdNr() { return liefer_kunde->get_value();}
};


#endif

