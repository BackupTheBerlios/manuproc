// generated 2001/6/25 10:38:36 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/petig/cvs/jacek/pps/auftrag.glade
// for gtk 1.2.8 and gtkmm 1.2.5
//
// Please modify the corresponding derived classes in ./src/auftrag_rechnung.cc

#include "config.h"
#include "auftrag_rechnung.hh"
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
#include <gtk--/menuitem.h>
#include <gtk--/adjustment.h>
#include <gtk--/table.h>
#include <gtk--/frame.h>
#include <gtk--/viewport.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/box.h>

auftrag_rechnung_glade::auftrag_rechnung_glade(
) : Gtk::Window(GTK_WINDOW_TOPLEVEL)
{   auftrag_rechnung = this;
   
   Gtk::MenuBar *menubar3 = manage(new class Gtk::MenuBar());
   Gtk::MenuItem *pixmapmenuitem1;
   Gtk::MenuItem *menuitem1;
   {   static GnomeUIInfo menuitem1_menu_uiinfo[] = 
      {        GNOMEUIINFO_MENU_CLOSE_ITEM(0, 0), 
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo menubar3_uiinfo[] = 
      {        GNOMEUIINFO_MENU_FILE_TREE(menuitem1_menu_uiinfo),
	      GNOMEUIINFO_END 
      };
      gnome_app_fill_menu(GTK_MENU_SHELL(menubar3->gtkobj()), menubar3_uiinfo, NULL, true, 0);
      pixmapmenuitem1 = Gtk::wrap(GTK_MENU_ITEM(menuitem1_menu_uiinfo[0].widget));
   }
   Gtk::Toolbar *toolbar7 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::Button *button23 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar7->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Schlie�en", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar7->gtkobj()), GNOME_STOCK_PIXMAP_CLOSE)), 0, 0));
   Gtk::Button *button24 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar7->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Neu", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar7->gtkobj()), GNOME_STOCK_PIXMAP_NEW)), 0, 0));
   Gtk::Button *button25 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar7->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Speichern", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar7->gtkobj()), GNOME_STOCK_PIXMAP_SAVE)), 0, 0));
   Gtk::Button *button26 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar7->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Anzeigen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar7->gtkobj()), GNOME_STOCK_PIXMAP_SEARCH)), 0, 0));
   Gtk::Button *button27 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar7->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Drucken", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar7->gtkobj()), GNOME_STOCK_PIXMAP_PRINT)), 0, 0));
   toolbar7->tools().push_back(Gtk::Toolbar_Helpers::Space());
   rngentry_del = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar7->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Rng.Zeile\n"
		"l�schen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar7->gtkobj()), GNOME_STOCK_PIXMAP_CLEAR)), 0, 0));
   
   Gtk::Button *button28 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar7->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Preise\n"
		"erg�nzen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar7->gtkobj()), GNOME_STOCK_PIXMAP_SEARCH)), 0, 0));
   Gtk::Button *button29 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar7->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Preise\n"
		"�berschreiben", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar7->gtkobj()), GNOME_STOCK_PIXMAP_SRCHRPL)), 0, 0));
   Gtk::Label *label60 = manage(new class Gtk::Label("Rg. Nr."));
   Gtk::Label *label61 = manage(new class Gtk::Label("Rg. Datum"));
   rngdatum = new class Datewin();
   rngnr = new class SearchComboContent<int>();
   lieferkunde = new class KundenBox();
   
   Gtk::Label *label72 = manage(new class Gtk::Label("Kunde"));
   Gtk::Label *label66 = manage(new class Gtk::Label("W�hrung"));
   rabatt_typ = manage(new class Gtk::OptionMenu());
   rng_waehrung = manage(new class Gtk::OptionMenu());
   
   Gtk::Adjustment *rabatt_wert_adj = manage(new class Gtk::Adjustment(0, 0, 99, 1, 10, 10));
   rabatt_wert = manage(new class Gtk::SpinButton(*rabatt_wert_adj, 1, 2));
   
   Gtk::Table *table10 = manage(new class Gtk::Table(3, 5, false));
   Gtk::Frame *frame7 = manage(new class Gtk::Frame("Rechnung"));
   rechnung_liste = new class Rg_TCList(6, 3);
   
   Gtk::Viewport *viewport2 = manage(new class Gtk::Viewport());
   Gtk::ScrolledWindow *scrolledwindow6 = manage(new class Gtk::ScrolledWindow());
   Gtk::Frame *frame8 = manage(new class Gtk::Frame("Rechnungsdaten"));
   Gtk::Table *table12 = manage(new class Gtk::Table(3, 3, false));
   Gtk::HBox *hbox5 = manage(new class Gtk::HBox(false, 0));
   Gtk::Label *label63 = manage(new class Gtk::Label("Liefer.Nr."));
   lieferscheinnr = manage(new class Gtk::Entry());
   
   Gtk::Toolbar *toolbar8 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   lief_uebernehmen = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar8->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Lieferschein �bernehmen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar8->gtkobj()), GNOME_STOCK_PIXMAP_ATTACH)), 0, 0));
   
   Gtk::Label *label71 = manage(new class Gtk::Label("Liefer. Datum"));
   lieferscheindatum = new class Datewin();
   
   Gtk::Table *table11 = manage(new class Gtk::Table(2, 3, false));
   offene_lieferscheine = new class OffLief_TCList(3, 3);
   
   Gtk::Viewport *viewport5 = manage(new class Gtk::Viewport());
   Gtk::ScrolledWindow *scrolledwindow7 = manage(new class Gtk::ScrolledWindow());
   vbox_n_b_lieferscheine = manage(new class Gtk::VBox(false, 0));
   
   Gtk::Frame *frame9 = manage(new class Gtk::Frame("nicht berechnete Lieferscheine"));
   Gtk::VBox *vbox7 = manage(new class Gtk::VBox(false, 0));
   button25->set_sensitive(false);
   rngentry_del->set_sensitive(false);
   toolbar7->set_space_size(5);
   toolbar7->set_tooltips(true);
   label60->set_alignment(0, 0.5);
   label60->set_padding(0, 0);
   label61->set_alignment(0, 0.5);
   label61->set_padding(0, 0);
   rngnr->set_usize(100, -1);
   label72->set_alignment(0, 0.5);
   label72->set_padding(0, 0);
   label66->set_alignment(0.99, 0.5);
   label66->set_padding(0, 0);
   _tooltips.set_tip(*rabatt_typ, "Rabatt/Zuschlag auf alle Rechnungsposten", "");
   {   
      
      Gtk::Menu *_m(manage(new Gtk::Menu()));
      Gtk::MenuItem *_mi;
      _mi = manage(new Gtk::MenuItem("Rabatt"));
      _m->append(*_mi);
      _mi->show();
      _mi->set_user_data((gpointer)0);
      _mi = manage(new Gtk::MenuItem("Zuschlag"));
      _m->append(*_mi);
      _mi->show();
      _mi->set_user_data((gpointer)1);
      rabatt_typ->set_menu(*_m);
      rabatt_typ->set_history(0);
   }
   {   
      
      Gtk::Menu *_m(manage(new Gtk::Menu()));
      Gtk::MenuItem *_mi;
      _mi = manage(new Gtk::MenuItem("DM"));
      _m->append(*_mi);
      _mi->show();
      _mi->set_user_data((gpointer)0);
      _mi = manage(new Gtk::MenuItem("Euro"));
      _m->append(*_mi);
      _mi->show();
      _mi->set_user_data((gpointer)1);
      rng_waehrung->set_menu(*_m);
      rng_waehrung->set_history(0);
   }
   rabatt_wert->set_usize(60, -1);
   _tooltips.set_tip(*rabatt_wert, "Rabatt/Zuschlag auf alle Rechnungsposten", "");
   rabatt_wert->set_editable(true);
   rabatt_wert->set_numeric(true);
   rabatt_wert->set_digits(2);
   table10->attach(*label60, 0, 1, 1, 2, GTK_FILL, 0, 0, 0);
   table10->attach(*label61, 0, 1, 2, 3, GTK_FILL, 0, 0, 0);
   table10->attach(*rngdatum, 1, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
   table10->attach(*rngnr, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table10->attach(*lieferkunde, 1, 5, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table10->attach(*label72, 0, 1, 0, 1, GTK_FILL, 0, 0, 0);
   table10->attach(*label66, 3, 4, 1, 2, GTK_FILL, 0, 0, 0);
   table10->attach(*rabatt_typ, 3, 4, 2, 3, 0, 0, 0, 0);
   table10->attach(*rng_waehrung, 4, 5, 1, 2, 0, 0, 0, 0);
   table10->attach(*rabatt_wert, 4, 5, 2, 3, 0, 0, 0, 0);
   frame7->set_border_width(5);
   frame7->set_label_align(0, 0);
   frame7->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame7->add(*table10);
   viewport2->set_shadow_type(GTK_SHADOW_NONE);
   viewport2->add(*rechnung_liste);
   scrolledwindow6->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow6->add(*viewport2);
   frame8->set_border_width(5);
   frame8->set_label_align(0, 0);
   frame8->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame8->add(*scrolledwindow6);
   hbox5->pack_start(*frame8);
   hbox5->pack_start(*table12);
   label63->set_alignment(0, 0.5);
   label63->set_padding(0, 0);
   lieferscheinnr->set_usize(100, -1);
   lieferscheinnr->set_editable(false);
   lief_uebernehmen->set_sensitive(false);
   _tooltips.set_tip(*lief_uebernehmen, "Kompletten Auftrag in Lieferschein �bernehmen", "");
   toolbar8->set_space_size(5);
   toolbar8->set_tooltips(true);
   label71->set_alignment(0, 0.5);
   label71->set_padding(0, 0);
   table11->set_border_width(5);
   table11->set_col_spacings(1);
   table11->attach(*label63, 0, 1, 0, 1, GTK_FILL, 0, 0, 0);
   table11->attach(*lieferscheinnr, 0, 1, 1, 2, GTK_FILL, 0, 0, 0);
   table11->attach(*toolbar8, 2, 3, 0, 2, GTK_FILL, 0, 0, 0);
   table11->attach(*label71, 1, 2, 0, 1, GTK_FILL, 0, 0, 0);
   table11->attach(*lieferscheindatum, 1, 2, 1, 2, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   viewport5->set_shadow_type(GTK_SHADOW_NONE);
   viewport5->add(*offene_lieferscheine);
   scrolledwindow7->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow7->add(*viewport5);
   vbox_n_b_lieferscheine->pack_start(*table11, false, true, 0);
   vbox_n_b_lieferscheine->pack_start(*scrolledwindow7);
   frame9->set_border_width(5);
   frame9->set_label_align(0, 0);
   frame9->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame9->add(*vbox_n_b_lieferscheine);
   vbox7->pack_start(*menubar3, false, false, 0);
   vbox7->pack_start(*toolbar7, false, false, 0);
   vbox7->pack_start(*frame7, false, false, 0);
   vbox7->pack_start(*hbox5);
   vbox7->pack_start(*frame9);
   auftrag_rechnung->set_usize(800, 600);
   auftrag_rechnung->set_title("Rechnungen");
   auftrag_rechnung->set_policy(true, true, false);
   auftrag_rechnung->add(*vbox7);
   menubar3->show();
   toolbar7->show();
   label60->show();
   label61->show();
   rngdatum->show();
   rngnr->show();
   lieferkunde->show();
   label72->show();
   label66->show();
   rabatt_typ->show();
   rng_waehrung->show();
   rabatt_wert->show();
   table10->show();
   frame7->show();
   rechnung_liste->show();
   viewport2->show();
   scrolledwindow6->show();
   frame8->show();
   hbox5->show();
   label63->show();
   lieferscheinnr->show();
   toolbar8->show();
   label71->show();
   lieferscheindatum->show();
   table11->show();
   offene_lieferscheine->show();
   viewport5->show();
   scrolledwindow7->show();
   vbox_n_b_lieferscheine->show();
   frame9->show();
   vbox7->show();
   auftrag_rechnung->show();
   pixmapmenuitem1->activate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rng_close));
   button23->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rng_close));
   button24->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rng_neu));
   button25->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rng_save));
   button26->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rng_preview));
   button27->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rng_print));
   rngentry_del->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::rngzeile_delete));
   button28->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::Preis_ergaenzen));
   button29->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::Preis_setzen));
   rngdatum->activate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rngdate_activate));
   rngnr->activate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rngnr_activate));
   rngnr->search.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_rngnr_search));
   lieferkunde->activate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_lieferkunde_activate));
   rabatt_typ->get_menu()->deactivate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::rabatt_geaendert));
   rng_waehrung->get_menu()->deactivate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::waehrung_geaendert));
   rabatt_wert->activate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::rabatt_geaendert));
   rechnung_liste->select_row.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_selectrow_rechnung));
   rechnung_liste->unselect_row.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_unselectrow_rechnung));
   lief_uebernehmen->clicked.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::lieferschein_uebernehmen));
   offene_lieferscheine->select_row.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_selectrow_offlief));
   offene_lieferscheine->unselect_row.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::on_unselectrow_offlief));
}

auftrag_rechnung_glade::~auftrag_rechnung_glade()
{   delete rngdatum;
   delete rngnr;
   delete lieferkunde;
   delete rechnung_liste;
   delete lieferscheindatum;
   delete offene_lieferscheine;
}
