#ifndef _WWaehrung_HH
#  define _WWaehrung_HH

#include <gtk--/optionmenu.h>
#include <Aux/Waehrung.h>
//#include <map>
#include <vector>

class WWaehrung : public Gtk::OptionMenu //, Waehrung 
{
   private:
      struct st_bez {std::string lang; std::string kurz;
         st_bez(std::string l, std::string k):lang(l), kurz(k){} };
#warning map wäre schöner geht aber leider nicht :-( MAT
//      map<int,st_bez> map_bez;
      vector<st_bez> vec_bez;
   public:
      enum darstellung {KURZ,LANG};

      WWaehrung(darstellung kl);
      ~WWaehrung(){}

      std::string Kurzbezeichnung();
      std::string Langbezeichnung();
      
      void set_History(Waehrung::enum_t et) {set_history(et-1);} //enum_t fängt bei 1 an
      Waehrung::enum_t get_enum();

      SigC::Signal0<void> activate;
   private:
      void fuelleMenu(darstellung kl);
      
};
#endif
