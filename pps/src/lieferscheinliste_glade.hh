// generated 2002/6/20 10:59:16 CEST by thoma@Tiger.(none)
// using glademm V1.1.0b_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/petig/cvs/jacek/pps/auftrag.glade
// for gtk 1.2.10 and gtkmm 1.2.9
//
// Please modify the corresponding derived classes in ./src/lieferscheinliste.hh and./src/lieferscheinliste.cc

#ifndef _LIEFERSCHEINLISTE_GLADE_HH
#  define _LIEFERSCHEINLISTE_GLADE_HH

#include <gtk--/window.h>
#include <gtk--/tooltips.h>
#include "Datewin.hh"
#include "ArtikelBox.hh"
#include <gtk--/radiobutton.h>
#include "KundenBox2.hh"
#include <gtk--/progressbar.h>
#include "SimpleTree.hh"
#include <gtk--/label.h>

class lieferscheinliste_glade : public Gtk::Window
{  
        
        Gtk::Tooltips _tooltips;
protected:
        class Datewin *Wdatum_von;
        class ArtikelBox *artikelbox;
        class Gtk::RadioButton *radiobutton_nur_kunde;
        class Gtk::RadioButton *radiobutton_alle_kunden;
        class KundenBox2 *kundenbox;
        class Gtk::RadioButton *radiobutton_alle_zeit_bis;
        class Gtk::RadioButton *radiobutton_nur_zeit_bis;
        class Datewin *Wdatum_bis;
        class Gtk::RadioButton *radiobutton_nur_artikel;
        class Gtk::RadioButton *radiobutton_alle_artikel;
        class Gtk::RadioButton *radiobutton_nur_zeit_von;
        class Gtk::RadioButton *radiobutton_alle_zeit_von;
        class Gtk::ProgressBar *progressbar;
        class SimpleTree *tree;
        class Gtk::Label *label_anzahl;
        
        lieferscheinliste_glade();
        
        ~lieferscheinliste_glade();
};
#endif
