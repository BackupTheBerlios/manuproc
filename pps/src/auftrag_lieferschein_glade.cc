// generated 2002/6/21 11:41:50 CEST by jacek@mabella.mabella.de
// using glademm V0.6.4c_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- auftrag.glade
// for gtk 1.2.10 and gtkmm 1.2.5
//
// Please modify the corresponding derived classes in ./src/auftrag_lieferschein.cc

#include "config.h"
#include "auftrag_lieferschein.hh"
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
#include <gtk--/frame.h>
#include <gtk--/adjustment.h>
#include <gtk--/table.h>
#include <gtk--/viewport.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/label.h>
#include <gtk--/box.h>

auftrag_lieferschein_glade::auftrag_lieferschein_glade(
) : Gtk::Window(GTK_WINDOW_TOPLEVEL)
{  auftrag_lieferschein = this;
   
   Gtk::MenuBar *menubar2 = manage(new class Gtk::MenuBar());
   Gtk::MenuItem *schlie_en2;
   Gtk::MenuItem *datei2;
   {  static GnomeUIInfo datei2_menu_uiinfo[] = 
      {       GNOMEUIINFO_MENU_CLOSE_ITEM(0, 0), 
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo menubar2_uiinfo[] = 
      {       GNOMEUIINFO_MENU_FILE_TREE(datei2_menu_uiinfo),
	      GNOMEUIINFO_END 
      };
      gnome_app_fill_menu(GTK_MENU_SHELL(menubar2->gtkobj()), menubar2_uiinfo, NULL, true, 0);
      schlie_en2 = Gtk::wrap(GTK_MENU_ITEM(datei2_menu_uiinfo[0].widget));
   }
   Gtk::Toolbar *toolbar5 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::Button *button9 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Schlie�en", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_CLOSE)), 0, 0));
   Gtk::Button *button10 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Neu", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_NEW)), 0, 0));
   Gtk::Button *button13 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Speichern", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_SAVE)), 0, 0));
   Gtk::Button *button14 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Anzeigen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_SEARCH)), 0, 0));
   Gtk::Button *button15 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Drucken", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_PRINT)), 0, 0));
   lieferzeile_delete = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Lief.Zeile\n"
		"l�schen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_CLEAR)), 0, 0));
   toolbar5->tools().push_back(Gtk::Toolbar_Helpers::Space());
   
   Gtk::Button *button_liste = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Liste", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_BOOK_OPEN)), 0, 0));
   liefernr = new class SearchComboContent<int>();
   
   Gtk::Frame *frame16 = manage(new class Gtk::Frame("Lief. Nr."));
   liefdate = new class Datewin();
   
   Gtk::Frame *frame17 = manage(new class Gtk::Frame("Lief. Datum"));
   Gtk::Adjustment *spinbutton_paeckchen_adj = manage(new class Gtk::Adjustment(1, 0, 999, 1, 10, 10));
   spinbutton_paeckchen = manage(new class Gtk::SpinButton(*spinbutton_paeckchen_adj, 1, 0));
   
   Gtk::Frame *frame19 = manage(new class Gtk::Frame("P�ckchen"));
   Gtk::Adjustment *spinbutton_pakete_adj = manage(new class Gtk::Adjustment(1, 0, 999, 1, 10, 10));
   spinbutton_pakete = manage(new class Gtk::SpinButton(*spinbutton_pakete_adj, 1, 0));
   
   Gtk::Frame *frame20 = manage(new class Gtk::Frame("Pakete"));
   Gtk::Adjustment *spinbutton_brutto_adj = manage(new class Gtk::Adjustment(0, 0, 999.9, 1, 10, 10));
   spinbutton_brutto = manage(new class Gtk::SpinButton(*spinbutton_brutto_adj, 1, 1));
   
   Gtk::Frame *frame21 = manage(new class Gtk::Frame("Brutto (kg)"));
   Gtk::Adjustment *spinbutton_netto_adj = manage(new class Gtk::Adjustment(0, 0, 999.9, 1, 10, 10));
   spinbutton_netto = manage(new class Gtk::SpinButton(*spinbutton_netto_adj, 1, 1));
   
   Gtk::Frame *frame22 = manage(new class Gtk::Frame("Netto (kg)"));
   entry_dpdnr = manage(new class Gtk::Entry());
   
   Gtk::Frame *frame23 = manage(new class Gtk::Frame("DPD Nummer"));
   Gtk::Button *button_erledigt = manage(new class Gtk::Button("Fertig"));
   table_mabella = manage(new class Gtk::Table(1, 6, false));
   rngnr = manage(new class Gtk::Entry());
   
   Gtk::Frame *frame18 = manage(new class Gtk::Frame("Rechnungsnr."));
   liefer_kunde = new class KundenBox2();
   
   Gtk::Table *table8 = manage(new class Gtk::Table(2, 4, false));
   Gtk::Frame *frame4 = manage(new class Gtk::Frame("Lieferschein"));
   tree_daten = new class SimpleTree(5, 4);
   
   Gtk::Viewport *viewport1 = manage(new class Gtk::Viewport());
   Gtk::ScrolledWindow *scrolledwindow4 = manage(new class Gtk::ScrolledWindow());
   Gtk::Frame *frame6 = manage(new class Gtk::Frame("Lieferscheindaten"));
   Gtk::Label *label50 = manage(new class Gtk::Label("Anzahl"));
   Gtk::Adjustment *anzahl_adj = manage(new class Gtk::Adjustment(1, 0, 1e+06, 1, 10, 10));
   anzahl = manage(new class Gtk::SpinButton(*anzahl_adj, 1, 0));
   label_kundenauftrag = manage(new class Gtk::Label("Kundenauftrag"));
   auftragnr = manage(new class Gtk::Entry());
   artikelbox = new class ArtikelBox(1);
   
   Gtk::Label *label59 = manage(new class Gtk::Label("Palette"));
   Gtk::Toolbar *toolbar6 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   button_zeile_uebernehmen = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar6->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Zeile", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar6->gtkobj()), GNOME_STOCK_PIXMAP_ADD)), 0, 0));
   button_kompletter_auftrag = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar6->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Auftrag", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar6->gtkobj()), GNOME_STOCK_PIXMAP_ATTACH)), 0, 0));
   
   Gtk::Adjustment *Palette_adj = manage(new class Gtk::Adjustment(1, 0, 999, 1, 10, 10));
   Palette = manage(new class Gtk::SpinButton(*Palette_adj, 1, 0));
   
   Gtk::Adjustment *liefermenge_adj = manage(new class Gtk::Adjustment(1, 0, 1e+06, 1, 10, 10));
   liefermenge = manage(new class Gtk::SpinButton(*liefermenge_adj, 1, 3));
   menge_einheit = manage(new class Gtk::Label("m"));
   
   Gtk::HBox *hbox7 = manage(new class Gtk::HBox(false, 0));
   label_menge = manage(new class Gtk::Label("Menge"));
   
   Gtk::Toolbar *toolbar12 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   button_zeile_modifizieren = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar12->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "�ndern", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar12->gtkobj()), GNOME_STOCK_PIXMAP_CONVERT)), 0, 0));
   
   Gtk::Table *table9 = manage(new class Gtk::Table(2, 7, false));
   tree_offen = new class SimpleTree(5, 3);
   
   Gtk::Viewport *viewport4 = manage(new class Gtk::Viewport());
   Gtk::ScrolledWindow *scrolledwindow5 = manage(new class Gtk::ScrolledWindow());
   vbox_eingabe = manage(new class Gtk::VBox(false, 0));
   
   Gtk::Frame *frame5 = manage(new class Gtk::Frame("Eingabe"));
   Gtk::VBox *vbox5 = manage(new class Gtk::VBox(false, 0));
   button13->set_sensitive(false);
   lieferzeile_delete->set_sensitive(false);
   toolbar5->set_space_size(5);
   toolbar5->set_tooltips(true);
   liefernr->set_usize(100,-1);
   frame16->set_label_align(0, 0);
   frame16->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame16->add(*liefernr);
   frame17->set_label_align(0, 0);
   frame17->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame17->add(*liefdate);
   spinbutton_paeckchen->set_editable(true);
   frame19->set_label_align(0, 0);
   frame19->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame19->add(*spinbutton_paeckchen);
   spinbutton_pakete->set_editable(true);
   frame20->set_label_align(0, 0);
   frame20->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame20->add(*spinbutton_pakete);
   spinbutton_brutto->set_editable(true);
   spinbutton_brutto->set_digits(1);
   frame21->set_label_align(0, 0);
   frame21->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame21->add(*spinbutton_brutto);
   spinbutton_netto->set_editable(true);
   spinbutton_netto->set_digits(1);
   frame22->set_label_align(0, 0);
   frame22->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame22->add(*spinbutton_netto);
   entry_dpdnr->set_sensitive(false);
   entry_dpdnr->set_editable(false);
   frame23->set_label_align(0, 0);
   frame23->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame23->add(*entry_dpdnr);
   button_erledigt->set_flags(GTK_CAN_FOCUS);
   table_mabella->attach(*frame19, 0, 1, 0, 1, GTK_EXPAND|GTK_FILL, GTK_EXPAND|GTK_FILL, 0, 0);
   table_mabella->attach(*frame20, 1, 2, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table_mabella->attach(*frame21, 2, 3, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table_mabella->attach(*frame22, 3, 4, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table_mabella->attach(*frame23, 4, 5, 0, 1, 0, 0, 0, 0);
   table_mabella->attach(*button_erledigt, 5, 6, 0, 1, GTK_FILL, 0, 0, 0);
   rngnr->set_usize(80,-1);
   rngnr->set_editable(false);
   frame18->set_label_align(0, 0);
   frame18->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame18->add(*rngnr);
   table8->attach(*frame16, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table8->attach(*frame17, 3, 4, 0, 2, GTK_FILL, GTK_FILL, 0, 0);
   table8->attach(*table_mabella, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table8->attach(*frame18, 2, 3, 1, 2, 0, GTK_FILL, 0, 0);
   table8->attach(*liefer_kunde, 0, 3, 0, 1, GTK_FILL, GTK_EXPAND|GTK_FILL, 0, 0);
   frame4->set_border_width(5);
   frame4->set_label_align(0, 0);
   frame4->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame4->add(*table8);
   viewport1->set_shadow_type(GTK_SHADOW_NONE);
   viewport1->add(*tree_daten);
   scrolledwindow4->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow4->add(*viewport1);
   frame6->set_border_width(5);
   frame6->set_label_align(0, 0);
   frame6->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame6->add(*scrolledwindow4);
   label50->set_alignment(0, 0.5);
   label50->set_padding(0, 0);
   label50->set_justify(GTK_JUSTIFY_LEFT);
   anzahl->set_usize(60,-1);
   anzahl->set_editable(true);
   anzahl->set_numeric(true);
   label_kundenauftrag->set_alignment(0, 0.5);
   label_kundenauftrag->set_padding(0, 0);
   label_kundenauftrag->set_justify(GTK_JUSTIFY_LEFT);
   auftragnr->set_usize(100,-1);
   auftragnr->set_editable(false);
   label59->set_alignment(0, 0.5);
   label59->set_padding(0, 0);
   _tooltips.set_tip(*button_zeile_uebernehmen, "Ausgew�hlte Zeile in Lieferschein �bernehmen", "");
   _tooltips.set_tip(*button_kompletter_auftrag, "Kompletten Auftrag in Lieferschein �bernehmen", "");
   toolbar6->set_space_size(5);
   toolbar6->set_tooltips(true);
   Palette->set_usize(60,-1);
   Palette->set_editable(true);
   Palette->set_numeric(true);
   liefermenge->set_usize(100,-1);
   liefermenge->set_editable(true);
   liefermenge->set_numeric(true);
   liefermenge->set_digits(3);
   menge_einheit->set_alignment(0.5, 0.5);
   menge_einheit->set_padding(0, 0);
   hbox7->pack_start(*liefermenge, false, false, 0);
   hbox7->pack_start(*menge_einheit, false, false, 0);
   label_menge->set_alignment(0, 0.5);
   label_menge->set_padding(0, 0);
   label_menge->set_justify(GTK_JUSTIFY_LEFT);
   button_zeile_modifizieren->set_sensitive(false);
   _tooltips.set_tip(*button_zeile_modifizieren, "Zeile modifiezieren, nachdem Anzahl, Menge und/oder Palette ge�ndert worden sind.", "");
   toolbar12->set_space_size(5);
   toolbar12->set_tooltips(true);
   table9->set_border_width(5);
   table9->set_col_spacings(1);
   table9->attach(*label50, 1, 2, 0, 1, GTK_FILL, 0, 0, 0);
   table9->attach(*anzahl, 1, 2, 1, 2, GTK_FILL, 0, 0, 0);
   table9->attach(*label_kundenauftrag, 4, 5, 0, 1, GTK_FILL, 0, 0, 0);
   table9->attach(*auftragnr, 4, 5, 1, 2, GTK_FILL, 0, 0, 0);
   table9->attach(*artikelbox, 2, 3, 0, 2, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table9->attach(*label59, 5, 6, 0, 1, GTK_FILL, 0, 0, 0);
   table9->attach(*toolbar6, 6, 7, 0, 2, GTK_FILL, 0, 0, 0);
   table9->attach(*Palette, 5, 6, 1, 2, GTK_FILL, 0, 0, 0);
   table9->attach(*hbox7, 3, 4, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table9->attach(*label_menge, 3, 4, 0, 1, GTK_FILL, 0, 0, 0);
   table9->attach(*toolbar12, 0, 1, 0, 2, GTK_FILL, 0, 0, 0);
   viewport4->set_shadow_type(GTK_SHADOW_NONE);
   viewport4->add(*tree_offen);
   scrolledwindow5->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow5->add(*viewport4);
   vbox_eingabe->pack_start(*table9, false, true, 0);
   vbox_eingabe->pack_start(*scrolledwindow5);
   frame5->set_border_width(5);
   frame5->set_label_align(0.12, 0);
   frame5->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame5->add(*vbox_eingabe);
   vbox5->pack_start(*menubar2, false, false, 0);
   vbox5->pack_start(*toolbar5, false, false, 0);
   vbox5->pack_start(*frame4, false, false, 0);
   vbox5->pack_start(*frame6);
   vbox5->pack_start(*frame5);
   auftrag_lieferschein->set_usize(800,600);
   auftrag_lieferschein->set_title("Lieferscheine");
   auftrag_lieferschein->set_modal(false);
   auftrag_lieferschein->set_position(GTK_WIN_POS_NONE);
   auftrag_lieferschein->set_policy(true, true, false);
   auftrag_lieferschein->add(*vbox5);
   menubar2->show();
   toolbar5->show();
   liefernr->show();
   frame16->show();
   liefdate->show();
   frame17->show();
   spinbutton_paeckchen->show();
   frame19->show();
   spinbutton_pakete->show();
   frame20->show();
   spinbutton_brutto->show();
   frame21->show();
   spinbutton_netto->show();
   frame22->show();
   entry_dpdnr->show();
   frame23->show();
   button_erledigt->show();
   table_mabella->show();
   rngnr->show();
   frame18->show();
   liefer_kunde->show();
   table8->show();
   frame4->show();
   tree_daten->show();
   viewport1->show();
   scrolledwindow4->show();
   frame6->show();
   label50->show();
   anzahl->show();
   label_kundenauftrag->show();
   auftragnr->show();
   artikelbox->show();
   label59->show();
   toolbar6->show();
   Palette->show();
   liefermenge->show();
   menge_einheit->show();
   hbox7->show();
   label_menge->show();
   toolbar12->show();
   table9->show();
   tree_offen->show();
   viewport4->show();
   scrolledwindow5->show();
   vbox_eingabe->show();
   frame5->show();
   vbox5->show();
   auftrag_lieferschein->show();
   schlie_en2->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefer_close));
   button9->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefer_close));
   button10->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefer_neu));
   button13->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_lief_save));
   button14->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_lief_preview));
   button15->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_lief_print));
   lieferzeile_delete->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::liefzeile_delete));
   button_liste->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_button_liste_clicked));
   liefernr->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefnr_activate));
   liefernr->search.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefnr_search));
   liefdate->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefdate_activate));
   spinbutton_paeckchen->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_spinbutton_paeckchen_activate));
   spinbutton_paeckchen->focus_out_event.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_spinbutton_paeckchen_focus_out_event));
   spinbutton_pakete->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_spinbutton_pakete_activate));
   spinbutton_pakete->focus_out_event.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_spinbutton_pakete_focus_out_event));
   spinbutton_brutto->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_spinbutton_brutto_activate));
   spinbutton_brutto->focus_out_event.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_spinbutton_brutto_focus_out_event));
   spinbutton_netto->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_spinbutton_netto_activate));
   spinbutton_netto->focus_out_event.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_spinbutton_netto_focus_out_event));
   button_erledigt->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_button_erledigt_clicked));
   liefer_kunde->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_lieferkunde_activate));
   tree_daten->leaf_selected.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_daten_leaf_selected));
   tree_daten->unselect_row.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_daten_unselect_row));
   anzahl->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefermenge_activate));
   auftragnr->activate.connect(Palette->grab_focus.slot());
   artikelbox->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_artikelbox_activate));
   button_zeile_uebernehmen->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_newlieferentry_ok));
   button_kompletter_auftrag->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_newlieferentryall_ok));
   Palette->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_Palette_activate));
   liefermenge->activate.connect(auftragnr->grab_focus.slot());
   button_zeile_modifizieren->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_button_zeile_modifizieren_clicked));
   tree_offen->leaf_selected.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_offen_leaf_selected));
   tree_offen->unselect_row.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_unselectrow_offauf));
}

auftrag_lieferschein_glade::~auftrag_lieferschein_glade()
{  delete liefernr;
   delete liefdate;
   delete liefer_kunde;
   delete tree_daten;
   delete artikelbox;
   delete tree_offen;
}
