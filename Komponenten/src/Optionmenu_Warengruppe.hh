#ifndef _OPTIONMENU_WARENGRUPPE_HH
#  define _OPTIONMENU_WARENGRUPPE_HH

#include <gtk--/optionmenu.h>
#include <vector>
#include <string>
#include <Artikel/ArtikelTyp.h>
#include <gtk--/menu.h>

class Optionmenu_Warengruppe :  public Gtk::OptionMenu
{
 public:
   Optionmenu_Warengruppe(int extbezschema); // extartbezid=kundenid
   SigC::Signal0<void> activate;

   void set_extartbezid(int i);
   void set_value(const ArtikelTyp& a);
   ArtikelTyp get_value() const; // liefert den artikeltyp

// veraltet
   std::string get_value_as_Name() const
   {  return ArtikelTyp::get_string(get_value());
   }
};
#endif
