#ifndef _OPTIONMENU_INSTANZ_HH
#  define _OPTIONMENU_INSTANZ_HH

#include <gtk--/optionmenu.h>
#include <vector>
#include <string>
#include <Aux/ppsInstanz.h>


class Optionmenu_Instanz :  public Gtk::OptionMenu
{
   void fuelle_menu();
   vector<std::string> vec_instanz;

void TEST();
 public:
   Optionmenu_Instanz() ;
   SigC::Signal0<void> activate;

   
// please use these standard names, CP
   void set_value(ppsInstanz::ppsInstId i) { set_History(i); }
   ppsInstanz::ppsInstId get_value() const { return get_Instanz_Id(); }

// ----
   ppsInstanz::ppsInstId get_Instanz_Id() const;
   std::string get_Instanz_Name() const;
   void set_History(ppsInstanz::ppsInstId i);
};
#endif
