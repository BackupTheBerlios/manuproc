#ifndef _ARTIKEL_BESTELLEN_HH
#  define _ARTIKEL_BESTELLEN_HH


#include "Optionmenu_Instanz.hh"
#include <gtk--/table.h>
#include <gtk--/label.h>
#include <gtk--/spinbutton.h>
#include <Artikel/ArtikelBase.h>
//#include <Aux/ppsInstanz.h>
#include "ArtikelBox.hh"
#include "ProzessBox.hh"

class Artikel_Bestellen :  public Gtk::Table
{
   Gtk::SpinButton *SP_menge;
   ArtikelBox *AB_artikel;
   ProzessBox *PB_prozess;

   void menge();
   void artikel();
   void prozess();
 public: 
   Artikel_Bestellen();

   cH_Prozess get_Prozess();
   void set_Prozess(const cH_Prozess& p) {PB_prozess->set_value(p);}
   ArtikelBase get_Artikel();
   int get_Menge();
   void grab_focus_Artikel()
   {  AB_artikel->grab_focus();
   }

   SigC::Signal0<void> activate;
};
#endif
