// generated 2002/1/29 13:17:09 CET by thoma@Tiger.
// using glademm V0.6.2c_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/petig/cvs/jacek/pps/auftrag.glade
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
#include <gtk--/label.h>
#include <gtk--/table.h>
#include <gtk--/frame.h>
#include <gtk--/viewport.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/adjustment.h>
#include <gtk--/box.h>

auftrag_lieferschein_glade::auftrag_lieferschein_glade(
) : Gtk::Window(GTK_WINDOW_TOPLEVEL)
{   auftrag_lieferschein = this;
   
   Gtk::MenuBar *menubar2 = manage(new class Gtk::MenuBar());
   Gtk::MenuItem *schlie_en2;
   Gtk::MenuItem *datei2;
   {   static GnomeUIInfo datei2_menu_uiinfo[] = 
      {        GNOMEUIINFO_MENU_CLOSE_ITEM(0, 0), 
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo menubar2_uiinfo[] = 
      {        GNOMEUIINFO_MENU_FILE_TREE(datei2_menu_uiinfo),
	      GNOMEUIINFO_END 
      };
      gnome_app_fill_menu(GTK_MENU_SHELL(menubar2->gtkobj()), menubar2_uiinfo, NULL, true, 0);
      schlie_en2 = Gtk::wrap(GTK_MENU_ITEM(datei2_menu_uiinfo[0].widget));
   }
   Gtk::Toolbar *toolbar5 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::Button *button9 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Schließen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_CLOSE)), 0, 0));
   Gtk::Button *button10 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Neu", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_NEW)), 0, 0));
   Gtk::Button *button13 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Speichern", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_SAVE)), 0, 0));
   Gtk::Button *button14 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Anzeigen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_SEARCH)), 0, 0));
   Gtk::Button *button15 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Drucken", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_PRINT)), 0, 0));
   lieferzeile_delete = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Lief.Zeile\n"
		"löschen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_CLEAR)), 0, 0));
   toolbar5->tools().push_back(Gtk::Toolbar_Helpers::Space());
   
   Gtk::Button *button_liste = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar5->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Liste", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar5->gtkobj()), GNOME_STOCK_PIXMAP_BOOK_OPEN)), 0, 0));
   Gtk::Label *label40 = manage(new class Gtk::Label("Lief. Nr."));
   Gtk::Label *label41 = manage(new class Gtk::Label("Lief. Datum"));
   liefdate = new class Datewin();
   
   Gtk::Label *label54 = manage(new class Gtk::Label("Rechnungsnr."));
   liefernr = new class SearchComboContent<int>();
   rngnr = manage(new class Gtk::Entry());
   liefer_kunde = new class KundenBox();
   
   Gtk::Table *table8 = manage(new class Gtk::Table(3, 5, false));
   Gtk::Frame *frame4 = manage(new class Gtk::Frame("Lieferschein"));
   tree_daten = new class SimpleTree(4, 3);
   
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
   Gtk::Button *button19 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar6->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Zeile übernehmen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar6->gtkobj()), GNOME_STOCK_PIXMAP_ADD)), 0, 0));
   Gtk::Button *button20 = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar6->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Auftrag übernehmen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar6->gtkobj()), GNOME_STOCK_PIXMAP_ATTACH)), 0, 0));
   Gtk::Adjustment *Palette_adj = manage(new class Gtk::Adjustment(1, 0, 999, 1, 10, 10));
   Palette = manage(new class Gtk::SpinButton(*Palette_adj, 1, 0));
   
   Gtk::Adjustment *liefermenge_adj = manage(new class Gtk::Adjustment(1, 0, 1e+06, 1, 10, 10));
   liefermenge = manage(new class Gtk::SpinButton(*liefermenge_adj, 1, 3));
   menge_einheit = manage(new class Gtk::Label("m"));
   
   Gtk::HBox *hbox7 = manage(new class Gtk::HBox(false, 0));
   label_menge = manage(new class Gtk::Label("Menge"));
   
   Gtk::Table *table9 = manage(new class Gtk::Table(2, 6, false));
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
   label40->set_alignment(0, 0.5);
   label40->set_padding(0, 0);
   label41->set_alignment(0, 0.5);
   label41->set_padding(0, 0);
   label54->set_alignment(0, 0.5);
   label54->set_padding(0, 0);
   label54->set_justify(GTK_JUSTIFY_LEFT);
   liefernr->set_usize(100, -1);
   rngnr->set_usize(80, -1);
   rngnr->set_editable(false);
   table8->attach(*label40, 0, 1, 1, 2, GTK_FILL, 0, 0, 0);
   table8->attach(*label41, 0, 1, 2, 3, GTK_FILL, 0, 0, 0);
   table8->attach(*liefdate, 1, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
   table8->attach(*label54, 3, 4, 1, 2, GTK_FILL, 0, 0, 0);
   table8->attach(*liefernr, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table8->attach(*rngnr, 4, 5, 1, 2, GTK_FILL, 0, 0, 0);
   table8->attach(*liefer_kunde, 0, 5, 0, 1, GTK_EXPAND|GTK_FILL, GTK_EXPAND|GTK_FILL, 0, 0);
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
   anzahl->set_usize(60, -1);
   anzahl->set_editable(true);
   anzahl->set_numeric(true);
   label_kundenauftrag->set_alignment(0, 0.5);
   label_kundenauftrag->set_padding(0, 0);
   label_kundenauftrag->set_justify(GTK_JUSTIFY_LEFT);
   auftragnr->set_usize(100, -1);
   auftragnr->set_editable(false);
   label59->set_alignment(0, 0.5);
   label59->set_padding(0, 0);
   _tooltips.set_tip(*button19, "Ausgewählte Zeile in Lieferschein übernehmen", "");
   button20->set_sensitive(false);
   _tooltips.set_tip(*button20, "Kompletten Auftrag in Lieferschein übernehmen", "");
   toolbar6->set_space_size(5);
   toolbar6->set_tooltips(true);
   Palette->set_usize(60, -1);
   Palette->set_editable(true);
   Palette->set_numeric(true);
   liefermenge->set_usize(100, -1);
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
   table9->set_border_width(5);
   table9->set_col_spacings(1);
   table9->attach(*label50, 0, 1, 0, 1, GTK_FILL, 0, 0, 0);
   table9->attach(*anzahl, 0, 1, 1, 2, GTK_FILL, 0, 0, 0);
   table9->attach(*label_kundenauftrag, 3, 4, 0, 1, GTK_FILL, 0, 0, 0);
   table9->attach(*auftragnr, 3, 4, 1, 2, GTK_FILL, 0, 0, 0);
   table9->attach(*artikelbox, 1, 2, 0, 2, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table9->attach(*label59, 4, 5, 0, 1, GTK_FILL, 0, 0, 0);
   table9->attach(*toolbar6, 5, 6, 0, 2, GTK_FILL, 0, 0, 0);
   table9->attach(*Palette, 4, 5, 1, 2, GTK_FILL, 0, 0, 0);
   table9->attach(*hbox7, 2, 3, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table9->attach(*label_menge, 2, 3, 0, 1, GTK_FILL, 0, 0, 0);
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
   auftrag_lieferschein->set_usize(800, 600);
   auftrag_lieferschein->set_title("Lieferscheine");
   auftrag_lieferschein->set_policy(true, true, false);
   auftrag_lieferschein->add(*vbox5);
   menubar2->show();
   toolbar5->show();
   label40->show();
   label41->show();
   liefdate->show();
   label54->show();
   liefernr->show();
   rngnr->show();
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
   liefdate->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefdate_activate));
   liefernr->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefnr_activate));
   liefernr->search.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefnr_search));
   liefer_kunde->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_lieferkunde_activate));
   tree_daten->leaf_selected.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_daten_leaf_selected));
   anzahl->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_liefermenge_activate));
   auftragnr->activate.connect(Palette->grab_focus.slot());
   artikelbox->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_artikelbox_activate));
   button19->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_newlieferentry_ok));
   button20->clicked.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_newlieferentryall_ok));
   Palette->activate.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_Palette_activate));
   liefermenge->activate.connect(auftragnr->grab_focus.slot());
   tree_offen->leaf_selected.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_offen_leaf_selected));
   tree_offen->unselect_row.connect(SigC::slot(static_cast<class auftrag_lieferschein*>(this), &auftrag_lieferschein::on_unselectrow_offauf));
}

auftrag_lieferschein_glade::~auftrag_lieferschein_glade()
{   delete liefdate;
   delete liefernr;
   delete liefer_kunde;
   delete tree_daten;
   delete artikelbox;
   delete tree_offen;
}
