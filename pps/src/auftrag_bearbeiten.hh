// $Id: auftrag_bearbeiten.hh,v 1.31 2006/08/09 15:36:51 christof Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2005 Adolf Petig GmbH & Co. KG, 
 *  written by Jacek Jakubowski
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

#include <Auftrag/AuftragFull.h>
#include "aktAufEintrag.h"
#include <Misc/SQLerror.h>
#include <Artikel/Prozess.h>
#include <vector>
#include <gtkmm/liststore.h>

void ArtBestandData(const ArtikelBase::ID aid, int &off,
	int &bestand, int &verfuegbar) throw(SQLerror);

class auftrag_bearbeiten : public auftrag_bearbeiten_glade
{   
 cH_ppsInstanz instanz;
 AuftragFull *auftrag;
 AufEintrag *aktaufeintrag;
 cH_Kunde kunde;
 gint aufentrystat_chid;
 gint aufstat_chid;
 int selectedentry;
 bool artikel_preisliste_geaendert;

 bool newauftrag;

        void on_backtomain_button_clicked();
        void on_newauftrag_button_clicked();
        void auftragstatus_geaendert();
        void on_youraufnrscombo_activate();
        void on_youraufnrscombo_search(int *_continue, GtkSCContext newsearch)
							throw(SQLerror);
        void on_button_preview_clicked();
        void on_button_drucken_clicked();
        void on_checkbutton_ean_drucken_clicked();
        void on_rueckstand_clicked();
        void preisliste_reset();
        void on_zahlziel_activate();
        void on_zahlziel_showkal_button_clicked();
        void on_aufdat_showkal_button_clicked();
        void on_stkmtr_spinbutton_activate();
        void on_lieferdatum_activate();
        void on_showkal_button_clicked();
        void on_rabattentry_spinbutton_activate();
        bool on_aufrabatt_spinbutton_focus_out_event(GdkEventFocus *ev);
	void on_aufrabatt_spinbutton_activate();
        void Rabatt_setzen(const cH_Kunde &kunde,AuftragFull *a=NULL);
        void Rabatt_setzen(const cH_PreisListe &liste);
        void on_aufentrystat_optionmenu_clicked();
     	  void waehrung_geaendert();
        void on_preisart_optionmenu_clicked();
        void on_offen_bei_lieferant_clicked();
        void on_preisautomatik_clicked();
        void on_leaf_selected(cH_RowDataBase row);
        void on_unselect();
        void select_show(const AufEintragBase &aeb);
        void on_splitten();
        void on_ean_etiketten_activate();
        void on_clear_all();
        void on_auftrag_kopieren_activate();
        void on_provisionierung_activate();
        void on_aufentry_abbruch_clicked();
        void on_aufentry_ok_clicked();
        void on_auftrag_abbruch_clicked();
        void on_auftrag_ok_clicked();
        void on_aufbemerkung_activate();
        void auftraglabel_geaendert();        
        void on_kunden_activate();
	void on_zahlart_activate();
        void on_bestellplan_clicked();
        
   void on_activate_wpreis();
   bool update_aufentry_preis();
   void on_preis_changed();

        void on_notiz_changed();
        void on_notiz_save_clicked();
	void on_lager_bestand_clicked();
        void on_datum_activate();

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
 
 void on_auftrag_preislisten_activate();

 void showBestandData();
 void lieferungen_zeigen();

public:
 AuftragBase::ID new_aufid_from_copy;
 bool splitEntry();
 cH_ppsInstanz Instanz() const { return instanz; }
 AuftragFull *Auftrag() const { return auftrag; }

auftrag_bearbeiten(const cH_ppsInstanz& _instanz,const AufEintragBase *auftragbase);
};
#endif
