// generated 2001/2/27 9:53:48 CET by thoma@ig23.
// using glademm V0.5_11f_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/petig/cvs/christof/kundendaten/kundendaten.glade
// for gtk 1.2.8 and gtkmm 1.2.4
//
// Please modify the corresponding derived classes in ./src/windowAuswahl.hh and./src/windowAuswahl.cc

#ifndef _WINDOWAUSWAHL_GLADE_HH
#  define _WINDOWAUSWAHL_GLADE_HH

#include <gtk--/window.h>
#include <gtk--/clist.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/radiobutton.h>

class windowAuswahl_glade : public Gtk::Window
{   
protected:
        
        class Gtk::CList *clistAuswahl;
        class Gtk::ScrolledWindow *scrolledwindow1;
        class Gtk::RadioButton *radiobuttonSortiername;
        class Gtk::RadioButton *radiobuttonKdNr;
        
        windowAuswahl_glade();
        
        ~windowAuswahl_glade();
};
#endif
