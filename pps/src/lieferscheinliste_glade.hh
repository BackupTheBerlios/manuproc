// generated 2002/11/4 13:51:18 CET by thoma@Tiger.(none)
// using glademm V1.1.2a_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/petig/cvs/jacek/pps/auftrag.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/lieferscheinliste.hh and./src/lieferscheinliste.cc

#ifndef _LIEFERSCHEINLISTE_GLADE_HH
#  define _LIEFERSCHEINLISTE_GLADE_HH


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
#include "SimpleTree.hh"
#include <gtk--/progressbar.h>
#include <gtk--/label.h>
#include <gtk--/radiobutton.h>
#include "Datewin.hh"
#include <gtk--/optionmenu.h>
#include <gtk--/checkbutton.h>
#include "ArtikelBox.hh"
#include "KundenBox2.hh"

class lieferscheinliste_glade : public Gtk::Window
{  
        
        GlademmData *_data;
        Gtk::Tooltips _tooltips;
protected:
        class SimpleTree *tree;
        class Gtk::ProgressBar *progressbar;
        class Gtk::Label *label_anzahl;
        class Gtk::RadioButton *radiobutton_alle_zeit_von;
        class Gtk::RadioButton *radiobutton_nur_zeit_von;
        class Datewin *Wdatum_von;
        class Gtk::RadioButton *radiobutton_alle_zeit_bis;
        class Gtk::RadioButton *radiobutton_nur_zeit_bis;
        class Datewin *Wdatum_bis;
        class Gtk::OptionMenu *date_cumulate;
        struct date_cumulate
        {  enum enum_t 
	   {  Datum, Woche, Monat, Quartal, Jahr, 
	   };
        };
        class Gtk::RadioButton *radiobutton_alle_artikel;
        class Gtk::RadioButton *radiobutton_nur_artikel;
        class Gtk::CheckButton *first_komponent;
        class ArtikelBox *artikelbox;
        class Gtk::RadioButton *radiobutton_alle_kunden;
        class Gtk::RadioButton *radiobutton_nur_kunde;
        class KundenBox2 *kundenbox;
        
        lieferscheinliste_glade();
        
        ~lieferscheinliste_glade();
private:
        virtual void on_button_close_clicked() = 0;
        virtual void on_button_show_clicked() = 0;
        virtual gint on_button_drucken_button_release_event(GdkEventButton *ev) = 0;
        virtual void on_radiobutton_zeit_toggled() = 0;
        virtual void datum_von_activate() = 0;
        virtual void on_radiobutton_zeit_2_toggled() = 0;
        virtual void datum_bis_activate() = 0;
        virtual void on_radiobutton_artikel_toggled() = 0;
        virtual void artikelbox_activate() = 0;
        virtual void on_radiobutton_kunde_toggled() = 0;
        virtual void kundenbox_activate() = 0;
};
#endif
