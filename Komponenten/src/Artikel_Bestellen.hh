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
//   Optionmenu_Instanz *OM_instanz;
   ArtikelBox *AB_artikel;
   ProzessBox *PB_prozess;

//   void instanz();
   void menge();
   void artikel();
   void prozess();
 public: 
   Artikel_Bestellen();
//   ~Artikel_Bestellen(){delete AB_artikel; delete OM_instanz;}  

   cH_Prozess get_Prozess();
   ArtikelBase get_Artikel();
   int get_Menge();
//   ppsInstanz::ppsInstId get_Instanz_Id();

   SigC::Signal0<void> activate;
};
#endif
