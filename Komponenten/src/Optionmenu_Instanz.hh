#ifndef _OPTIONMENU_INSTANZ_HH
#  define _OPTIONMENU_INSTANZ_HH

#include <gtk--/optionmenu.h>
#include <vector>
#include <string>
#include <Aux/ppsInstanz.h>


class Optionmenu_Instanz :  public Gtk::OptionMenu
{
 public:
   enum emode {allesAnzeigen,nurLager,ohneLagerZulieferer,nurLieferanten,keineKunden};

   void fuelle_menu(emode mode,bool nokunde);

 public:
   Optionmenu_Instanz(emode mode,bool nokunde=false) ;
   SigC::Signal0<void> activate;

   void set_value(const cH_ppsInstanz &i);
   cH_ppsInstanz get_value() const;

// ---- veraltet !!!!
   ppsInstanz::ID get_Instanz_Id() const
   {  return get_value()->Id();
   }
   std::string get_Instanz_Name() const
   {  return get_value()->Name();
   }
   void set_History(ppsInstanz::ID i)
   {  set_value(i); }
};
#endif
