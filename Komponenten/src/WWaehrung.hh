#ifndef _WWaehrung_HH
#  define _WWaehrung_HH

#include <gtk--/optionmenu.h>
#include <Aux/Waehrung.h>
#include <vector>

class WWaehrung : public Gtk::OptionMenu //, Waehrung 
{
   public:
      enum darstellung {KURZ,LANG};

      WWaehrung(darstellung kl=KURZ);
      ~WWaehrung(){}

      void set_value(cP_Waehrung et);
      cP_Waehrung get_value() const;

      SigC::Signal0<void> activate;

   private:
      void fuelleMenu(darstellung kl);
      
   public: // veraltet
      std::string Kurzbezeichnung() const
      {  return get_value()->Kurzbezeichnung(); }
      std::string Langbezeichnung() const
      {  return get_value()->Langbezeichnung(); }
      void set_History(Waehrung::enum_t et) 
      {  set_value(et);}
      Waehrung::enum_t get_enum() const
      {  return get_value()->get_enum(); }
};
#endif
