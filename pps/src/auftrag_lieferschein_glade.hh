// generated 2001/6/6 10:19:52 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/petig/cvs/jacek/pps/auftrag.glade
// for gtk 1.2.8 and gtkmm 1.2.5
//
// Please modify the corresponding derived classes in ./src/auftrag_lieferschein.hh and./src/auftrag_lieferschein.cc

#ifndef _AUFTRAG_LIEFERSCHEIN_GLADE_HH
#  define _AUFTRAG_LIEFERSCHEIN_GLADE_HH

#include <gtk--/window.h>
#include <gtk--/tooltips.h>
#include <libgnomeui/gtkpixmapmenuitem.h>
#include <gtk--/button.h>
#include "Datewin.hh"
#include "SearchComboContent.hh"
#include <gtk--/entry.h>
#include "KundenBox.hh"
#include "Lief_TCList.hh"
#include <gtk--/spinbutton.h>
#include "ArtikelBox.hh"
#include <gtk--/label.h>
#include "OffAuf_TCList.hh"
#include <gtk--/box.h>

class auftrag_lieferschein_glade : public Gtk::Window
{   
protected:
        
        Gtk::Tooltips _tooltips;
        class Gtk::Window *auftrag_lieferschein;
        class Gtk::Button *lieferzeile_delete;
        class Datewin *liefdate;
        class SearchComboContent<int> *liefernr;
        class Gtk::Entry *rngnr;
        class KundenBox *liefer_kunde;
        class Lief_TCList *lieferschein_liste;
        class Gtk::SpinButton *anzahl;
        class Gtk::Entry *auftragnr;
        class ArtikelBox *artikelbox;
        class Gtk::SpinButton *Palette;
        class Gtk::SpinButton *liefermenge;
        class Gtk::Label *menge_einheit;
        class OffAuf_TCList *offene_auftraege;
        class Gtk::VBox *vbox_eingabe;
        
        auftrag_lieferschein_glade();
        
        ~auftrag_lieferschein_glade();
};
#endif
