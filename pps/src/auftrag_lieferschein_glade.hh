// generated 2002/9/27 16:10:53 CEST by thoma@Tiger.(none)
// using glademm V1.1.1c_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- auftrag.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/auftrag_lieferschein.hh and./src/auftrag_lieferschein.cc

#ifndef _AUFTRAG_LIEFERSCHEIN_GLADE_HH
#  define _AUFTRAG_LIEFERSCHEIN_GLADE_HH


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
#include <gtk--/checkbutton.h>
#include <gtk--/button.h>
#include "SearchComboContent.hh"
#include "Datewin.hh"
#include <gtk--/spinbutton.h>
#include <gtk--/entry.h>
#include <gtk--/table.h>
#include "KundenBox2.hh"
#include "SimpleTree.hh"
#include <gtk--/label.h>
#include "ArtikelBox.hh"
#include <gtk--/box.h>

class auftrag_lieferschein_glade : public Gtk::Window
{  
        
        GlademmData *_data;
protected:
        Gtk::Tooltips _tooltips;
        class Gtk::Window *auftrag_lieferschein;
        class Gtk::CheckMenuItem *masseneingabe2;
        class Gtk::CheckButton *checkbutton_ean_drucken;
        class Gtk::Button *lieferzeile_delete;
        class SearchComboContent<int> *liefernr;
        class Datewin *liefdate;
        class Gtk::SpinButton *spinbutton_paeckchen;
        class Gtk::SpinButton *spinbutton_pakete;
        class Gtk::SpinButton *spinbutton_brutto;
        class Gtk::SpinButton *spinbutton_netto;
        class Gtk::Entry *entry_dpdnr;
        class Gtk::Table *table_DPD;
        class Gtk::Entry *rngnr;
        class KundenBox2 *liefer_kunde;
        class SimpleTree *tree_daten;
        class Gtk::SpinButton *anzahl;
        class Gtk::Label *label_kundenauftrag;
        class Gtk::Entry *auftragnr;
        class ArtikelBox *artikelbox;
        class Gtk::Button *button_zeile_uebernehmen;
        class Gtk::Button *button_kompletter_auftrag;
        class Gtk::SpinButton *Palette;
        class Gtk::SpinButton *liefermenge;
        class Gtk::Label *menge_einheit;
        class Gtk::Label *label_menge;
        class Gtk::Button *button_zeile_modifizieren;
        class SimpleTree *tree_offen;
        class Gtk::VBox *vbox_eingabe;
        
        auftrag_lieferschein_glade();
        
        ~auftrag_lieferschein_glade();
};
#endif
