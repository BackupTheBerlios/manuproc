#ifndef _WPREIS_HH
#  define _WPREIS_HH

#include <gtk--/table.h>
//#include <gtk--/eventbox.h>
//#include <Aux/Waehrung.h>
#include <gtk--/spinbutton.h>
#include <gtk--/label.h>
#include "WWaehrung.hh"

class WPreis : public Gtk::Table 
{
//      class Gtk::Table *TA;
      Gtk::SpinButton *SP,*SP2;
      Gtk::Label *LA,*LA2;
      WWaehrung *WW;
      void betrag();
      void waehrung();
      void preismenge();
      void einheit();
      bool wwaehrung_bool;
   public:
//      WPreis() { WPreis(0); }
      WPreis(bool wwaehrung);
      ~WPreis(){}

      SigC::Signal0<void> activate;
      void update();

      void set_Betrag(double d){SP->set_value(d);}
      void set_Waehrung(const cP_Waehrung &w);
      void set_Preismenge(double d){SP2->set_value(d);}
      void set_Einheit(const std::string s){LA2->set_text(s);}

      double get_Betrag() const {gtk_spin_button_update(SP->gtkobj());return SP->get_value_as_float();}
      double get_Preismenge() const {gtk_spin_button_update(SP2->gtkobj());return SP2->get_value_as_float();}
      Waehrung::enum_t get_Waehrung_enum() const {return WW->get_enum();}

      void reset();
      void set_all(double d1, double d2);
      int get_text_length(){return SP->get_text().size();}
#warning diese Funktion wird nur von pps gebraucht, warum versteh ich aber nicht. MAT
         
};
#endif
