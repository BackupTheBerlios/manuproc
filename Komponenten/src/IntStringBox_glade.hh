// generated 2003/3/17 17:16:49 CET by christof@puck.petig-baender.de
// using glademm V1.1.3e_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- Komponenten.glade
// for gtk 2.2.1 and gtkmm 2.2.0
//
// Please modify the corresponding derived classes in ././IntStringBox.hh and././IntStringBox.cc

#ifndef _INTSTRINGBOX_GLADE_HH
#  define _INTSTRINGBOX_GLADE_HH


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

#include <gtkmm/eventbox.h>
#include <gtkmm/label.h>
#include "SearchCombo.hh"

class IntStringBox_glade : public Gtk::EventBox
{  
protected:
        
        class Gtk::Label *label_int;
        class Gtk::Label *label_string;
        class Gtk::Label *label_string2;
        class SearchCombo *sc_int;
        class SearchCombo *sc1_string;
        class SearchCombo *sc2_string;
        
        IntStringBox_glade(GlademmData *_data);
        
        ~IntStringBox_glade();
private:
        virtual void int_activate() = 0;
        virtual void int_search(gboolean *cont,GtkSCContext context) = 0;
        virtual void string1_activate() = 0;
        virtual void string1_search(gboolean *cont,GtkSCContext context) = 0;
        virtual void string2_activate() = 0;
        virtual void string2_search(gboolean *cont,GtkSCContext context) = 0;
        virtual void reset() = 0;
        virtual bool MouseButton(GdkEventButton *ev) = 0;
};
#endif
