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

#ifndef _AUFTRAG_LIEFERSCHEIN_HH
#  include "auftrag_lieferschein_glade.hh"
#  define _AUFTRAG_LIEFERSCHEIN_HH

#include<Lieferschein/LieferscheinVoll.h>
#include<tclistrowdata.h>
#include<Auftrag/AufEintragBase.h>
#include<Artikel/Einheiten.h>
#include "auftrag_lieferschein_classes.h" 

class auftrag_lieferschein : public auftrag_lieferschein_glade
{   
        cH_ppsInstanz instanz;
        H_Lieferschein lieferschein;
//        bool show_offen;
	std::vector<cH_RowDataBase> datavec_liefoff;
	std::vector<cH_RowDataBase> datavec_liefdata;

        
        friend class auftrag_lieferschein_glade;
        void set_tree_titles();
        void set_tree_daten_content(LieferscheinBase::ID lfrsid);
        void set_tree_offen_content();
        void on_daten_leaf_selected(cH_RowDataBase d);
        void on_daten_unselect_row(int row, int col, GdkEvent* b);
        void on_offen_leaf_selected(cH_RowDataBase d);
        void on_unselectrow_offauf(int row, int col, GdkEvent* b);
        bool deleteLiefEntry();

	bool checkVerkConsist(const AufEintragBase &ae=AufEintragBase());
#ifdef DPD_LIEFERSCHEINE
        void on_button_rng_erledigt_clicked();
        void on_button_erledigt_clicked();
        void on_spinbutton_paeckchen_activate();
        gint on_spinbutton_paeckchen_focus_out_event(GdkEventFocus *ev);
        void on_spinbutton_pakete_activate();
        gint on_spinbutton_pakete_focus_out_event(GdkEventFocus *ev);
        void on_spinbutton_brutto_activate();
        gint on_spinbutton_brutto_focus_out_event(GdkEventFocus *ev);
        void on_spinbutton_netto_activate();
        gint on_spinbutton_netto_focus_out_event(GdkEventFocus *ev);
#else
        void on_button_rng_erledigt_clicked(){}
        void on_button_erledigt_clicked(){}
        void on_spinbutton_paeckchen_activate(){}
        gint on_spinbutton_paeckchen_focus_out_event(GdkEventFocus *ev){return false;}
        void on_spinbutton_pakete_activate(){}
        gint on_spinbutton_pakete_focus_out_event(GdkEventFocus *ev){return false;}
        void on_spinbutton_brutto_activate(){}
        gint on_spinbutton_brutto_focus_out_event(GdkEventFocus *ev){return false;}
        void on_spinbutton_netto_activate(){}
        gint on_spinbutton_netto_focus_out_event(GdkEventFocus *ev){return false;}
#endif
        void on_liefer_close();
        void on_ungeb_lief_show_activate();
        void on_ungeb_lief_print_activate();
        void on_petig_we_activate();
        gint on_liefer_delete(GdkEventAny *ev);
        void on_liefer_neu();
        void on_lief_save();
        void on_lief_preview();
        void on_lief_print();
        void on_checkbutton_ean_drucken_clicked();
        void on_liefnr_activate();
        void on_lieferkunde_reset();
        void on_liefnr_search(int *_continue, GtkSCContext newsearch);
        void on_lieferkunde_activate();
        void on_liefdate_activate();
        void on_liefermenge_activate();
        void on_newlieferentryall_ok();
        void on_Palette_activate();
        void auftragzeile_zeile_uebernehmen(const AufEintrag &AE);
        void on_artikelbox_activate();
        void on_button_zeile_modifizieren_clicked();

        void on_liefnotiz_changed();
        void on_liefnotiz_save_clicked();

        void liefzeile_delete();
        void on_button_liste_clicked();
        void on_lager_buchen_clicked();
	void erledigt_markieren();
	void on_lagerwahl_changed();

	void clear_input();
   void fill_input(const AufEintrag& AE);
   void fill_input(const AufEintrag& AE,const LieferscheinEntry& LE);
   void fill_with(const AufEintrag& AE,const Einheit& E,int stueck,
				double menge, bool check_bestand=false);
	void display(int lfrsid);
	void display2(int kdnr);

	void adjustOffAufEntry(cH_Data_Lieferdaten dt, int deltaMenge);
public:
	auftrag_lieferschein(cH_ppsInstanz _instanz);
	Kunde::ID getKdNr() { return liefer_kunde->get_value();}
};


#endif

