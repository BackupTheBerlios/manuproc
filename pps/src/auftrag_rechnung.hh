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

#ifndef _AUFTRAG_RECHNUNG_HH
#  include "auftrag_rechnung_glade.hh"
#  define _AUFTRAG_RECHNUNG_HH

#include<Instanzen/ppsInstanz.h>
#include<Lieferschein/Rechnung.h>
#include<Lieferschein/Lieferschein.h>

class auftrag_rechnung : public auftrag_rechnung_glade
{   
        cH_ppsInstanz instanz;
        friend class auftrag_rechnung_glade;
        SigC::Connection timeout_connection;
        Rechnung rechnung;
         
        void fill_optionmenu_zahlungsart();
        void optionmenu_zahlart_deactivate();
        void set_tree_titles();
        void set_rtree_daten_content(RechnungBase::ID rngid);
        void set_rtree_offen_content(Kunde::ID kid);
        void on_rdaten_leaf_selected(cH_RowDataBase d);
        void on_roffen_leaf_selected(cH_RowDataBase d);
        void newRechnung(const Kunde::ID kid);
        void on_rng_close();
        void on_rng_neu();
        void on_clear_all();        
        void on_rng_save();
        void on_rng_preview();
        gint on_rng_print(GdkEventButton *ev);
        void rngzeile_delete();
        void on_rngdate_activate();
        gint timeout();
        void on_rngnr_activate();
        void on_rngnr_search(int *_continue, GtkSCContext newsearch);
        void on_lieferkunde_activate();
//        void on_selectrow_rechnung(int row, int col, GdkEvent* b);
        void on_unselectrow_rtree(int row, int col, GdkEvent* b);
        void preis_activate();
        void lieferschein_uebernehmen();
//        void on_selectrow_offlief(int row, int col, GdkEvent* b);
        void on_unselectrow_rtree_offen(int row, int col, GdkEvent* b);        
        void Preis_setzen();
        void Preis_ergaenzen();
        void on_checkbutton_ean_drucken_clicked();

        void waehrung_geaendert();
        void on_lieferkunde_reset();        
	void rabatt_geaendert();
        gint on_bezahlt_toggled(GdkEventButton *ev);
	void on_radiobutton_preiseingabe_toggled();
        void on_zahlziel_activate();
	void redisplay();
	void clear_rng();
   void on_button_gutschrift_clicked();
   void on_button_gutschrift_nein_clicked();
   void on_button_gutschrift_ja_clicked();

   void on_button_allopen_clicked();
   void on_button_lieferscheine_aufraumen_clicked();

        void on_notiz_changed();
        void on_notiz_save_clicked();

        void on_gutschrift_activate();
        void on_storno_activate();

   bool checkVerkConsist(const cH_Lieferschein &chl);

   void showBetraege();
   void on_provisionierung_activate();
// TreeRow *selectedrow_lief;
// TreeRow *selectedrow_rng;
// cH_Kunde kunde;

public:
 auftrag_rechnung(cH_ppsInstanz _instanz); 

};
#endif
