// generated 2002/11/19 10:18:48 MET by jacek@ijacek.jacek.de
// using glademm V1.1.2a_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- auftrag.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/auftrag_main.cc

#include "config.h"
#include "auftrag_main_glade.hh"
#include <libgnomeui/gnome-app.h>
#include <libgnomeui/gnome-app-helper.h>
#include <gdk/gdkkeysyms.h>
#include <libgnomeui/gnome-stock.h>
#include <libgnome/libgnome.h>
#include <gtk--/button.h>
#include <gtk--/pixmap.h>
#include "../pixmaps/stock_convert_rev2.xpm"
#include <gtk--/toolbar.h>
#include <gtk--/box.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/label.h>
#include <gtk--/frame.h>
#include "../pixmaps/print_image.xpm"
#include "../pixmaps/wintext.xpm"
#include <gtk--/adjustment.h>
#include <gtk--/table.h>

auftrag_main_glade::auftrag_main_glade(
) : Gtk::Window(GTK_WINDOW_TOPLEVEL)
{  auftrag_main = this;
   main_menubar = manage(new class Gtk::MenuBar());
   {  
      
      static GnomeUIInfo main_datei_menu_uiinfo[] = 
      {       GNOMEUIINFO_MENU_EXIT_ITEM(0, 0), 
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo bearbeiten_menu_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "Erfassen", 0,  0, 0, 0, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_PROP,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Neuladen", 0,  0, 0, 0, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_REFRESH,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_MENU_PRINT_ITEM(0, 0), 
	      GNOMEUIINFO_SEPARATOR, 
	      { GNOME_APP_UI_ITEM, "Lieferscheine", 0,  0, 0, 0, GNOME_APP_PIXMAP_STOCK, GNOME_STOCK_MENU_SRCHRPL,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo main_kndbez_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "Kundenbezeichnung", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Interne Bezeichnung", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo kundenauftragsnummer_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "Kundenauftragsnummer", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Interne Auftragsnummer", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo kalenderwoche_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "Kalenderwoche", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Datum", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo kunden_name1_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "Kunden Name", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Kunden Nr.", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo materialbedarf_sortiert_nach_artikel_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "Materialbedarf sortiert nach Artikel", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Materialbedarf sortiert nach Menge", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo auftraege_ohne_kunde_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "Auftr�ge ohne Kunde", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Auftr�ge mit Kunde", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo main_einstellungen_menu_uiinfo[] = 
      {       { GNOME_APP_UI_RADIOITEMS, 0, 0,  main_kndbez_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_SEPARATOR, 
	      { GNOME_APP_UI_RADIOITEMS, 0, 0,  kundenauftragsnummer_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_SEPARATOR, 
	      { GNOME_APP_UI_RADIOITEMS, 0, 0,  kalenderwoche_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_SEPARATOR, 
	      { GNOME_APP_UI_RADIOITEMS, 0, 0,  kunden_name1_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_SEPARATOR, 
	      { GNOME_APP_UI_RADIOITEMS, 0, 0,  materialbedarf_sortiert_nach_artikel_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_SEPARATOR, 
	      { GNOME_APP_UI_RADIOITEMS, 0, 0,  auftraege_ohne_kunde_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo offene_auftraege_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "offene Auftr�ge", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Unbest�tigte Auftr�ge", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Geschlossene Auftr�ge", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "Stornierte Auftr�ge", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo ungeplante_menge_menu_uiinfo[] = 
      {       { GNOME_APP_UI_ITEM, "ungeplante Menge", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "bestellte Menge", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      { GNOME_APP_UI_ITEM, "freie verf�gbare Menge ", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo auftr_ge_menu_uiinfo[] = 
      {       { GNOME_APP_UI_TOGGLEITEM, "Auftr�ge bei Programmstart laden", 0,  0, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_SEPARATOR, 
	      { GNOME_APP_UI_RADIOITEMS, 0, 0,  offene_auftraege_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_SEPARATOR, 
	      { GNOME_APP_UI_RADIOITEMS, 0, 0,  ungeplante_menge_menu_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      static GnomeUIInfo main_menubar_uiinfo[] = 
      {       GNOMEUIINFO_MENU_FILE_TREE(main_datei_menu_uiinfo),
	      GNOMEUIINFO_MENU_EDIT_TREE(bearbeiten_menu_uiinfo),
	      GNOMEUIINFO_MENU_VIEW_TREE(main_einstellungen_menu_uiinfo),
	      { GNOME_APP_UI_SUBTREE, "Auftr�ge", 0,  auftr_ge_menu_uiinfo, 0, 0, GNOME_APP_PIXMAP_NONE, 0,  0, (GdkModifierType)0, 0 },
	      GNOMEUIINFO_END 
      };
      gnome_app_fill_menu(GTK_MENU_SHELL(main_menubar->gtkobj()), main_menubar_uiinfo, NULL, true, 0);
      beenden = Gtk::wrap(GTK_MENU_ITEM(main_datei_menu_uiinfo[0].widget));
      erfassen = Gtk::wrap(GTK_MENU_ITEM(bearbeiten_menu_uiinfo[0].widget));
      neuladen = Gtk::wrap(GTK_MENU_ITEM(bearbeiten_menu_uiinfo[1].widget));
      main_drucken = Gtk::wrap(GTK_MENU_ITEM(bearbeiten_menu_uiinfo[2].widget));
      trennlinie2 = Gtk::wrap(GTK_MENU_ITEM(bearbeiten_menu_uiinfo[3].widget));
      abschreiben = Gtk::wrap(GTK_MENU_ITEM(bearbeiten_menu_uiinfo[4].widget));
      main_kndbez = Gtk::wrap(GTK_RADIO_MENU_ITEM(main_kndbez_uiinfo[0].widget));
      main_intbez = Gtk::wrap(GTK_RADIO_MENU_ITEM(main_kndbez_uiinfo[1].widget));
      kundenauftragsnummer = Gtk::wrap(GTK_RADIO_MENU_ITEM(kundenauftragsnummer_uiinfo[0].widget));
      interne_auftragsnummer = Gtk::wrap(GTK_RADIO_MENU_ITEM(kundenauftragsnummer_uiinfo[1].widget));
      kalenderwoche = Gtk::wrap(GTK_RADIO_MENU_ITEM(kalenderwoche_uiinfo[0].widget));
      datum = Gtk::wrap(GTK_RADIO_MENU_ITEM(kalenderwoche_uiinfo[1].widget));
      kunden_name1 = Gtk::wrap(GTK_RADIO_MENU_ITEM(kunden_name1_uiinfo[0].widget));
      kunden_nr = Gtk::wrap(GTK_RADIO_MENU_ITEM(kunden_name1_uiinfo[1].widget));
      materialbedarf_sortiert_nach_artikel = Gtk::wrap(GTK_RADIO_MENU_ITEM(materialbedarf_sortiert_nach_artikel_uiinfo[0].widget));
      materialbedarf_sortiert_nach_menge = Gtk::wrap(GTK_RADIO_MENU_ITEM(materialbedarf_sortiert_nach_artikel_uiinfo[1].widget));
      auftraege_ohne_kunde = Gtk::wrap(GTK_RADIO_MENU_ITEM(auftraege_ohne_kunde_uiinfo[0].widget));
      auftraege_mit_kunde = Gtk::wrap(GTK_RADIO_MENU_ITEM(auftraege_ohne_kunde_uiinfo[1].widget));
      trennlinie5 = Gtk::wrap(GTK_MENU_ITEM(main_einstellungen_menu_uiinfo[1].widget));
      trennlinie3 = Gtk::wrap(GTK_MENU_ITEM(main_einstellungen_menu_uiinfo[3].widget));
      trennlinie4 = Gtk::wrap(GTK_MENU_ITEM(main_einstellungen_menu_uiinfo[5].widget));
      trennlinie6 = Gtk::wrap(GTK_MENU_ITEM(main_einstellungen_menu_uiinfo[7].widget));
      trennlinie8 = Gtk::wrap(GTK_MENU_ITEM(main_einstellungen_menu_uiinfo[9].widget));
      offene_auftraege = Gtk::wrap(GTK_RADIO_MENU_ITEM(offene_auftraege_uiinfo[0].widget));
      unbestaetigte_auftraege = Gtk::wrap(GTK_RADIO_MENU_ITEM(offene_auftraege_uiinfo[1].widget));
      geschlossene_auftraege = Gtk::wrap(GTK_RADIO_MENU_ITEM(offene_auftraege_uiinfo[2].widget));
      stornierte_auftraege = Gtk::wrap(GTK_RADIO_MENU_ITEM(offene_auftraege_uiinfo[3].widget));
      ungeplante_menge_menu = Gtk::wrap(GTK_RADIO_MENU_ITEM(ungeplante_menge_menu_uiinfo[0].widget));
      plan_menge_menu = Gtk::wrap(GTK_RADIO_MENU_ITEM(ungeplante_menge_menu_uiinfo[1].widget));
      dispo_menge_menu = Gtk::wrap(GTK_RADIO_MENU_ITEM(ungeplante_menge_menu_uiinfo[2].widget));
      preload_orders = Gtk::wrap(GTK_CHECK_MENU_ITEM(auftr_ge_menu_uiinfo[0].widget));
      trennlinie10 = Gtk::wrap(GTK_MENU_ITEM(auftr_ge_menu_uiinfo[1].widget));
      trennlinie7 = Gtk::wrap(GTK_MENU_ITEM(auftr_ge_menu_uiinfo[3].widget));
   }
   
   Gtk::Toolbar *toolbar1 = manage(new class Gtk::Toolbar(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH));
   maintree_s = new class SimpleTree(12, 10);
   scrolledwindow_maintree_s = manage(new class Gtk::ScrolledWindow());
   
   Gtk::VBox *vbox11 = manage(new class Gtk::VBox(false, 0));
   frame_offene_mengen = manage(new class Gtk::Frame("offene Mengen"));
   viewport_materialbedarf = manage(new class Gtk::Viewport());
   
   Gtk::ScrolledWindow *scrolledwindow11 = manage(new class Gtk::ScrolledWindow());
   frame_materialbedarf = manage(new class Gtk::Frame("Materialbedarf"));
   vpaned_an_mat = manage(new class Gtk::VPaned());
   
   Gtk::Label *label95 = manage(new class Gtk::Label("Freie Menge"));
   tree_lager_frei = new class SimpleTree(2, 2);
   
   Gtk::ScrolledWindow *scrolledwindow14 = manage(new class Gtk::ScrolledWindow());
   Gtk::VBox *vbox9 = manage(new class Gtk::VBox(false, 0));
   Gtk::Label *label96 = manage(new class Gtk::Label("Verplante Menge"));
   tree_lager_verplant = new class SimpleTree(3, 3);
   
   Gtk::ScrolledWindow *scrolledwindow15 = manage(new class Gtk::ScrolledWindow());
   Gtk::VBox *vbox10 = manage(new class Gtk::VBox(false, 0));
   hpaned_tree_lager = manage(new class Gtk::HPaned());
   Datum_instanz = new class Datewin();
   
   Gtk::Frame *frame32 = manage(new class Gtk::Frame("geplante Verf�gbarkeit"));
   Gtk::Pixmap *pixmap6 = manage(new class Gtk::Pixmap(stock_convert_rev2_xpm));
   Gtk::Label *label93 = manage(new class Gtk::Label("Selektierte(n)\n"
		"Auftr. planen"));
   Gtk::HBox *hbox14 = manage(new class Gtk::HBox(false, 0));
   button_instanz_get_selection = manage(new class Gtk::Button());
   
   Gtk::Pixmap *pixmap4 = manage(new class Gtk::Pixmap(print_image_xpm));
   Gtk::Label *label86 = manage(new class Gtk::Label("Drucken"));
   Gtk::HBox *hbox12 = manage(new class Gtk::HBox(false, 0));
   Gtk::Button *button_instanz_print = manage(new class Gtk::Button());
   Gtk::Pixmap *pixmap3 = manage(new class Gtk::Pixmap(wintext_xpm));
   Gtk::Label *label84 = manage(new class Gtk::Label("Neuer\n"
		"Auftrag"));
   Gtk::HBox *hbox11 = manage(new class Gtk::HBox(false, 0));
   button_neue_anr = manage(new class Gtk::Button());
   searchcombo_auftragid = new class SearchComboContent<int>();
   
   Gtk::Frame *frame31 = manage(new class Gtk::Frame("Auftragsnummer"));
   Gtk::Adjustment *spinbutton_geplante_menge_adj = manage(new class Gtk::Adjustment(0, 0, 1e+07, 1, 10, 10));
   spinbutton_geplante_menge = manage(new class Gtk::SpinButton(*spinbutton_geplante_menge_adj, 1, 0));
   frame_mengen_eingabe = manage(new class Gtk::Frame("Menge"));
   OptMen_Instanz_Bestellen = new class Optionmenu_Instanz();
   kunden_lieferant = new class KundenBox2();
   
   Gtk::HBox *hbox21 = manage(new class Gtk::HBox(false, 0));
   Gtk::Table *table24 = manage(new class Gtk::Table(2, 7, false));
   Gtk::Frame *frame15 = manage(new class Gtk::Frame());
   Gtk::Table *table18 = manage(new class Gtk::Table(1, 2, false));
   info_label_instanzen = manage(new class Gtk::Label("info text (etwas l�nger)"));
   table_instanzen = manage(new class Gtk::Table(2, 4, false));
   tree_neuer_auftrag = new class SimpleTree(4);
   
   Gtk::ScrolledWindow *scrolledwindow9 = manage(new class Gtk::ScrolledWindow());
   Gtk::VBox *vbox16 = manage(new class Gtk::VBox(false, 0));
   frame_instanzen = manage(new class Gtk::Frame("Bestellung"));
   
   Gtk::VBox *vbox15 = manage(new class Gtk::VBox(false, 0));
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("beenden", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_EXIT)), 0));
   endemain_button = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("drucken", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_PRINT)), 0));
   mainprint_button = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("neu laden", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_REFRESH)), 0));
   mainreload_button = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::Space());
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Auftrag", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_BOOK_RED)), 0));
   erfassen_button = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Liefersch.", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_BOOK_GREEN)), 0));
   lieferschein_button = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Rechnung", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_BOOK_YELLOW)), 0));
   rechnung_button = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::Space());
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ToggleElem("Bestellen", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_PROPERTIES)), 0));
   togglebutton_bestellen = static_cast<Gtk::ToggleButton *>(toolbar1->tools().back()->get_widget());
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ToggleElem("Material", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_CONVERT)), 0));
   togglebutton_material = static_cast<Gtk::ToggleButton *>(toolbar1->tools().back()->get_widget());
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ToggleElem("Auftr�ge", *manage(new Gtk::Pixmap(stock_convert_rev2_xpm)), 0));
   togglebutton_auftraege = static_cast<Gtk::ToggleButton *>(toolbar1->tools().back()->get_widget());
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::Space());
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("Artikel", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_COLORSELECTOR)), 0));
   Gtk::Button *button_artikeleingabe = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::Space());
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("erledigt", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_TRASH)), 0));
   button_auftrag_erledigt = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("aufr�umen", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_TRASH_FULL)), 0));
   button_Kunden_erledigt = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem("F�rben", *Gtk::wrap(gnome_stock_pixmap_widget(GTK_WIDGET(gtkobj()), GNOME_STOCK_PIXMAP_LINE_IN)), 0));
   button_faerben = static_cast<Gtk::Button *>(toolbar1->tools().back()->get_widget());
   main_kndbez->set_active(false);
   main_kndbez->set_show_toggle(true);
   main_intbez->set_active(true);
   main_intbez->set_show_toggle(true);
   kundenauftragsnummer->set_active(false);
   kundenauftragsnummer->set_show_toggle(true);
   interne_auftragsnummer->set_active(true);
   interne_auftragsnummer->set_show_toggle(true);
   kalenderwoche->set_active(false);
   kalenderwoche->set_show_toggle(true);
   datum->set_active(true);
   datum->set_show_toggle(true);
   kunden_name1->set_active(false);
   kunden_name1->set_show_toggle(true);
   kunden_nr->set_active(true);
   kunden_nr->set_show_toggle(true);
   materialbedarf_sortiert_nach_artikel->set_active(false);
   materialbedarf_sortiert_nach_artikel->set_show_toggle(true);
   materialbedarf_sortiert_nach_menge->set_active(true);
   materialbedarf_sortiert_nach_menge->set_show_toggle(true);
   auftraege_ohne_kunde->set_active(false);
   auftraege_ohne_kunde->set_show_toggle(true);
   auftraege_mit_kunde->set_active(true);
   auftraege_mit_kunde->set_show_toggle(true);
   preload_orders->set_active(false);
   preload_orders->set_show_toggle(true);
   offene_auftraege->set_active(false);
   offene_auftraege->set_show_toggle(true);
   unbestaetigte_auftraege->set_active(false);
   unbestaetigte_auftraege->set_show_toggle(true);
   geschlossene_auftraege->set_active(false);
   geschlossene_auftraege->set_show_toggle(true);
   stornierte_auftraege->set_active(true);
   stornierte_auftraege->set_show_toggle(true);
   ungeplante_menge_menu->set_active(false);
   ungeplante_menge_menu->set_show_toggle(true);
   plan_menge_menu->set_active(false);
   plan_menge_menu->set_show_toggle(true);
   dispo_menge_menu->set_active(false);
   dispo_menge_menu->set_show_toggle(true);
   main_menubar->set_shadow_type(GTK_SHADOW_OUT);
   _tooltips.set_tip(*mainprint_button, "linke Maustaste: drucken    rechte Maustaste: anzeigen", "");
   togglebutton_bestellen->set_active(false);
   togglebutton_material->set_active(false);
   togglebutton_auftraege->set_active(false);
   _tooltips.set_tip(*button_auftrag_erledigt, "Der gew�hlte Auftrag wird als 'erledigt' markiert", "");
   _tooltips.set_tip(*button_Kunden_erledigt, "Die zu erledigten Kundenauftr�gen geh�renden Eintr�ge dieser Instanz werden als 'erledigt' markiert.", "");
   toolbar1->set_space_size(5);
   toolbar1->set_tooltips(true);
   toolbar1->set_space_style(GTK_TOOLBAR_SPACE_EMPTY);
   scrolledwindow_maintree_s->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow_maintree_s->add(*maintree_s);
   vbox11->pack_start(*scrolledwindow_maintree_s);
   frame_offene_mengen->set_border_width(5);
   frame_offene_mengen->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame_offene_mengen->set_label_align(0,0);
   frame_offene_mengen->add(*vbox11);
   viewport_materialbedarf->set_usize(-1,70);
   viewport_materialbedarf->set_shadow_type(GTK_SHADOW_NONE);
   scrolledwindow11->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow11->add(*viewport_materialbedarf);
   frame_materialbedarf->set_usize(-1,120);
   frame_materialbedarf->set_border_width(5);
   frame_materialbedarf->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame_materialbedarf->set_label_align(0,0);
   frame_materialbedarf->add(*scrolledwindow11);
   vpaned_an_mat->set_handle_size(10);
   vpaned_an_mat->set_gutter_size(1);
   vpaned_an_mat->set_position(167);
   vpaned_an_mat->pack1(*frame_offene_mengen, false, true);
   vpaned_an_mat->pack2(*frame_materialbedarf, true, true);
   label95->set_alignment(0.5,0.5);
   label95->set_padding(0,0);
   label95->set_justify(GTK_JUSTIFY_CENTER);
   label95->set_line_wrap(false);
   scrolledwindow14->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow14->add(*tree_lager_frei);
   vbox9->pack_start(*label95, false, false, 0);
   vbox9->pack_start(*scrolledwindow14);
   label96->set_alignment(0.5,0.5);
   label96->set_padding(0,0);
   label96->set_justify(GTK_JUSTIFY_CENTER);
   label96->set_line_wrap(false);
   scrolledwindow15->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow15->add(*tree_lager_verplant);
   vbox10->pack_start(*label96, false, false, 0);
   vbox10->pack_start(*scrolledwindow15);
   hpaned_tree_lager->set_handle_size(6);
   hpaned_tree_lager->set_gutter_size(6);
   hpaned_tree_lager->pack1(*vbox9, true, true);
   hpaned_tree_lager->pack2(*vbox10, true, true);
   frame32->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame32->set_label_align(0,0);
   frame32->add(*Datum_instanz);
   pixmap6->set_alignment(0.5,0.5);
   pixmap6->set_padding(0,0);
   label93->set_alignment(0.5,0.5);
   label93->set_padding(0,0);
   label93->set_justify(GTK_JUSTIFY_CENTER);
   label93->set_line_wrap(false);
   hbox14->pack_start(*pixmap6);
   hbox14->pack_start(*label93, false, false, 0);
   button_instanz_get_selection->set_usize(-1,30);
   button_instanz_get_selection->set_flags(GTK_CAN_FOCUS);
   button_instanz_get_selection->set_relief(GTK_RELIEF_NORMAL);
   button_instanz_get_selection->add(*hbox14);
   pixmap4->set_alignment(0.5,0.5);
   pixmap4->set_padding(0,0);
   label86->set_alignment(0.5,0.5);
   label86->set_padding(0,0);
   label86->set_justify(GTK_JUSTIFY_CENTER);
   label86->set_line_wrap(false);
   hbox12->pack_start(*pixmap4);
   hbox12->pack_start(*label86, false, false, 0);
   button_instanz_print->set_flags(GTK_CAN_FOCUS);
   _tooltips.set_tip(*button_instanz_print, "Linker Knopf: Drucken, rechter Knopf: Vorschau", "");
   button_instanz_print->set_relief(GTK_RELIEF_NORMAL);
   button_instanz_print->add(*hbox12);
   pixmap3->set_alignment(0.5,0.5);
   pixmap3->set_padding(0,0);
   label84->set_alignment(0.5,0.5);
   label84->set_padding(0,0);
   label84->set_justify(GTK_JUSTIFY_CENTER);
   label84->set_line_wrap(false);
   hbox11->pack_start(*pixmap3);
   hbox11->pack_start(*label84, false, false, 0);
   button_neue_anr->set_usize(-1,30);
   button_neue_anr->set_flags(GTK_CAN_FOCUS);
   button_neue_anr->set_relief(GTK_RELIEF_NORMAL);
   button_neue_anr->add(*hbox11);
   searchcombo_auftragid->set_usize(150,-1);
   frame31->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame31->set_label_align(0,0);
   frame31->add(*searchcombo_auftragid);
   spinbutton_geplante_menge->set_usize(80,-1);
   spinbutton_geplante_menge->set_flags(GTK_CAN_FOCUS);
   spinbutton_geplante_menge->set_update_policy(GTK_UPDATE_ALWAYS);
   spinbutton_geplante_menge->set_numeric(false);
   spinbutton_geplante_menge->set_digits(0);
   spinbutton_geplante_menge->set_wrap(false);
   spinbutton_geplante_menge->set_snap_to_ticks(false);
   frame_mengen_eingabe->set_shadow_type(GTK_SHADOW_NONE);
   frame_mengen_eingabe->set_label_align(0,0);
   frame_mengen_eingabe->add(*spinbutton_geplante_menge);
   hbox21->pack_start(*OptMen_Instanz_Bestellen);
   hbox21->pack_start(*kunden_lieferant);
   table24->set_row_spacings(0);
   table24->set_col_spacings(0);
   table24->attach(*frame32, 4, 5, 1, 2, GTK_SHRINK|GTK_FILL, GTK_SHRINK|GTK_FILL, 0, 0);
   table24->attach(*button_instanz_get_selection, 5, 6, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
   table24->attach(*button_instanz_print, 6, 7, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
   table24->attach(*button_neue_anr, 1, 2, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
   table24->attach(*frame31, 2, 3, 1, 2, GTK_SHRINK|GTK_FILL, GTK_SHRINK|GTK_FILL, 0, 0);
   table24->attach(*frame_mengen_eingabe, 3, 4, 1, 2, GTK_SHRINK|GTK_FILL, GTK_SHRINK|GTK_FILL, 0, 0);
   table24->attach(*hbox21, 1, 7, 0, 1, GTK_SHRINK|GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);
   frame15->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame15->set_label_align(0,0);
   frame15->add(*table24);
   table18->set_row_spacings(0);
   table18->set_col_spacings(0);
   table18->attach(*frame15, 0, 1, 0, 1, GTK_EXPAND|GTK_SHRINK|GTK_FILL, GTK_SHRINK, 0, 0);
   info_label_instanzen->set_alignment(0,0.5);
   info_label_instanzen->set_padding(0,0);
   info_label_instanzen->set_justify(GTK_JUSTIFY_CENTER);
   info_label_instanzen->set_line_wrap(false);
   table_instanzen->set_row_spacings(0);
   table_instanzen->set_col_spacings(0);
   table_instanzen->attach(*table18, 0, 4, 1, 2, GTK_FILL, 0, 0, 0);
   table_instanzen->attach(*info_label_instanzen, 0, 4, 0, 1, GTK_SHRINK|GTK_FILL, GTK_SHRINK, 0, 0);
   scrolledwindow9->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   scrolledwindow9->add(*tree_neuer_auftrag);
   vbox16->pack_start(*table_instanzen, false, true, 0);
   vbox16->pack_start(*scrolledwindow9);
   frame_instanzen->set_border_width(5);
   frame_instanzen->set_shadow_type(GTK_SHADOW_ETCHED_IN);
   frame_instanzen->set_label_align(0,0);
   frame_instanzen->add(*vbox16);
   vbox15->pack_start(*main_menubar, false, false, 0);
   vbox15->pack_start(*toolbar1, false, false, 0);
   vbox15->pack_start(*vpaned_an_mat);
   vbox15->pack_start(*hpaned_tree_lager);
   vbox15->pack_start(*frame_instanzen);
   auftrag_main->set_title("Auftragsbearbeitung");
   auftrag_main->set_default_size(800,600);
   auftrag_main->set_modal(false);
   auftrag_main->set_position(GTK_WIN_POS_NONE);
   auftrag_main->set_policy(true, true, false);
   auftrag_main->add(*vbox15);
   main_menubar->show();
   toolbar1->show();
   maintree_s->show();
   scrolledwindow_maintree_s->show();
   vbox11->show();
   frame_offene_mengen->show();
   viewport_materialbedarf->show();
   scrolledwindow11->show();
   frame_materialbedarf->show();
   vpaned_an_mat->show();
   label95->show();
   tree_lager_frei->show();
   scrolledwindow14->show();
   vbox9->show();
   label96->show();
   tree_lager_verplant->show();
   scrolledwindow15->show();
   vbox10->show();
   hpaned_tree_lager->show();
   Datum_instanz->show();
   frame32->show();
   pixmap6->show();
   label93->show();
   hbox14->show();
   button_instanz_get_selection->show();
   pixmap4->show();
   label86->show();
   hbox12->show();
   button_instanz_print->show();
   pixmap3->show();
   label84->show();
   hbox11->show();
   button_neue_anr->show();
   searchcombo_auftragid->show();
   frame31->show();
   spinbutton_geplante_menge->show();
   frame_mengen_eingabe->show();
   OptMen_Instanz_Bestellen->show();
   kunden_lieferant->show();
   hbox21->show();
   table24->show();
   frame15->show();
   table18->show();
   info_label_instanzen->show();
   table_instanzen->show();
   tree_neuer_auftrag->show();
   scrolledwindow9->show();
   vbox16->show();
   frame_instanzen->show();
   vbox15->show();
   auftrag_main->show();
   beenden->activate.connect(SigC::slot(this, &auftrag_main_glade::on_beenden_activate));
   erfassen->activate.connect(SigC::slot(this, &auftrag_main_glade::on_erfassen_activate));
   neuladen->activate.connect(SigC::slot(this, &auftrag_main_glade::on_neuladen_activate));
   main_drucken->activate.connect(SigC::slot(this, &auftrag_main_glade::on_main_drucken_activate));
   abschreiben->activate.connect(SigC::slot(this, &auftrag_main_glade::on_lieferscheine_activate));
   main_kndbez->activate.connect(SigC::slot(this, &auftrag_main_glade::on_main_bezeichnung_activate));
   main_intbez->activate.connect(SigC::slot(this, &auftrag_main_glade::on_main_bezeichnung_activate));
   kundenauftragsnummer->activate.connect(SigC::slot(this, &auftrag_main_glade::on_kunden_anr_activate));
   interne_auftragsnummer->activate.connect(SigC::slot(this, &auftrag_main_glade::on_kunden_anr_activate));
   kalenderwoche->activate.connect(SigC::slot(this, &auftrag_main_glade::on_zeitdarstellung_activate));
   datum->activate.connect(SigC::slot(this, &auftrag_main_glade::on_zeitdarstellung_activate));
   kunden_name1->activate.connect(SigC::slot(this, &auftrag_main_glade::on_kundendarstellung_activate));
   kunden_nr->activate.connect(SigC::slot(this, &auftrag_main_glade::on_kundendarstellung_activate));
   materialbedarf_sortiert_nach_artikel->activate.connect(SigC::slot(this, &auftrag_main_glade::on_materialbedarf_sortiert));
   materialbedarf_sortiert_nach_menge->activate.connect(SigC::slot(this, &auftrag_main_glade::on_materialbedarf_sortiert));
   auftraege_ohne_kunde->activate.connect(SigC::slot(this, &auftrag_main_glade::on_auftraege_kunde_activate));
   auftraege_mit_kunde->activate.connect(SigC::slot(this, &auftrag_main_glade::on_auftraege_kunde_activate));
   preload_orders->activate.connect(SigC::slot(this, &auftrag_main_glade::on_preload_orders_activate));
   offene_auftraege->activate.connect(SigC::slot(this, &auftrag_main_glade::on_offene_auftraege_activate));
   unbestaetigte_auftraege->activate.connect(SigC::slot(this, &auftrag_main_glade::on_unbest_auftraege_activate));
   geschlossene_auftraege->activate.connect(SigC::slot(this, &auftrag_main_glade::on_closed_auftraege_activate));
   stornierte_auftraege->activate.connect(SigC::slot(this, &auftrag_main_glade::on_storno_auftraege_activate));
   ungeplante_menge_menu->activate.connect(SigC::slot(this, &auftrag_main_glade::on_ungeplante_menge_menu_activate));
   plan_menge_menu->activate.connect(SigC::slot(this, &auftrag_main_glade::on_plan_menge_menu_activate));
   dispo_menge_menu->activate.connect(SigC::slot(this, &auftrag_main_glade::on_dispo_menge_menu_activate));
   endemain_button->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_beenden_activate));
   mainprint_button->button_press_event.connect(SigC::slot(this, &auftrag_main_glade::on_mainprint_button_clicked));
   mainreload_button->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_neuladen_activate));
   erfassen_button->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_erfassen_activate));
   lieferschein_button->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_lieferscheine_activate));
   rechnung_button->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_rechnung_activate));
   togglebutton_bestellen->toggled.connect(SigC::slot(this, &auftrag_main_glade::on_togglebutton_bestellen_toggled));
   togglebutton_material->toggled.connect(SigC::slot(this, &auftrag_main_glade::on_togglebutton_material_toggled));
   togglebutton_auftraege->toggled.connect(SigC::slot(this, &auftrag_main_glade::on_togglebutton_auftraege_toggled));
   button_artikeleingabe->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_button_artikeleingabe_clicked));
   button_auftrag_erledigt->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_button_auftrag_erledigt_clicked));
   button_Kunden_erledigt->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_button_Kunden_erledigt_clicked));
   button_faerben->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_button_faerben_clicked));
   maintree_s->leaf_selected.connect(SigC::slot(this, &auftrag_main_glade::on_leaf_selected));
   maintree_s->unselect_row.connect(SigC::slot(this, &auftrag_main_glade::on_unselect_row));
   maintree_s->node_selected.connect(SigC::slot(this, &auftrag_main_glade::on_node_selected));
   maintree_s->leaf_unselected.connect(SigC::slot(this, &auftrag_main_glade::on_leaf_unselected));
   tree_lager_frei->leaf_selected.connect(SigC::slot(this, &auftrag_main_glade::on_lager_leaf_selected));
   tree_lager_frei->unselect_row.connect(SigC::slot(this, &auftrag_main_glade::on_lager_unselect_row));
   tree_lager_frei->node_selected.connect(SigC::slot(this, &auftrag_main_glade::on_lager_node_selected));
   tree_lager_verplant->leaf_selected.connect(SigC::slot(this, &auftrag_main_glade::on_lager_leaf_selected));
   tree_lager_verplant->unselect_row.connect(SigC::slot(this, &auftrag_main_glade::on_lager_unselect_row));
   tree_lager_verplant->node_selected.connect(SigC::slot(this, &auftrag_main_glade::on_lager_node_selected));
   button_instanz_get_selection->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_button_instanz_get_selection_clicked));
   button_instanz_print->button_press_event.connect(SigC::slot(this, &auftrag_main_glade::on_button_instanz_print_clicked));
   button_neue_anr->clicked.connect(SigC::slot(this, &auftrag_main_glade::on_button_neue_anr_clicked));
   searchcombo_auftragid->activate.connect(SigC::slot(this, &auftrag_main_glade::on_searchcombo_auftragid_activate));
   searchcombo_auftragid->search.connect(SigC::slot(this, &auftrag_main_glade::on_searchcombo_auftragid_search));
   OptMen_Instanz_Bestellen->activate.connect(SigC::slot(this, &auftrag_main_glade::on_OptMenu_Instanz_Bestellen_activate));
   kunden_lieferant->activate.connect(SigC::slot(this, &auftrag_main_glade::on_kunden_lieferant_activate));
   tree_neuer_auftrag->leaf_selected.connect(SigC::slot(this, &auftrag_main_glade::tree_neuer_auftrag_leaf_selected));
}

auftrag_main_glade::~auftrag_main_glade()
{  delete maintree_s;
   delete tree_lager_frei;
   delete tree_lager_verplant;
   delete Datum_instanz;
   delete searchcombo_auftragid;
   delete OptMen_Instanz_Bestellen;
   delete kunden_lieferant;
   delete tree_neuer_auftrag;
}
