// generated 2002/3/20 10:41:47 MET by christof@petig.petig.de
// using glademm V0.6.4b_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- auftrag.glade
// for gtk 1.2.9 and gtkmm 1.2.4
//
// Please modify the corresponding derived classes in ./src/lieferscheinliste.cc

#include "config.h"
#include "lieferscheinliste.hh"
#include <gtk--/button.h>
#include <libgnomeui/gnome-stock.h>
#include <gtk--/toolbar.h>
#include <gtk--/fixed.h>
#include <gtk--/separator.h>
#include <gtk--/table.h>
#include <gtk--/viewport.h>
#include <gtk--/scrolledwindow.h>

lieferscheinliste_glade::lieferscheinliste_glade(
) : Gtk::Window(GTK_WINDOW_TOPLEVEL)
{  
   
   Gtk::Window *lieferscheinliste = this;
   Gtk::Toolbar *toolbar11 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   Gtk::Button *button_close = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar11->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Schlie�en", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar11->gtkobj()), GNOME_STOCK_PIXMAP_EXIT)), 0, 0));
   Gtk::Button *button_show = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar11->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Anzeigen", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar11->gtkobj()), GNOME_STOCK_PIXMAP_SEARCH)), 0, 0));
   Gtk::Button *button_drucken = Gtk::wrap((GtkButton*)gtk_toolbar_append_element(GTK_TOOLBAR(toolbar11->gtkobj()), GTK_TOOLBAR_CHILD_BUTTON, 0, "Drucken", 0, 0, GTK_WIDGET(gnome_stock_pixmap_widget(GTK_WIDGET(toolbar11->gtkobj()), GNOME_STOCK_PIXMAP_PRINT)), 0, 0));
   Wdatum_von = new class Datewin();
   artikelbox = new class ArtikelBox(1);
   
   Gtk::RadioButton::Group _RadioBGroup_kunde;
   radiobutton_nur_kunde = manage(new class Gtk::RadioButton(_RadioBGroup_kunde, "Zeige nur Kunde", 0, 0.5));
   radiobutton_alle_kunden = manage(new class Gtk::RadioButton(_RadioBGroup_kunde, "Zeige alle Kunden", 0, 0.5));
   
   Gtk::Fixed *fixed3 = manage(new class Gtk::Fixed());
   Gtk::VSeparator *vseparator1 = manage(new class Gtk::VSeparator());
   Gtk::VSeparator *vseparator2 = manage(new class Gtk::VSeparator());
   kundenbox = new class KundenBox();
   
   Gtk::RadioButton::Group _RadioBGroup_zeit_2;
   radiobutton_alle_zeit_bis = manage(new class Gtk::RadioButton(_RadioBGroup_zeit_2, "bis heute", 0, 0.5));
   radiobutton_nur_zeit_bis = manage(new class Gtk::RadioButton(_RadioBGroup_zeit_2, "bis zum", 0, 0.5));
   
   Gtk::Fixed *fixed4 = manage(new class Gtk::Fixed());
   Wdatum_bis = new class Datewin();
   
   Gtk::RadioButton::Group _RadioBGroup_artikel;
   radiobutton_nur_artikel = manage(new class Gtk::RadioButton(_RadioBGroup_artikel, "Zeige nur Artikel", 0, 0.5));
   radiobutton_alle_artikel = manage(new class Gtk::RadioButton(_RadioBGroup_artikel, "Zeige alle Artikel", 0, 0.5));
   
   Gtk::Fixed *fixed1 = manage(new class Gtk::Fixed());
   Gtk::RadioButton::Group _RadioBGroup_zeit;
   radiobutton_nur_zeit_von = manage(new class Gtk::RadioButton(_RadioBGroup_zeit, "Zeige vom", 0, 0.5));
   radiobutton_alle_zeit_von = manage(new class Gtk::RadioButton(_RadioBGroup_zeit, "Zeige vom Anfang", 0, 0.5));
   
   Gtk::Fixed *fixed2 = manage(new class Gtk::Fixed());
   Gtk::Table *table15 = manage(new class Gtk::Table(4, 7, false));
   progressbar = manage(new class Gtk::ProgressBar());
   tree = new class SimpleTree(12, 10);
   
   Gtk::Viewport *viewport10 = manage(new class Gtk::Viewport());
   Gtk::ScrolledWindow *scrolledwindow10 = manage(new class Gtk::ScrolledWindow());
   label_anzahl = manage(new class Gtk::Label("label81"));
   
   Gtk::Table *table14 = manage(new class Gtk::Table(7, 5, false));
   toolbar11->set_space_size(5);
   toolbar11->set_tooltips(true);
   Wdatum_von->set_usize(180, -1);
   artikelbox->set_usize(350, -1);
   radiobutton_nur_kunde->set_usize(119, 24);
   radiobutton_nur_kunde->set_flags(GTK_CAN_FOCUS);
   radiobutton_alle_kunden->set_usize(126, 24);
   radiobutton_alle_kunden->set_flags(GTK_CAN_FOCUS);
   radiobutton_alle_kunden->set_active(true);
   fixed3->set_usize(220, -1);
   fixed3->put(*radiobutton_nur_kunde, 0, 16);
   fixed3->put(*radiobutton_alle_kunden, 0, 0);
   kundenbox->set_usize(220, -1);
   radiobutton_alle_zeit_bis->set_usize(71, 24);
   radiobutton_alle_zeit_bis->set_flags(GTK_CAN_FOCUS);
   radiobutton_alle_zeit_bis->set_active(true);
   radiobutton_nur_zeit_bis->set_usize(63, 24);
   radiobutton_nur_zeit_bis->set_flags(GTK_CAN_FOCUS);
   fixed4->set_usize(180, -1);
   fixed4->put(*radiobutton_alle_zeit_bis, 0, 0);
   fixed4->put(*radiobutton_nur_zeit_bis, 0, 16);
   Wdatum_bis->set_usize(180, -1);
   radiobutton_nur_artikel->set_usize(112, 24);
   radiobutton_nur_artikel->set_flags(GTK_CAN_FOCUS);
   radiobutton_alle_artikel->set_usize(116, 24);
   radiobutton_alle_artikel->set_flags(GTK_CAN_FOCUS);
   radiobutton_alle_artikel->set_active(true);
   fixed1->set_usize(350, -1);
   fixed1->put(*radiobutton_nur_artikel, 0, 16);
   fixed1->put(*radiobutton_alle_artikel, 0, 0);
   radiobutton_nur_zeit_von->set_usize(79, 24);
   radiobutton_nur_zeit_von->set_flags(GTK_CAN_FOCUS);
   radiobutton_alle_zeit_von->set_usize(123, 24);
   radiobutton_alle_zeit_von->set_flags(GTK_CAN_FOCUS);
   radiobutton_alle_zeit_von->set_active(true);
   fixed2->set_usize(180, -1);
   fixed2->put(*radiobutton_nur_zeit_von, 0, 16);
   fixed2->put(*radiobutton_alle_zeit_von, 0, 0);
   table15->attach(*Wdatum_von, 4, 5, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table15->attach(*artikelbox, 2, 3, 1, 2, GTK_FILL, GTK_EXPAND|GTK_FILL, 0, 0);
   table15->attach(*fixed3, 0, 1, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table15->attach(*vseparator1, 3, 4, 0, 2, GTK_FILL, GTK_FILL, 6, 0);
   table15->attach(*vseparator2, 1, 2, 0, 2, GTK_EXPAND, GTK_FILL, 6, 0);
   table15->attach(*kundenbox, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table15->attach(*fixed4, 6, 7, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table15->attach(*Wdatum_bis, 6, 7, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
   table15->attach(*fixed1, 2, 3, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   table15->attach(*fixed2, 4, 5, 0, 1, GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   viewport10->set_shadow_type(GTK_SHADOW_NONE);
   viewport10->add(*tree);
   scrolledwindow10->set_usize(-1, 500);
   scrolledwindow10->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow10->add(*viewport10);
   label_anzahl->set_alignment(0, 0.5);
   label_anzahl->set_padding(0, 0);
   table14->attach(*toolbar11, 1, 2, 0, 1, GTK_FILL, 0, 0, 0);
   table14->attach(*table15, 1, 2, 2, 3, GTK_FILL, GTK_EXPAND|GTK_FILL, 0, 0);
   table14->attach(*progressbar, 1, 2, 3, 4, GTK_EXPAND|GTK_FILL, 0, 0, 0);
   table14->attach(*scrolledwindow10, 1, 2, 5, 6, GTK_FILL, 0, 0, 0);
   table14->attach(*label_anzahl, 1, 2, 4, 5, GTK_FILL, 0, 0, 0);
   lieferscheinliste->set_title("Lieferscheinliste");
   lieferscheinliste->add(*table14);
   toolbar11->show();
   Wdatum_von->show();
   artikelbox->show();
   radiobutton_nur_kunde->show();
   radiobutton_alle_kunden->show();
   fixed3->show();
   vseparator1->show();
   vseparator2->show();
   kundenbox->show();
   radiobutton_alle_zeit_bis->show();
   radiobutton_nur_zeit_bis->show();
   fixed4->show();
   Wdatum_bis->show();
   radiobutton_nur_artikel->show();
   radiobutton_alle_artikel->show();
   fixed1->show();
   radiobutton_nur_zeit_von->show();
   radiobutton_alle_zeit_von->show();
   fixed2->show();
   table15->show();
   progressbar->show();
   tree->show();
   viewport10->show();
   scrolledwindow10->show();
   label_anzahl->show();
   table14->show();
   lieferscheinliste->show();
   button_close->clicked.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_button_close_clicked));
   button_show->clicked.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_button_show_clicked));
   button_drucken->clicked.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_button_drucken_clicked));
   Wdatum_von->activate.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::datum_von_activate));
   artikelbox->activate.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::artikelbox_activate));
   radiobutton_nur_kunde->toggled.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_radiobutton_kunde_toggled));
   radiobutton_alle_kunden->toggled.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_radiobutton_kunde_toggled));
   kundenbox->activate.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::kundenbox_activate));
   radiobutton_alle_zeit_bis->toggled.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_radiobutton_zeit_2_toggled));
   radiobutton_nur_zeit_bis->toggled.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_radiobutton_zeit_2_toggled));
   Wdatum_bis->activate.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::datum_bis_activate));
   radiobutton_nur_artikel->toggled.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_radiobutton_artikel_toggled));
   radiobutton_alle_artikel->toggled.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_radiobutton_artikel_toggled));
   radiobutton_nur_zeit_von->toggled.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_radiobutton_zeit_toggled));
   radiobutton_alle_zeit_von->toggled.connect(SigC::slot(static_cast<class lieferscheinliste*>(this), &lieferscheinliste::on_radiobutton_zeit_toggled));
}

lieferscheinliste_glade::~lieferscheinliste_glade()
{  delete Wdatum_von;
   delete artikelbox;
   delete kundenbox;
   delete Wdatum_bis;
   delete tree;
}
