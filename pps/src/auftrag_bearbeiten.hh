// $Id: auftrag_bearbeiten.hh,v 1.15 2002/09/27 12:51:30 thoma Exp $
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

#ifndef _AUFTRAG_BEARBEITEN_HH
#  include "auftrag_bearbeiten_glade.hh"
#  define _AUFTRAG_BEARBEITEN_HH

#include "termsplidial.hh"
#include <Auftrag/AuftragFull.h>
#include "aktAufEintrag.h"
#include <Aux/SQLerror.h>
#include <Artikel/Prozess.h>
#include <vector>

class auftrag_bearbeiten : public auftrag_bearbeiten_glade
{   
 cH_ppsInstanz instanz;
 AuftragFull *auftrag;
 AufEintrag *aktaufeintrag;
 cH_Kunde kunde;
 gint aufentrystat_chid;
 gint aufstat_chid;
 int selectedentry;
 cH_PreisListe artikel_preisliste;
 bool artikel_preisliste_geaendert;

 bool newauftrag;

 termsplidial *splitdialog;

        friend class auftrag_bearbeiten_glade;
        void on_backtomain_button_clicked();
        void on_newauftrag_button_clicked();
        void auftragstatus_geaendert();
        void on_youraufnrscombo_activate();
        void on_youraufnrscombo_search(int *_continue, GtkSCContext newsearch)
							throw(SQLerror);
        void on_button_preview_clicked();
        gint on_button_drucken_clicked(GdkEventButton *ev);
        void on_checkbutton_ean_drucken_clicked();
        void on_rueckstand_clicked();
        void preisliste_reset();
        void on_zahlziel_activate();
        void on_zahlziel_showkal_button_clicked();
        void on_aufdat_showkal_button_clicked();
        void on_jahrgang_spinbutton_activate();
        void on_stkmtr_spinbutton_activate();
        void on_lieferdatum_activate();
        void on_showkal_button_clicked();
        void on_rabattentry_spinbutton_activate();
        gint on_aufrabatt_spinbutton_focus_out_event(GdkEventFocus *ev);
        void Rabatt_setzen(const cH_Kunde &kunde);
        void Rabatt_setzen(const cH_PreisListe &liste);
        void on_aufentrystat_optionmenu_clicked();
     	  void waehrung_geaendert();
        void on_preisart_optionmenu_clicked();
        void on_preisautomatik_clicked();
        void on_auftrag_clist_select_row(gint row, gint column, GdkEvent *event);
        void on_auftrag_clist_unselect_row(gint row, gint column, GdkEvent *event);
        void on_splitten();
        void on_clear_all();
        void on_aufentry_abbruch_clicked();
        void on_aufentry_ok_clicked();
//        void InstanzAuftraegeAnlegen(const Auftrag& altAuftrag,const int altZnr);
//        void InstanzAuftraegeAnlegenR(const ArtikelBase& art,const long menge,const Auftrag& altAuftrag,const int altZnr);
        void on_auftrag_abbruch_clicked();
        void on_auftrag_ok_clicked();
        void on_aufbemerkung_activate();
        void on_kunden_activate();
	void on_zahlart_activate();

   void on_activate_wpreis();
   void on_preis_changed();

        void on_notiz_changed();
        void on_notiz_save_clicked();

 void loadAuftrag(const AuftragBase& auftrag);
 void fillMask();
 void andererKunde();
 void clearEntry();
 void fillCList();

 void on_aufnrscombo_search(int *_continue, GtkSCContext newsearch) throw(SQLerror);
 void on_aufnrscombo_activate();
 void onSelArtikel();

 void setAufStat_Option(AufStatVal as); 
 int get_active_index(Gtk::Menu *om); 
 void setAufEntries();
 
//#ifdef MABELLA_EXTENSIONS Das geht so nicht, wiel das Signal in glade
// an diese Methode konnektiert wird.
 void on_auftrag_preislisten_activate();
//#endif;

public:
 
 bool splitEntry();
 void split_dialog_destroyed() { splitdialog=0; }

auftrag_bearbeiten(const cH_ppsInstanz& _instanz,const AufEintragBase *auftragbase);
~auftrag_bearbeiten();

};
#endif
