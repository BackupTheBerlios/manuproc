#ifndef _OPTIONMENU_INSTANZ_HH
#  define _OPTIONMENU_INSTANZ_HH

#include <gtk--/optionmenu.h>
#include <vector>
#include <string>
#include <Aux/ppsInstanz.h>


class Optionmenu_Instanz :  public Gtk::OptionMenu
{
   void fuelle_menu();

 public:
   Optionmenu_Instanz() ;
   SigC::Signal0<void> activate;

   void set_value(const cH_ppsInstanz &i);
   cH_ppsInstanz get_value() const;

// ---- veraltet !!!!
   ppsInstanz::ppsInstId get_Instanz_Id() const
   {  return get_value()->Id();
   }
   std::string get_Instanz_Name() const
   {  return get_value()->Name();
   }
   void set_History(ppsInstanz::ppsInstId i)
   {  set_value(i); }
};
#endif
