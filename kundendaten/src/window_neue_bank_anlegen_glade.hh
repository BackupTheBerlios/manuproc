// generated 2005/6/24 11:37:58 CEST by christof@vesta.petig-baender.de
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- kundendaten.glade
// for gtk 2.7.0 and gtkmm 2.6.3
//
// Please modify the corresponding derived classes in ./src/window_neue_bank_anlegen.hh and./src/window_neue_bank_anlegen.cc

#ifndef _WINDOW_NEUE_BANK_ANLEGEN_GLADE_HH
#  define _WINDOW_NEUE_BANK_ANLEGEN_GLADE_HH


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
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

class window_neue_bank_anlegen_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
protected:
        class Gtk::Entry * entry_bankname;
        class Gtk::Entry * entry_blz;
        class Gtk::Label * label_text;
        
        window_neue_bank_anlegen_glade();
        
        ~window_neue_bank_anlegen_glade();
private:
        virtual void on_entry_bankname_activate() = 0;
        virtual void on_entry_blz_activate() = 0;
        virtual void on_button_uebernehmen_clicked() = 0;
        virtual void on_button_abbrechen_clicked() = 0;
};
#endif
