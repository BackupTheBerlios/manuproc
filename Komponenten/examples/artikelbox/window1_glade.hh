// generated 2004/11/16 12:42:47 CET by christof@felix.petig.de
// using glademm V2.6.0_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- marc.glade
// for gtk 2.4.13 and gtkmm 2.4.5
//
// Please modify the corresponding derived classes in ././window1.hh and././window1.cc

#ifndef _WINDOW1_GLADE_HH
#  define _WINDOW1_GLADE_HH


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtkmm/accelgroup.h>

class GlademmData
{  
        
        Glib::RefPtr<Gtk::AccelGroup> accgrp;
public:
        
        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {  
        }
        
        Glib::RefPtr<Gtk::AccelGroup>  getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtkmm/window.h>
#include "WPreis.hh"
#include "Optionmenu_Instanz.hh"
#include "Optionmenu_Warengruppe.hh"
#include "PreisBox.hh"
#include "WAuftragStatus.hh"
#include "WWaehrung.hh"
#include "Datewin.hh"
#include "ProzessBox.hh"
#include "ArtikelBox.hh"
#include "KundenBox2.hh"
#include <gtkmm/box.h>

class window1_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
protected:
        class WPreis * WPreis;
private:
        class Optionmenu_Instanz * custom_OI;
protected:
        class Optionmenu_Warengruppe * custom_OW;
        class PreisBox * custom2;
        class WAuftragStatus * WAufStat;
        class WWaehrung * WWaehrung;
        class Datewin * datum;
        class ProzessBox * prozess;
        class ArtikelBox * artikelbox;
        class ArtikelBox * artikelbox2;
        class KundenBox2 * kundenbox;
        class Gtk::VBox * vbox1;
        
        window1_glade();
        
        ~window1_glade();
private:
        virtual void on_Beenden_activate() = 0;
        virtual void on_activate_wpreis() = 0;
        virtual void OI_activate() = 0;
        virtual void OW_activate() = 0;
        virtual void on_show_clicked() = 0;
        virtual void on_activate_wwaehrung() = 0;
        virtual void on_show_complete() = 0;
        virtual void andererKunde() = 0;
        virtual void on_reset() = 0;
};
#endif
