// generated 2005/2/17 11:44:45 CET by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/cvs/pps/auftrag.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/auftrag_bearbeiten.hh and./src/auftrag_bearbeiten.cc

#ifndef _AUFTRAG_BEARBEITEN_GLADE_HH
#  define _AUFTRAG_BEARBEITEN_GLADE_HH


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtk--/accelgroup.h>

class GlademmData
{  
        
        Gtk::AccelGroup *accgrp;
public:
        
        GlademmData(Gtk::AccelGroup *ag) : accgrp(ag)
        {  
        }
        
        Gtk::AccelGroup * getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtk--/window.h>
#include <gtk--/tooltips.h>
#include <libgnomeui/gtkpixmapmenuitem.h>
#include <gtk--/checkmenuitem.h>
#include <gtk--/button.h>
#include <gtk--/checkbutton.h>
#include "SearchComboContent.hh"
#include "WWaehrung.hh"
#include "WAuftragStatus.hh"
#include <gtk--/entry.h>
#include "ZahlungsArtBox.hh"
#include "Datewin.hh"
#include <gtk--/spinbutton.h>
#include "KundenBox2.hh"
#include "Optionmenu_AuftragLabel.hh"
#include "PreisBox.hh"
#include <gtk--/table.h>
#include "ArtikelBox.hh"
#include <gtk--/label.h>
#include "WPreis.hh"
#include <gtk--/clist.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/text.h>
#include <gtk--/frame.h>
#include <gtk--/paned.h>
#include <gtk--/box.h>

class auftrag_bearbeiten_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
protected:
        Gtk::Tooltips _tooltips;
        class Gtk::Window * auftrag_bearbeiten;
        class Gtk::CheckMenuItem * masseneingabe1;
        class Gtk::CheckMenuItem * kombinierte_artikel;
        class Gtk::CheckButton * checkbutton_ean_drucken;
        class Gtk::Button * newauftrag_button;
        class Gtk::Button * button_drucken;
        class SearchComboContent<int> * aufnr_scombo;
        class SearchComboContent<int> * youraufnr_scombo;
        class Gtk::Button * auftrag_ok;
        class Gtk::Button * auftrag_abbruch;
        class WWaehrung * bea_WWaehrung;
        class WAuftragStatus * WAufStat;
        class Gtk::Entry * auftragswert;
        class ZahlungsArtBox * zahlart;
        class Datewin * zahlziel_datewin;
        class Gtk::SpinButton * aufrabatt_spinbutton;
        class Datewin * aufdatum_datewin;
        class KundenBox2 * kundenbox;
        class Gtk::Entry * aufbemerkung_entry;
        class Optionmenu_AuftragLabel * auftrag_label;
        class Gtk::SpinButton * stkmtr_spinbutton;
        class WAuftragStatus * WAufEntryStat;
        class PreisBox * preislisten;
        class Gtk::CheckButton * preisautomatik;
        class Gtk::Table * table_preislisten;
        class Gtk::SpinButton * rabattentry_spinbutton;
        class ArtikelBox * artikelbox;
        class Datewin * liefdatum_datewin;
        class Gtk::Button * lager_bestand;
        class Gtk::Label * lagerbest_label;
        class Gtk::Label * bestellt_label;
        class Gtk::Label * verfuegbar_label;
        class Gtk::Label * lieferant_offen;
        class Gtk::Button * offen_bei_lieferant;
        class Gtk::Table * table_vorraetige_menge;
        class Gtk::Label * mengeeinheit;
        class WPreis * WPreis;
        class Gtk::Button * bestellplan;
        class Gtk::Button * aufentry_abbruch;
        class Gtk::Button * aufentry_ok;
        class Gtk::Table * table_auftragseintraege;
        class Gtk::CList * auftrag_clist;
        class Gtk::ScrolledWindow * scrolledwindow_auftraege;
        class Gtk::Text * notiz;
        class Gtk::Button * notiz_save;
        class Gtk::Frame * frame_notiz;
        class Gtk::VPaned * notiz_paned;
        class Gtk::VBox * aufeintrag_box;
        
        auftrag_bearbeiten_glade();
        
        ~auftrag_bearbeiten_glade();
private:
        virtual void on_backtomain_button_clicked() = 0;
        virtual void on_newauftrag_button_clicked() = 0;
        virtual void on_clear_all() = 0;
        virtual void on_auftrag_kopieren_activate() = 0;
        virtual void on_provisionierung_activate() = 0;
        virtual void on_splitten() = 0;
        virtual void on_ean_etiketten_activate() = 0;
        virtual void on_button_preview_clicked() = 0;
        virtual gint on_button_drucken_clicked(GdkEventButton *ev) = 0;
        virtual void on_checkbutton_ean_drucken_clicked() = 0;
        virtual void on_rueckstand_clicked() = 0;
        virtual void on_aufnrscombo_activate() = 0;
        virtual void on_aufnrscombo_search(gboolean *cont,GtkSCContext context) = 0;
        virtual void on_youraufnrscombo_activate() = 0;
        virtual void on_youraufnrscombo_search(gboolean *cont,GtkSCContext context) = 0;
        virtual void on_auftrag_ok_clicked() = 0;
        virtual void on_auftrag_abbruch_clicked() = 0;
        virtual void waehrung_geaendert() = 0;
        virtual void auftragstatus_geaendert() = 0;
        virtual void on_zahlart_activate() = 0;
        virtual void on_zahlziel_activate() = 0;
        virtual gint on_aufrabatt_spinbutton_focus_out_event(GdkEventFocus *ev) = 0;
        virtual void on_aufrabatt_spinbutton_activate() = 0;
        virtual void on_kunden_activate() = 0;
        virtual void on_aufbemerkung_activate() = 0;
        virtual void auftraglabel_geaendert() = 0;
        virtual void on_stkmtr_spinbutton_activate() = 0;
        virtual void on_aufentrystat_optionmenu_clicked() = 0;
        virtual void preisliste_reset() = 0;
        virtual void on_auftrag_preislisten_activate() = 0;
        virtual void on_preisautomatik_clicked() = 0;
        virtual void on_rabattentry_spinbutton_activate() = 0;
        virtual void onSelArtikel() = 0;
        virtual void on_lieferdatum_activate() = 0;
        virtual void on_offen_bei_lieferant_clicked() = 0;
        virtual void on_activate_wpreis() = 0;
        virtual void on_preis_changed() = 0;
        virtual void on_bestellplan_clicked() = 0;
        virtual void on_aufentry_abbruch_clicked() = 0;
        virtual void on_aufentry_ok_clicked() = 0;
        virtual void on_auftrag_clist_select_row(gint row, gint column, GdkEvent *event) = 0;
        virtual void on_auftrag_clist_unselect_row(gint row, gint column, GdkEvent *event) = 0;
        virtual void on_notiz_changed() = 0;
        virtual void on_notiz_save_clicked() = 0;
};
#endif
