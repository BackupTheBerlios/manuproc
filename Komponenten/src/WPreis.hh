#ifndef _WPREIS_HH
#  define _WPREIS_HH

#include <gtk--/table.h>
//#include <gtk--/eventbox.h>
//#include <Aux/Waehrung.h>
#include <gtk--/spinbutton.h>
#include <gtk--/label.h>

class WPreis : public Gtk::Table 
{
//      class Gtk::Table *TA;
      Gtk::SpinButton *SP,*SP2;
      Gtk::Label *LA,*LA2;
      void betrag();
      void waehrung();
      void preismenge();
      void einheit();
   public:
      WPreis();
      ~WPreis(){}

      SigC::Signal0<void> activate;
      void update();

      double get_Betrag(){return SP->get_value_as_float();}
      void   set_Betrag(double d){SP->set_value(d);}
      void set_Waehrung(const std::string s){LA->set_text(s);}
      double get_Preismenge(){return SP2->get_value_as_float();}
      void   set_Preismenge(double d){SP2->set_value(d);}
      void set_Einheit(const std::string s){LA2->set_text(s);}

      void reset();
      void set_all(double d1, double d2);
      int get_text_length(){return SP->get_text().size();}
#warning diese Funktion wird nur von pps gebraucht, warum versteh ich aber nicht. MAT
         
};
#endif
