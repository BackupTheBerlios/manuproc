// generated 2004/5/11 14:19:25 CEST by jacek@jaceksdell.(none)
// using glademm V2.0.0.2
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/jacek/cvs/manuproc/ManuProC_Agent/mpc_agent.glade
// for gtk 2.2.4 and gtkmm 2.2.8
//
// Please modify the corresponding derived classes in ./src/kunden_selector.hh and./src/kunden_selector.cc

#ifndef _KUNDEN_SELECTOR_GLADE_HH
#  define _KUNDEN_SELECTOR_GLADE_HH


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

#include <gtkmm/dialog.h>
#include <gtkmm/button.h>
#include "SimpleTree.hh"

class kunden_selector_glade : public Gtk::Dialog
{  
        
        GlademmData *gmm_data;
protected:
        class Gtk::Button *ok_kunde_select;
        class SimpleTree *kunden_select_liste;
        
        kunden_selector_glade();
        
        ~kunden_selector_glade();
private:
        virtual void on_cancel_kunde_select_clicked() = 0;
        virtual void on_ok_kunde_select_clicked() = 0;
        virtual void on_kundenlist_leaf_selected(cH_RowDataBase leaf) = 0;
};
#endif