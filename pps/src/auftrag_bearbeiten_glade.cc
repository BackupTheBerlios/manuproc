// generated 2002/3/20 10:41:46 MET by christof@petig.petig.de
// using glademm V0.6.4b_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- auftrag.glade
// for gtk 1.2.9 and gtkmm 1.2.4
//
// Please modify the corresponding derived classes in ./src/auftrag_bearbeiten.cc

#include "config.h"
#include "auftrag_bearbeiten.hh"
#include <gtk--/list.h>
#include <gtk--/menu.h>
#include <gtk--/menubar.h>
#include <libgnomeui/gnome-app.h>
#include <libgnomeui/gnome-app-helper.h>
#include <gdk/gdkkeysyms.h>
#include <libgnomeui/gnome-stock.h>
#include <libgnome/libgnome.h>
#include <gtk--/button.h>
#include <gtk--/toolbar.h>
#include <gtk--/label.h>
#include <gtk--/adjustment.h>
#include <gtk--/menuitem.h>
#include <gtk--/table.h>
#include <gtk--/frame.h>
#include <gtk--/box.h>

auftrag_bearbeiten_glade::auftrag_bearbeiten_glade(
) : Gtk::Window(GTK_WINDOW_TOPLEVEL)
{  auftrag_bearbeiten = this;
   
   Gtk::MenuBar *menubar1 = manage(new class Gtk::MenuBar());
   Gtk::MenuItem *schlie_en1;
   Gtk::MenuItem *neu1;
   Gtk::MenuItem *allesleeren1;
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
	      { GNOME_APP_UI_ITEM, "alles_leeren", 0,  0, 0, 0, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_STOP,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo auftragseintr_ge1_menu_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "splitten", 0,  0, 0, 0, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_SCORES,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo menubar1_uiinfo[] = 
      {       GNOMEUIINFO_MENU_FILE_TREE(datei1_menu_uiinfo),
	      { GNOME_APP_UI_SUBTREE, "Auftrag", 0,  auftrag_menu_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_SUBTREE, "Auftragseintr�ge", 0,  auftragseintr_ge1_menu_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      gnome_app_fill_menu(GTK_MENU_SHELL(menubar1->gtkobj()), menubar1_uiinfo, NULL, true, 0);
      schlie_en1 = Gtk::wrap(GTK_MENU_ITEM(datei1_menu_uiinfo[0].widget));
      neu1 = Gtk::wrap(GTK_MENU_ITEM(auftrag_menu_uiinfo[0].widget));
      allesleeren1 = Gtk::wrap(GTK_MENU_ITEM(auftrag_menu_uiinfo[1].widget));
      splitten1 = Gtk::wrap(GTK_MENU_ITEM(auftragseintr_ge1_menu_uiinfo[0].widget));
   }
   Gtk::Toolbar *toolbar2 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::Button *backtomain_button = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar2->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Zur�ck", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar2->gtkobj()), GNOME_STOCK_PIXMAP_EXIT)), 0, 0));
   Gtk::Button *newauftrag_button = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar2->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "neuer Auftrag", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar2->gtkobj()), GNOME_STOCK_PIXMAP_NEW)), 0, 0));
   Gtk::Button *button8 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar2->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "alles leeren", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar2->gtkobj()), GNOME_STOCK_PIXMAP_CLEAR)), 0, 0));
   toolbar2->tools().push_back(Gtk::Toolbar_Helpers::Space());
   
   Gtk::Button *button_preview = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar2->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Anzeigen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar2->gtkobj()), GNOME_STOCK_PIXMAP_SEARCH)), 0, 0));
   Gtk::Button *button_drucken = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar2->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Drucken", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar2->gtkobj()), GNOME_STOCK_PIXMAP_PRINT)), 0, 0));
   Gtk::Label *label26 = manage(new class Gtk::Label("Auftragsnummer"));
   Gtk::Label *label27 = manage(new class Gtk::Label("Kunden Auf. Nr."));
   aufnr_scombo = new class SearchComboContent<int>();
   youraufnr_scombo = new class SearchComboContent<int>();
   
   Gtk::Label *label28 = manage(new class Gtk::Label("Auftragsstatus"));
   Gtk::Toolbar *toolbar4 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::Label *label31 = manage(new class Gtk::Label("Bemerkung"));
   aufbemerkung_entry = manage(new class Gtk::Entry());
   aufdatum_datewin = new class Datewin();
   
   Gtk::Label *label30 = manage(new class Gtk::Label("Jahrgang"));
   Gtk::Adjustment *jahrgang_spinbutton_adj = manage(new class Gtk::Adjustment(2001, 2000, 2100, 1, 10, 10));
   jahrgang_spinbutton = manage(new class Gtk::SpinButton(*jahrgang_spinbutton_adj, 1, 0));
   
   Gtk::Label *label32 = manage(new class Gtk::Label("Rabatt %"));
   Gtk::Adjustment *aufrabatt_spinbutton_adj = manage(new class Gtk::Adjustment(0, 0, 100, 1, 10, 10));
   aufrabatt_spinbutton = manage(new class Gtk::SpinButton(*aufrabatt_spinbutton_adj, 1, 2));
   zahlziel_datewin = new class Datewin();
   
   Gtk::Label *label34 = manage(new class Gtk::Label("Zahlungsart"));
   zahlart = manage(new class Gtk::OptionMenu());
   
   Gtk::Label *label37 = manage(new class Gtk::Label("Waehrung"));
   auftrag_ok = Gtk::wrap((GtkButton*)gnome_stock_button(GNOME_STOCK_BUTTON_OK));
   auftrag_abbruch = Gtk::wrap((GtkButton*)gnome_stock_button(GNOME_STOCK_BUTTON_CANCEL));
   kundenbox = new class KundenBox();
   bea_WWaehrung = new class WWaehrung(WWaehrung::KURZ);
   WAufStat = new class WAuftragStatus();
   
   Gtk::Table *table5 = manage(new class Gtk::Table(5, 6, false));
   Gtk::Frame *frame2 = manage(new class Gtk::Frame("Auftrag"));
   Gtk::Adjustment *kw_spinbutton_adj = manage(new class Gtk::Adjustment(1, 1, 56, 1, 10, 10));
   kw_spinbutton = manage(new class Gtk::SpinButton(*kw_spinbutton_adj, 1, 0));
   
   Gtk::Adjustment *jahr_spinbutton_adj = manage(new class Gtk::Adjustment(1999, 1999, 2100, 1, 10, 10));
   jahr_spinbutton = manage(new class Gtk::SpinButton(*jahr_spinbutton_adj, 1, 0));
   
   Gtk::HBox *hbox1 = manage(new class Gtk::HBox(false, 0));
   Gtk::Label *label23 = manage(new class Gtk::Label("Lief.Woche"));
   liefdatum_datewin = new class Datewin();
   
   Gtk::Label *label24 = manage(new class Gtk::Label("Lief. Datum"));
   liefertermin = manage(new class Gtk::Notebook());
   
   Gtk::Toolbar *toolbar3 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   showkal_button = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar3->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Kalender", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar3->gtkobj()), GNOME_STOCK_PIXMAP_BOOK_BLUE)), 0, 0));
   
   Gtk::HBox *hbox3 = manage(new class Gtk::HBox(false, 5));
   Gtk::Label *label25 = manage(new class Gtk::Label("Eintragstatus"));
   aufentry_abbruch = Gtk::wrap((GtkButton*)gnome_stock_button(GNOME_STOCK_BUTTON_CANCEL));
   aufentry_ok = Gtk::wrap((GtkButton*)gnome_stock_button(GNOME_STOCK_BUTTON_OK));
   artikelbox = new class ArtikelBox(1);
   mengeeinheit = manage(new class Gtk::Label("kg"));
   
   Gtk::Label *label67 = manage(new class Gtk::Label("Menge"));
   Gtk::Adjustment *stkmtr_spinbutton_adj = manage(new class Gtk::Adjustment(0, 0, 1e+06, 1, 10, 10));
   stkmtr_spinbutton = manage(new class Gtk::SpinButton(*stkmtr_spinbutton_adj, 1, 0));
   WAufEntryStat = new class WAuftragStatus();
   
   Gtk::Label *label29 = manage(new class Gtk::Label("Rabatt"));
   Gtk::Adjustment *rabattentry_spinbutton_adj = manage(new class Gtk::Adjustment(0, 0, 100, 1, 10, 10));
   rabattentry_spinbutton = manage(new class Gtk::SpinButton(*rabattentry_spinbutton_adj, 1, 2));
   WPreis = new class WPreis(false);
   
   Gtk::Label *label73 = manage(new class Gtk::Label("%"));
   table_auftragseintraege = manage(new class Gtk::Table(4, 9, false));
   
   Gtk::Frame *frame3 = manage(new class Gtk::Frame("Auftragseintr�ge"));
   auftrag_clist = manage(new class Gtk::CList(8));
   scrolledwindow_auftraege = manage(new class Gtk::ScrolledWindow());
   
   Gtk::VBox *vbox4 = manage(new class Gtk::VBox(false, 0));
   _tooltips.set_tip(*newauftrag_button, "neuer Auftrag: hier dr�cken, dann Kunde eingeben, dann Kundenauftrag eingeben, dann oberes OK anw�hlen", "");
   toolbar2->set_space_size(5);
   toolbar2->set_tooltips(true);
   label26->set_alignment(0, 0.5);
   label26->set_padding(0, 0);
   label26->set_justify(GTK_JUSTIFY_LEFT);
   label27->set_alignment(0, 0.5);
   label27->set_padding(0, 0);
   label27->set_justify(GTK_JUSTIFY_LEFT);
   label28->set_alignment(0, 0.5);
   label28->set_padding(0, 0);
   label28->set_justify(GTK_JUSTIFY_LEFT);
   toolbar4->set_tooltips(true);
   label31->set_alignment(0, 0.5);
   label31->set_padding(0, 0);
   label31->set_justify(GTK_JUSTIFY_LEFT);
   aufbemerkung_entry->set_editable(true);
   label30->set_alignment(0, 0.5);
   label30->set_padding(0, 0);
   label30->set_justify(GTK_JUSTIFY_LEFT);
   jahrgang_spinbutton->set_editable(true);
   jahrgang_spinbutton->set_numeric(true);
   label32->set_alignment(0, 0.5);
   label32->set_padding(0, 0);
   label32->set_justify(GTK_JUSTIFY_LEFT);
   aufrabatt_spinbutton->set_sensitive(false);
   aufrabatt_spinbutton->set_editable(true);
   aufrabatt_spinbutton->set_numeric(true);
   aufrabatt_spinbutton->set_digits(2);
   label34->set_alignment(0, 0.5);
   label34->set_padding(0, 0);
   label34->set_justify(GTK_JUSTIFY_LEFT);
   zahlart->set_sensitive(false);
   zahlart->set_flags(GTK_CAN_FOCUS);
   {  
      
      Gtk::Menu *_m(manage(new Gtk::Menu()));
      Gtk::MenuItem *_mi;
      _mi = manage(new Gtk::MenuItem("Rechnung"));
      _m->append(*_mi);
      _mi->show();
      _mi->set_user_data((gpointer)0);
      _mi = manage(new Gtk::MenuItem("Abbuchung"));
      _m->append(*_mi);
      _mi->show();
      _mi->set_user_data((gpointer)1);
      zahlart->set_menu(*_m);
      zahlart->set_history(0);
   }
   label37->set_alignment(0, 0.5);
   label37->set_padding(0, 0);
   label37->set_justify(GTK_JUSTIFY_LEFT);
   auftrag_ok->set_usize(102, -1);
   auftrag_ok->set_sensitive(false);
   auftrag_ok->set_flags(GTK_CAN_FOCUS);
   auftrag_abbruch->set_sensitive(false);
   auftrag_abbruch->set_flags(GTK_CAN_FOCUS);
   table5->set_border_width(5);
   table5->attach(*label26, 0, 1, 1, 2, GTK_FILL, 0, 0, 0);
   table5->attach(*label27, 0, 1, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*aufnr_scombo, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*youraufnr_scombo, 1, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*label28, 0, 1, 3, 4, GTK_FILL, 0, 0, 0);
   table5->attach(*toolbar4, 3, 4, 1, 3, 0, 0, 0, 0);
   table5->attach(*label31, 0, 1, 4, 5, GTK_FILL, 0, 0, 0);
   table5->attach(*aufbemerkung_entry, 1, 4, 4, 5, GTK_EXPAND|GTK_FILL, 0, 0, 0);
   table5->attach(*aufdatum_datewin, 2, 4, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*label30, 2, 3, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*jahrgang_spinbutton, 3, 4, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*label32, 4, 5, 0, 1, GTK_FILL, 0, 0, 0);
   table5->attach(*aufrabatt_spinbutton, 5, 6, 0, 1, GTK_EXPAND|GTK_FILL, 0, 0, 0);
   table5->attach(*zahlziel_datewin, 4, 6, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*label34, 4, 5, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*zahlart, 5, 6, 2, 3, GTK_FILL, 0, 0, 0);
   table5->attach(*label37, 4, 5, 3, 4, GTK_FILL, 0, 0, 0);
   table5->attach(*auftrag_ok, 4, 5, 4, 5, GTK_FILL, 0, 0, 0);
   table5->attach(*auftrag_abbruch, 5, 6, 4, 5, 0, 0, 0, 0);
   table5->attach(*kundenbox, 0, 4, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*bea_WWaehrung, 5, 6, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
   table5->attach(*WAufStat, 1, 2, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
   frame2->set_border_width(5);
   frame2->set_label_align(0, 0);
   frame2->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame2->add(*table5);
   kw_spinbutton->set_editable(true);
   kw_spinbutton->set_numeric(true);
   jahr_spinbutton->set_editable(true);
   jahr_spinbutton->set_numeric(true);
   hbox1->pack_start(*kw_spinbutton);
   hbox1->pack_start(*jahr_spinbutton);
   label23->set_alignment(0.5, 0.5);
   label23->set_padding(0, 0);
   label24->set_alignment(0.5, 0.5);
   label24->set_padding(0, 0);
   liefertermin->set_flags(GTK_CAN_FOCUS);
   liefertermin->set_show_tabs(true);
   liefertermin->set_tab_border(3);
   liefertermin->pages().push_back(Gtk::Notebook_Helpers::TabElem(*hbox1, *label23));
   liefertermin->pages().push_back(Gtk::Notebook_Helpers::TabElem(*liefdatum_datewin, *label24));
   showkal_button->set_usize(-1, 50);
   toolbar3->set_space_size(5);
   toolbar3->set_tooltips(true);
   hbox3->pack_start(*liefertermin);
   hbox3->pack_start(*toolbar3, false, false, 0);
   label25->set_alignment(0, 0.5);
   label25->set_padding(0, 0);
   label25->set_justify(GTK_JUSTIFY_LEFT);
   aufentry_abbruch->set_flags(GTK_CAN_FOCUS);
   aufentry_ok->set_flags(GTK_CAN_FOCUS);
   mengeeinheit->set_alignment(0, 0.5);
   mengeeinheit->set_padding(0, 0);
   mengeeinheit->set_justify(GTK_JUSTIFY_LEFT);
   label67->set_usize(40, -1);
   label67->set_alignment(0.89, 0);
   label67->set_padding(0, 0);
   label67->set_justify(GTK_JUSTIFY_RIGHT);
   stkmtr_spinbutton->set_usize(70, -1);
   stkmtr_spinbutton->set_editable(true);
   stkmtr_spinbutton->set_numeric(true);
   label29->set_usize(40, -1);
   label29->set_alignment(0.1, 0.5);
   label29->set_padding(0, 0);
   label29->set_justify(GTK_JUSTIFY_LEFT);
   rabattentry_spinbutton->set_usize(70, -1);
   rabattentry_spinbutton->set_editable(true);
   rabattentry_spinbutton->set_numeric(true);
   rabattentry_spinbutton->set_digits(2);
   WPreis->set_usize(110, -1);
   label73->set_alignment(0, 0.5);
   label73->set_padding(0, 0);
   label73->set_justify(GTK_JUSTIFY_LEFT);
   table_auftragseintraege->set_border_width(5);
   table_auftragseintraege->attach(*hbox3, 4, 5, 2, 4, GTK_FILL, GTK_FILL, 0, 0);
   table_auftragseintraege->attach(*label25, 8, 9, 2, 3, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*aufentry_abbruch, 6, 7, 2, 3, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*aufentry_ok, 6, 7, 3, 4, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*artikelbox, 0, 5, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 5);
   table_auftragseintraege->attach(*mengeeinheit, 3, 4, 3, 4, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*label67, 1, 2, 3, 4, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*stkmtr_spinbutton, 2, 3, 3, 4, 0, 0, 0, 0);
   table_auftragseintraege->attach(*WAufEntryStat, 8, 9, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
   table_auftragseintraege->attach(*label29, 1, 2, 2, 3, GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*rabattentry_spinbutton, 2, 3, 2, 3, GTK_EXPAND|GTK_FILL, 0, 0, 0);
   table_auftragseintraege->attach(*WPreis, 0, 1, 2, 4, GTK_EXPAND|GTK_FILL, GTK_EXPAND|GTK_FILL, 0, 0);
   table_auftragseintraege->attach(*label73, 3, 4, 2, 3, GTK_FILL, 0, 0, 0);
   frame3->set_border_width(5);
   frame3->set_label_align(0, 0);
   frame3->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame3->add(*table_auftragseintraege);
   auftrag_clist->set_column_title(0, "St�ck/Meter");
   auftrag_clist->set_column_justification(0, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(0, 80);
   auftrag_clist->set_column_title(1, "Artikel");
   auftrag_clist->set_column_width(1, 80);
   auftrag_clist->set_column_title(2, "Menge");
   auftrag_clist->set_column_justification(2, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(2, 80);
   auftrag_clist->set_column_title(3, "Rest");
   auftrag_clist->set_column_justification(3, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(3, 80);
   auftrag_clist->set_column_title(4, "Preis");
   auftrag_clist->set_column_justification(4, GTK_JUSTIFY_RIGHT);
   auftrag_clist->set_column_width(4, 80);
   auftrag_clist->set_column_title(5, "Lief.Termin");
   auftrag_clist->set_column_width(5, 80);
   auftrag_clist->set_column_title(6, "Stand");
   auftrag_clist->set_column_width(6, 80);
   auftrag_clist->set_column_title(7, "�nderung");
   auftrag_clist->set_column_width(7, 80);
   auftrag_clist->column_titles_show();
   scrolledwindow_auftraege->set_border_width(5);
   scrolledwindow_auftraege->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow_auftraege->add(*auftrag_clist);
   vbox4->pack_start(*menubar1, false, false, 0);
   vbox4->pack_start(*toolbar2, false, false, 0);
   vbox4->pack_start(*frame2, false, true, 0);
   vbox4->pack_start(*frame3, false, false, 0);
   vbox4->pack_start(*scrolledwindow_auftraege);
   auftrag_bearbeiten->set_usize(800, 600);
   auftrag_bearbeiten->set_title("Auftragserfassung");
   auftrag_bearbeiten->set_policy(true, true, false);
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
   label30->show();
   jahrgang_spinbutton->show();
   label32->show();
   aufrabatt_spinbutton->show();
   zahlziel_datewin->show();
   label34->show();
   zahlart->show();
   label37->show();
   auftrag_ok->show();
   auftrag_abbruch->show();
   kundenbox->show();
   bea_WWaehrung->show();
   WAufStat->show();
   table5->show();
   frame2->show();
   kw_spinbutton->show();
   jahr_spinbutton->show();
   hbox1->show();
   label23->show();
   liefdatum_datewin->show();
   label24->show();
   liefertermin->show();
   toolbar3->show();
   hbox3->show();
   label25->show();
   aufentry_abbruch->show();
   aufentry_ok->show();
   artikelbox->show();
   mengeeinheit->show();
   label67->show();
   stkmtr_spinbutton->show();
   WAufEntryStat->show();
   label29->show();
   rabattentry_spinbutton->show();
   WPreis->show();
   label73->show();
   table_auftragseintraege->show();
   frame3->show();
   auftrag_clist->show();
   scrolledwindow_auftraege->show();
   vbox4->show();
   auftrag_bearbeiten->show();
   schlie_en1->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_backtomain_button_clicked));
   neu1->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_newauftrag_button_clicked));
   allesleeren1->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_clear_all));
   splitten1->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_splitten));
   backtomain_button->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_backtomain_button_clicked));
   newauftrag_button->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_newauftrag_button_clicked));
   button8->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_clear_all));
   button_preview->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_button_preview_clicked));
   button_drucken->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_button_drucken_clicked));
   aufnr_scombo->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_aufnrscombo_activate));
   aufnr_scombo->search.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_aufnrscombo_search));
   youraufnr_scombo->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_youraufnrscombo_activate));
   youraufnr_scombo->search.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_youraufnrscombo_search));
   aufbemerkung_entry->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_aufbemerkung_activate));
   jahrgang_spinbutton->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_jahrgang_spinbutton_activate));
   zahlziel_datewin->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_zahlzieldatewin_activate));
   auftrag_ok->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_auftrag_ok_clicked));
   auftrag_abbruch->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_auftrag_abbruch_clicked));
   kundenbox->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_kunden_activate));
   bea_WWaehrung->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::waehrung_geaendert));
   WAufStat->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::auftragstatus_geaendert));
   kw_spinbutton->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_kw_spinbutton_activate));
   jahr_spinbutton->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_jahr_spinbutton_activate));
   liefdatum_datewin->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_lieferdatum_activate));
   showkal_button->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_showkal_button_clicked));
   aufentry_abbruch->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_aufentry_abbruch_clicked));
   aufentry_ok->clicked.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_aufentry_ok_clicked));
   artikelbox->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::onSelArtikel));
   stkmtr_spinbutton->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_stkmtr_spinbutton_activate));
   WAufEntryStat->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_aufentrystat_optionmenu_clicked));
   rabattentry_spinbutton->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_rabattentry_spinbutton_activate));
   WPreis->activate.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_activate_wpreis));
   auftrag_clist->select_row.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_auftrag_clist_select_row));
   auftrag_clist->unselect_row.connect(SigC::slot(static_cast<class auftrag_bearbeiten*>(this), &auftrag_bearbeiten::on_auftrag_clist_unselect_row));
}

auftrag_bearbeiten_glade::~auftrag_bearbeiten_glade()
{  delete aufnr_scombo;
   delete youraufnr_scombo;
   delete aufdatum_datewin;
   delete zahlziel_datewin;
   delete kundenbox;
   delete bea_WWaehrung;
   delete WAufStat;
   delete liefdatum_datewin;
   delete artikelbox;
   delete WAufEntryStat;
   delete WPreis;
}
