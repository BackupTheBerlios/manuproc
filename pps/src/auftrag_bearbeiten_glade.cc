// generated 2003/5/13 15:53:08 MEST by jacek@ijacek.jacek.de
// using glademm V1.1.3f_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- auftrag.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/auftrag_bearbeiten.cc

#include "config.h"
#include "auftrag_bearbeiten_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtk--/accelgroup.h>
#include <gtk--/menuitem.h>
#include <gtk--/menu.h>
#include <gtk--/menubar.h>
#include <libgnomeui/gnome-app.h>
#include <libgnomeui/gnome-app-helper.h>
#include <libgnomeui/gnome-stock.h>
#include <libgnome/libgnome.h>
#include <gtk--/button.h>
#include <gtk--/toolbar.h>
#include <gtk--/label.h>
#include <gtk--/adjustment.h>
#include <gtk--/table.h>
#include <gtk--/frame.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/box.h>

auftrag_bearbeiten_glade::auftrag_bearbeiten_glade(
) : Gtk::Window(GTK_WINDOW_TOPLEVEL)
{  auftrag_bearbeiten = this;
   
   Gtk::AccelGroup *auftrag_bearbeiten_accgrp = Gtk::AccelGroup::create();
   gmm_data = new GlademmData(auftrag_bearbeiten_accgrp);
   
   Gtk::MenuBar *menubar1 = manage(new class Gtk::MenuBar());
   Gtk::MenuItem *schlie_en1;
   Gtk::MenuItem *neu1;
   Gtk::MenuItem *allesleeren1;
   Gtk::MenuItem *kopieren1;
   Gtk::MenuItem *splitten1;
   Gtk::MenuItem *datei1;
   Gtk::MenuItem *auftrag;
   Gtk::MenuItem *auftragseintr_ge1;
   {  static GnomeUIInfo datei1_menu_uiinfo[] = 
      {       GNOMEUIINFO_MENU_CLOSE_ITEM(0, 0), 
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo auftrag_menu_uiinfo[] = 
      {       GNOMEUIINFO_MENU_NEW_ITEM("_Neu", "", 0, 0), 
	      { GNOME_APP_UI_ITEM, "alles _leeren", 0,  0, 0, 0, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_STOP,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_TOGGLEITEM, "Masseneingabe", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  GDK_F1, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Kopieren", "Erstellt neuen Auftrag und �bernimt die Eintr�ge des aktuellen Auftrages",  0, 0, 0, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_COPY,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo auftragseintr_ge1_menu_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "_splitten", 0,  0, 0, 0, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_SCORES,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo menubar1_uiinfo[] = 
      {       GNOMEUIINFO_MENU_FILE_TREE(datei1_menu_uiinfo),
	      { GNOME_APP_UI_SUBTREE, "Auftrag", 0,  auftrag_menu_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_SUBTREE, "Auftragseintr�ge", 0,  auftragseintr_ge1_menu_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      gnome_app_fill_menu(GTK_MENU_SHELL(menubar1->gtkobj()), menubar1_uiinfo, gmm_data->getAccelGroup()->gtkobj(), true, 0);
      schlie_en1 = Gtk::wrap(GTK_MENU_ITEM(datei1_menu_uiinfo[0].widget));
      neu1 = Gtk::wrap(GTK_MENU_ITEM(auftrag_menu_uiinfo[0].widget));
      allesleeren1 = Gtk::wrap(GTK_MENU_ITEM(auftrag_menu_uiinfo[1].widget));
      masseneingabe1 = Gtk::wrap(GTK_CHECK_MENU_ITEM(auftrag_menu_uiinfo[2].widget));
      kopieren1 = Gtk::wrap(GTK_MENU_ITEM(auftrag_menu_uiinfo[3].widget));
      splitten1 = Gtk::wrap(GTK_MENU_ITEM(auftragseintr_ge1_menu_uiinfo[0].widget));
   }
   checkbutton_ean_drucken = manage(new class Gtk::CheckButton("EAN", 0, 0.5));
   
   Gtk::Toolbar *toolbar2 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::Label *label26 = manage(new class Gtk::Label("Auftragsnummer"));
   Gtk::Label *label27 = manage(new class Gtk::Label("Kunden Auf. Nr."));
   aufnr_scombo = new class SearchComboContent<int>();
   youraufnr_scombo = new class SearchComboContent<int>();
   
   Gtk::Label *label28 = manage(new class Gtk::Label("Auftragsstatus"));
   Gtk::Toolbar *toolbar4 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::Label *label31 = manage(new class Gtk::Label("Bemerkung"));
   aufbemerkung_entry = manage(new class Gtk::Entry());
   aufdatum_datewin = new class Datewin();
   
   Gtk::Label *label32 = manage(new class Gtk::Label("Rabatt %"));
   Gtk::Adjustment *aufrabatt_spinbutton_adj = manage(new class Gtk::Adjustment(0, 0, 100, 1, 10, 10));
   aufrabatt_spinbutton = manage(new class Gtk::SpinButton(*aufrabatt_spinbutton_adj, 1, 2));
   
   Gtk::Label *label34 = manage(new class Gtk::Label("Zahlungsart"));
   Gtk::Label *label37 = manage(new class Gtk::Label("Waehrung"));
   auftrag_ok = Gtk::wrap((GtkButton*)gnome_stock_button(GNOME_STOCK_BUTTON_OK));
   auftrag_abbruch = Gtk::wrap((GtkButton*)gnome_stock_button(GNOME_STOCK_BUTTON_CANCEL));
   kundenbox = new class KundenBox2();
   bea_WWaehrung = new class WWaehrung(WWaehrung::KURZ);
   WAufStat = new class WAuftragStatus();
   
   Gtk::Label *label30 = manage(new class Gtk::Label("Jahrgang"));
   Gtk::Label *label90 = manage(new class Gtk::Label("Auftragswert"));
   Gtk::Adjustment *jahrgang_spinbutton_adj = manage(new class Gtk::Adjustment(0, 2000, 2100, 1, 10, 10));
   jahrgang_spinbutton = manage(new class Gtk::SpinButton(*jahrgang_spinbutton_adj, 1, 0));
   auftragswert = manage(new class Gtk::Entry());
   zahlart = new class ZahlungsArtBox();
   zahlziel_datewin = new class Datewin();
   
   Gtk::Label *label99 = manage(new class Gtk::Label("Valuta"));
   Gtk::Table *table5 = manage(new class Gtk::Table(5, 6, false));
   Gtk::Frame *frame2 = manage(new class Gtk::Frame("Auftrag"));
   Gtk::Label *label25 = manage(new class Gtk::Label("Eintragstatus"));
   aufentry_abbruch = Gtk::wrap((GtkButton*)gnome_stock_button(GNOME_STOCK_BUTTON_CANCEL));
   aufentry_ok = Gtk::wrap((GtkButton*)gnome_stock_button(GNOME_STOCK_BUTTON_OK));
   mengeeinheit = manage(new class Gtk::Label("kg"));
   
   Gtk::Label *label67 = manage(new class Gtk::Label("Menge"));
   Gtk::Adjustment *stkmtr_spinbutton_adj = manage(new class Gtk::Adjustment(0, 0, 1e+06, 1, 10, 10));
   stkmtr_spinbutton = manage(new class Gtk::SpinButton(*stkmtr_spinbutton_adj, 1, 0));
   
   Gtk::Label *label29 = manage(new class Gtk::Label("Rabatt"));
   Gtk::Label *label73 = manage(new class Gtk::Label("%"));
   artikelbox = new class ArtikelBox("artbox_pps", "auftragserfassung");
   WAufEntryStat = new class WAuftragStatus();
   liefdatum_datewin = new class Datewin();
   WPreis = new class WPreis(false);
   
   Gtk::Adjustment *rabattentry_spinbutton_adj = manage(new class Gtk::Adjustment(0, 0, 100, 1, 10, 10));
   rabattentry_spinbutton = manage(new class Gtk::SpinButton(*rabattentry_spinbutton_adj, 1, 2));
   preislisten = new class PreisBox();
   
   Gtk::Label *label91 = manage(new class Gtk::Label("Preisliste"));
   preisautomatik = manage(new class Gtk::CheckButton("Preisautomatik", 0, 0.5));
   table_preislisten = manage(new class Gtk::Table(1, 3, false));
   table_auftragseintraege = manage(new class Gtk::Table(4, 9, false));
   
   Gtk::Frame *frame3 = manage(new class Gtk::Frame("Auftragseintr�ge"));
   auftrag_clist = manage(new class Gtk::CList(11));
   scrolledwindow_auftraege = manage(new class Gtk::ScrolledWindow());
   notiz = manage(new class Gtk::Text());
   
   Gtk::ScrolledWindow *scrolledwindow12 = manage(new class Gtk::ScrolledWindow());
   Gtk::Toolbar *toolbar13 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::HBox *hbox15 = manage(new class Gtk::HBox(false, 0));
   Gtk::Frame *frame24 = manage(new class Gtk::Frame("Notiz"));
   notiz_paned = manage(new class Gtk::VPaned());
   aufeintrag_box = manage(new class Gtk::VBox(false, 0));
   
   Gtk::VBox *vbox4 = manage(new class Gtk::VBox(false, 0));
   
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Zur�ck", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_EXIT)), 0));
   Gtk::Button *backtomain_button = static_cast<Gtk::Button *>(toolbar2->tools().back()->get_widget());
   
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("neuer Auftrag", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_NEW)), 0));
   Gtk::Button *newauftrag_button = static_cast<Gtk::Button *>(toolbar2->tools().back()->get_widget());
   
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("alles leeren", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_CLEAR)), 0));
   Gtk::Button *button8 = static_cast<Gtk::Button *>(toolbar2->tools().back()->get_widget());
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::Space());
   
   
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Anzeigen", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_SEARCH)), 0));
   Gtk::Button *button_preview = static_cast<Gtk::Button *>(toolbar2->tools().back()->get_widget());
   
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Drucken", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_PRINT)), 0));
   button_drucken = static_cast<Gtk::Button *>(toolbar2->tools().back()->get_widget());
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::Element(*checkbutton_ean_drucken));
   
   
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("R�ckstand", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_PRINT)), 0));
   Gtk::Button *rueckstand = static_cast<Gtk::Button *>(toolbar2->tools().back()->get_widget());
   
   toolbar13->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("speichern", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_SAVE)), 0));
   notiz_save = static_cast<Gtk::Button *>(toolbar13->tools().back()->get_widget());
   masseneingabe1->set_active(false);
   masseneingabe1->set_show_toggle(true);
   menubar1->set_shadow_type(GTK_SHADOW_OUT);
   _tooltips.set_tip(*newauftrag_button, "neuer Auftrag: Kunde eingeben, dann hier dr�cken, dann Kundenauftrag eingeben, dann oberes OK anw�hlen", "");
   checkbutton_ean_drucken->set_flags(GTK_CAN_FOCUS);
   checkbutton_ean_drucken->set_mode(true);
   checkbutton_ean_drucken->set_active(false);
   toolbar2->set_space_size(5);
   toolbar2->set_tooltips(true);
   toolbar2->set_space_style(GTK_TOOLBAR_SPACE_EMPTY);
   label26->set_alignment(0,0.5);
   label26->set_padding(0,0);
   label26->set_justify(GTK_JUSTIFY_LEFT);
   label26->set_line_wrap(false);
   label27->set_alignment(0,0.5);
   label27->set_padding(0,0);
   label27->set_justify(GTK_JUSTIFY_LEFT);
   label27->set_line_wrap(false);
   label28->set_alignment(0,0.5);
   label28->set_padding(0,0);
   label28->set_justify(GTK_JUSTIFY_LEFT);
   label28->set_line_wrap(false);
   toolbar4->set_space_size(0);
   toolbar4->set_tooltips(true);
   toolbar4->set_space_style(GTK_TOOLBAR_SPACE_EMPTY);
   label31->set_alignment(0,0.5);
   label31->set_padding(0,0);
   label31->set_justify(GTK_JUSTIFY_LEFT);
   label31->set_line_wrap(false);
   aufbemerkung_entry->set_flags(GTK_CAN_FOCUS);
   _tooltips.set_tip(*aufbemerkung_entry, "Eingabe mit 'Enter' abschlie�en.", "");
   aufbemerkung_entry->set_visibility(true);
   aufbemerkung_entry->set_editable(true);
   aufbemerkung_entry->set_text("");
   label32->set_alignment(0,0.5);
   label32->set_padding(0,0);
   label32->set_justify(GTK_JUSTIFY_LEFT);
   label32->set_line_wrap(false);
   aufrabatt_spinbutton->set_sensitive(false);
   aufrabatt_spinbutton->set_flags(GTK_CAN_FOCUS);
   aufrabatt_spinbutton->set_update_policy(GTK_UPDATE_ALWAYS);
   aufrabatt_spinbutton->set_numeric(true);
   aufrabatt_spinbutton->set_digits(2);
   aufrabatt_spinbutton->set_wrap(false);
   aufrabatt_spinbutton->set_snap_to_ticks(false);
   label34->set_alignment(0,0.5);
   label34->set_padding(0,0);
   label34->set_justify(GTK_JUSTIFY_LEFT);
   label34->set_line_wrap(false);
   label37->set_alignment(0,0.5);
   label37->set_padding(0,0);
   label37->set_justify(GTK_JUSTIFY_LEFT);
   label37->set_line_wrap(false);
   auftrag_ok->set_usize(102,-1);
   auftrag_ok->set_sensitive(false);
   auftrag_ok->set_flags(GTK_CAN_FOCUS);
   auftrag_ok->set_relief(GTK_RELIEF_NORMAL);
   auftrag_abbruch->set_sensitive(false);
   auftrag_abbruch->set_flags(GTK_CAN_FOCUS);
   auftrag_abbruch->set_relief(GTK_RELIEF_NORMAL);
   label30->set_alignment(0,0.5);
   label30->set_padding(0,0);
   label30->set_justify(GTK_JUSTIFY_LEFT);
   label30->set_line_wrap(false);
   label90->set_alignment(0,0.5);
   label90->set_padding(0,0);
   label90->set_justify(GTK_JUSTIFY_CENTER);
   label90->set_line_wrap(false);
   jahrgang_spinbutton->set_flags(GTK_CAN_FOCUS);
   jahrgang_spinbutton->set_update_policy(GTK_UPDATE_ALWAYS);
   jahrgang_spinbutton->set_numeric(true);
   jahrgang_spinbutton->set_digits(0);
   jahrgang_spinbutton->set_wrap(false);
   jahrgang_spinbutton->set_snap_to_ticks(false);
   auftragswert->set_usize(45,-1);
   auftragswert->set_sensitive(false);
   auftragswert->set_flags(GTK_CAN_FOCUS);
   auftragswert->set_visibility(true);
   auftragswert->set_editable(false);
   auftragswert->set_text("");
   label99->set_alignment(0,0.5);
   label99->set_padding(0,0);
   label99->set_justify(GTK_JUSTIFY_CENTER);
   label99->set_line_wrap(false);
   table5->set_border_width(5);
   table5->set_row_spacings(0);
   table5->set_col_spacings(0);
   table5->attach(*label26, 0, 1, 1, 2, GTK_FILL, 0, 0, 0);
   table5->attach(*label27, 0, 1, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*aufnr_scombo, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*youraufnr_scombo, 1, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*label28, 0, 1, 3, 4, GTK_FILL, 0, 0, 0);
   table5->attach(*toolbar4, 3, 4, 1, 3, 0, 0, 0, 0);
   table5->attach(*label31, 0, 1, 4, 5, GTK_FILL, 0, 0, 0);
   table5->attach(*aufbemerkung_entry, 1, 4, 4, 5, GTK_EXPAND|GTK_FILL, 0, 0, 0);
   table5->attach(*aufdatum_datewin, 2, 4, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*label32, 4, 5, 0, 1, GTK_FILL, 0, 0, 0);
   table5->attach(*aufrabatt_spinbutton, 5, 6, 0, 1, GTK_EXPAND|GTK_FILL, 0, 0, 0);
   table5->attach(*label34, 4, 5, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*label37, 4, 5, 3, 4, GTK_FILL, 0, 0, 0);
   table5->attach(*auftrag_ok, 4, 5, 4, 5, GTK_FILL, 0, 0, 0);
   table5->attach(*auftrag_abbruch, 5, 6, 4, 5, 0, 0, 0, 0);
   table5->attach(*kundenbox, 0, 4, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*bea_WWaehrung, 5, 6, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*WAufStat, 1, 2, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*label30, 2, 3, 3, 4, GTK_FILL, 0, 0, 0);
   table5->attach(*label90, 2, 3, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*jahrgang_spinbutton, 3, 4, 3, 4, GTK_FILL, 0, 0, 0);
   table5->attach(*auftragswert, 3, 4, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*zahlart, 5, 6, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*zahlziel_datewin, 5, 6, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*label99, 4, 5, 1, 2, GTK_FILL, 0, 0, 0);
   frame2->set_border_width(5);
   frame2->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame2->set_label_align(0,0);
   frame2->add(*table5);
   label25->set_alignment(0,0.5);
   label25->set_padding(0,0);
   label25->set_justify(GTK_JUSTIFY_LEFT);
   label25->set_line_wrap(false);
   aufentry_abbruch->set_flags(GTK_CAN_FOCUS);
   aufentry_abbruch->set_relief(GTK_RELIEF_NORMAL);
   aufentry_ok->set_flags(GTK_CAN_FOCUS);
   aufentry_ok->set_relief(GTK_RELIEF_NORMAL);
   mengeeinheit->set_alignment(0,0.5);
   mengeeinheit->set_padding(0,0);
   mengeeinheit->set_justify(GTK_JUSTIFY_LEFT);
   mengeeinheit->set_line_wrap(false);
   label67->set_usize(40,-1);
   label67->set_alignment(0.89,0);
   label67->set_padding(0,0);
   label67->set_justify(GTK_JUSTIFY_RIGHT);
   label67->set_line_wrap(false);
   stkmtr_spinbutton->set_usize(70,-1);
   stkmtr_spinbutton->set_flags(GTK_CAN_FOCUS);
   stkmtr_spinbutton->set_update_policy(GTK_UPDATE_ALWAYS);
   stkmtr_spinbutton->set_numeric(true);
   stkmtr_spinbutton->set_digits(0);
   stkmtr_spinbutton->set_wrap(false);
   stkmtr_spinbutton->set_snap_to_ticks(false);
   label29->set_usize(40,-1);
   label29->set_alignment(0.1,0.5);
   label29->set_padding(0,0);
   label29->set_justify(GTK_JUSTIFY_LEFT);
   label29->set_line_wrap(false);
   label73->set_alignment(0,0.5);
   label73->set_padding(0,0);
   label73->set_justify(GTK_JUSTIFY_LEFT);
   label73->set_line_wrap(false);
   WPreis->set_usize(110,-1);
   rabattentry_spinbutton->set_usize(70,-1);
   rabattentry_spinbutton->set_flags(GTK_CAN_FOCUS);
   rabattentry_spinbutton->set_update_policy(GTK_UPDATE_ALWAYS);
   rabattentry_spinbutton->set_numeric(true);
   rabattentry_spinbutton->set_digits(2);
   rabattentry_spinbutton->set_wrap(false);
   rabattentry_spinbutton->set_snap_to_ticks(false);
   preislisten->set_sensitive(false);
   label91->set_alignment(0.5,0.5);
   label91->set_padding(0,0);
   label91->set_justify(GTK_JUSTIFY_CENTER);
   label91->set_line_wrap(false);
   preisautomatik->set_flags(GTK_CAN_FOCUS);
   preisautomatik->set_mode(true);
   preisautomatik->set_active(true);
   table_preislisten->set_row_spacings(0);
   table_preislisten->set_col_spacings(0);
   table_preislisten->attach(*preislisten, 1, 2, 0, 1, GTK_EXPAND|GTK_FILL, GTK_EXPAND|GTK_FILL, 0, 0);
   table_preislisten->attach(*label91, 0, 1, 0, 1, GTK_FILL, 0, 0, 0);
   table_preislisten->attach(*preisautomatik, 2, 3, 0, 1, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->set_border_width(5);
   table_auftragseintraege->set_row_spacings(0);
   table_auftragseintraege->set_col_spacings(0);
   table_auftragseintraege->attach(*label25, 8, 9, 2, 3, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*aufentry_abbruch, 6, 7, 2, 3, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*aufentry_ok, 6, 7, 3, 4, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*mengeeinheit, 3, 4, 3, 4, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*label67, 1, 2, 3, 4, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*stkmtr_spinbutton, 2, 3, 3, 4, 0, 0, 0, 0);
   table_auftragseintraege->attach(*label29, 1, 2, 2, 3, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*label73, 3, 4, 2, 3, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*artikelbox, 0, 5, 1, 2, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 5);
   table_auftragseintraege->attach(*WAufEntryStat, 8, 9, 3, 4, GTK_SHRINK|GTK_FILL, GTK_SHRINK, 0, 0);
   table_auftragseintraege->attach(*liefdatum_datewin, 4, 5, 2, 4, GTK_SHRINK|GTK_FILL, GTK_SHRINK, 0, 0);
   table_auftragseintraege->attach(*WPreis, 0, 1, 2, 4, GTK_EXPAND|GTK_SHRINK|GTK_FILL, GTK_EXPAND|GTK_SHRINK, 0, 0);
   table_auftragseintraege->attach(*rabattentry_spinbutton, 2, 3, 2, 3, GTK_SHRINK|GTK_FILL, GTK_SHRINK, 0, 0);
   table_auftragseintraege->attach(*table_preislisten, 0, 5, 0, 1, GTK_SHRINK|GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);
   frame3->set_border_width(5);
   frame3->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame3->set_label_align(0,0);
   frame3->add(*table_auftragseintraege);
   auftrag_clist->set_flags(GTK_CAN_FOCUS);
   auftrag_clist->set_column_title(0, "Pos.");
   auftrag_clist->set_column_justification(0, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(0, 80);
   auftrag_clist->set_column_title(1, "St�ck/Meter");
   auftrag_clist->set_column_justification(1, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(1, 80);
   auftrag_clist->set_column_title(2, "Artikel");
   auftrag_clist->set_column_width(2, 80);
   auftrag_clist->set_column_title(3, "Menge");
   auftrag_clist->set_column_justification(3, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(3, 80);
   auftrag_clist->set_column_title(4, "Rest");
   auftrag_clist->set_column_justification(4, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(4, 80);
   auftrag_clist->set_column_title(5, "EPreis");
   auftrag_clist->set_column_justification(5, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(5, 80);
   auftrag_clist->set_column_title(6, "Rabatt");
   auftrag_clist->set_column_justification(6, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(6, 80);
   auftrag_clist->set_column_title(7, "GPreis");
   auftrag_clist->set_column_justification(7, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(7, 80);
   auftrag_clist->set_column_title(8, "Lief.Termin");
   auftrag_clist->set_column_width(8, 80);
   auftrag_clist->set_column_title(9, "Stand");
   auftrag_clist->set_column_width(9, 80);
   auftrag_clist->set_column_title(10, "�nderung");
   auftrag_clist->set_column_width(10, 80);
   auftrag_clist->set_selection_mode(GTK_SELECTION_SINGLE);
   auftrag_clist->set_shadow_type(GTK_SHADOW_IN);
   auftrag_clist->column_titles_show();
   scrolledwindow_auftraege->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow_auftraege->add(*auftrag_clist);
   notiz->set_flags(GTK_CAN_FOCUS);
   notiz->set_editable(true);
   scrolledwindow12->set_policy(GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
   scrolledwindow12->add(*notiz);
   notiz_save->set_sensitive(false);
   toolbar13->set_space_size(5);
   toolbar13->set_tooltips(true);
   toolbar13->set_space_style(GTK_TOOLBAR_SPACE_EMPTY);
   hbox15->pack_end(*scrolledwindow12);
   hbox15->pack_start(*toolbar13, false, false, 0);
   frame24->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame24->set_label_align(0,0);
   frame24->add(*hbox15);
   notiz_paned->set_border_width(5);
   notiz_paned->set_handle_size(15);
   notiz_paned->set_gutter_size(6);
   notiz_paned->set_position(0);
   notiz_paned->pack1(*scrolledwindow_auftraege, true, true);
   notiz_paned->pack2(*frame24, false, true);
   aufeintrag_box->pack_start(*frame3, false, true, 0);
   aufeintrag_box->pack_start(*notiz_paned);
   vbox4->pack_start(*menubar1, false, false, 0);
   vbox4->pack_start(*toolbar2, false, false, 0);
   vbox4->pack_start(*frame2, false, true, 0);
   vbox4->pack_start(*aufeintrag_box);
   auftrag_bearbeiten->set_title("Auftragserfassung");
   auftrag_bearbeiten->set_default_size(800,600);
   auftrag_bearbeiten->set_modal(false);
   auftrag_bearbeiten->set_position(GTK_WIN_POS_NONE);
   auftrag_bearbeiten->set_policy(true, true, false);
   auftrag_bearbeiten->add_accel_group(*(gmm_data->getAccelGroup()));
   auftrag_bearbeiten->add(*vbox4);
   menubar1->show();
   toolbar2->show();
   label26->show();
   label27->show();
   aufnr_scombo->show();
   youraufnr_scombo->show();
   label28->show();
   toolbar4->show();
   label31->show();
   aufbemerkung_entry->show();
   aufdatum_datewin->show();
   label32->show();
   aufrabatt_spinbutton->show();
   label34->show();
   label37->show();
   auftrag_ok->show();
   auftrag_abbruch->show();
   kundenbox->show();
   bea_WWaehrung->show();
   WAufStat->show();
   label30->show();
   label90->show();
   jahrgang_spinbutton->show();
   auftragswert->show();
   zahlart->show();
   zahlziel_datewin->show();
   label99->show();
   table5->show();
   frame2->show();
   label25->show();
   aufentry_abbruch->show();
   aufentry_ok->show();
   mengeeinheit->show();
   label67->show();
   stkmtr_spinbutton->show();
   label29->show();
   label73->show();
   artikelbox->show();
   WAufEntryStat->show();
   liefdatum_datewin->show();
   WPreis->show();
   rabattentry_spinbutton->show();
   preislisten->show();
   label91->show();
   preisautomatik->show();
   table_preislisten->show();
   table_auftragseintraege->show();
   frame3->show();
   auftrag_clist->show();
   scrolledwindow_auftraege->show();
   notiz->show();
   scrolledwindow12->show();
   toolbar13->show();
   hbox15->show();
   frame24->show();
   notiz_paned->show();
   aufeintrag_box->show();
   vbox4->show();
   auftrag_bearbeiten->show();
   schlie_en1->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_backtomain_button_clicked));
   neu1->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_newauftrag_button_clicked));
   allesleeren1->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_clear_all));
   kopieren1->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_auftrag_kopieren_activate));
   splitten1->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_splitten));
   backtomain_button->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_backtomain_button_clicked));
   newauftrag_button->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_newauftrag_button_clicked));
   button8->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_clear_all));
   button_preview->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_button_preview_clicked));
   button_drucken->button_press_event.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_button_drucken_clicked));
   checkbutton_ean_drucken->clicked.connect_after(SigC::slot(this, &auftrag_bearbeiten_glade::on_checkbutton_ean_drucken_clicked));
   rueckstand->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_rueckstand_clicked));
   aufnr_scombo->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_aufnrscombo_activate));
   aufnr_scombo->search.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_aufnrscombo_search));
   youraufnr_scombo->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_youraufnrscombo_activate));
   youraufnr_scombo->search.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_youraufnrscombo_search));
   aufbemerkung_entry->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_aufbemerkung_activate));
   aufrabatt_spinbutton->focus_out_event.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_aufrabatt_spinbutton_focus_out_event));
   auftrag_ok->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_auftrag_ok_clicked));
   auftrag_abbruch->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_auftrag_abbruch_clicked));
   kundenbox->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_kunden_activate));
   bea_WWaehrung->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::waehrung_geaendert));
   WAufStat->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::auftragstatus_geaendert));
   jahrgang_spinbutton->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_jahrgang_spinbutton_activate));
   zahlart->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_zahlart_activate));
   zahlziel_datewin->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_zahlziel_activate));
   aufentry_abbruch->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_aufentry_abbruch_clicked));
   aufentry_ok->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_aufentry_ok_clicked));
   stkmtr_spinbutton->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_stkmtr_spinbutton_activate));
   artikelbox->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::onSelArtikel));
   WAufEntryStat->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_aufentrystat_optionmenu_clicked));
   liefdatum_datewin->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_lieferdatum_activate));
   WPreis->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_activate_wpreis));
   WPreis->preis_changed.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_preis_changed));
   rabattentry_spinbutton->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_rabattentry_spinbutton_activate));
   preislisten->reset_signal.connect(SigC::slot(this, &auftrag_bearbeiten_glade::preisliste_reset));
   preislisten->activate.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_auftrag_preislisten_activate));
   preisautomatik->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_preisautomatik_clicked));
   auftrag_clist->select_row.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_auftrag_clist_select_row));
   auftrag_clist->unselect_row.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_auftrag_clist_unselect_row));
   notiz->changed.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_notiz_changed));
   notiz_save->clicked.connect(SigC::slot(this, &auftrag_bearbeiten_glade::on_notiz_save_clicked));
}

auftrag_bearbeiten_glade::~auftrag_bearbeiten_glade()
{  delete aufnr_scombo;
   delete youraufnr_scombo;
   delete aufdatum_datewin;
   delete kundenbox;
   delete bea_WWaehrung;
   delete WAufStat;
   delete zahlart;
   delete zahlziel_datewin;
   delete artikelbox;
   delete WAufEntryStat;
   delete liefdatum_datewin;
   delete WPreis;
   delete preislisten;
   delete gmm_data;
}
