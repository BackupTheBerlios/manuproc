#ifndef _OPTIONMENU_WARENGRUPPE_HH
#  define _OPTIONMENU_WARENGRUPPE_HH

#include <gtk--/optionmenu.h>
#include <map>
#include <string>
#include <Artikel/ArtikelTyp.h>
#include <gtk--/menu.h>

class Optionmenu_Warengruppe :  public Gtk::OptionMenu
{
   void fuelle_menu(int extartbezid);
   std::map<int,std::string> map_warengruppe;

 public:
   Optionmenu_Warengruppe(int extartbezid) ; // extartbezid=kundenid
   SigC::Signal0<void> activate;

   void set_extartbezid(int i) 
      {fuelle_menu(i);get_menu()->deactivate.connect(activate.slot());}   
   void set_value(const ArtikelTyp& a) ;
   ArtikelTyp get_value() const; // liefert den artikeltyp
   std::string get_value_as_Name() const;
};
#endif
