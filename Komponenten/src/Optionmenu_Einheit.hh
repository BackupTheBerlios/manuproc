#ifndef _OPTIONMENU_EINHEIT_HH
#  define _OPTIONMENU_EINHEIT_HH

#include <gtk--/optionmenu.h>
#include <vector>
#include <string>
#include <Artikel/Einheiten.h>


class Optionmenu_Einheit :  public Gtk::OptionMenu
{
   void fuelle_menu();

 public:
   Optionmenu_Einheit() ;
   SigC::Signal0<void> activate;

   void set_value(const Einheit &i);
   Einheit get_value() const;

};
#endif
