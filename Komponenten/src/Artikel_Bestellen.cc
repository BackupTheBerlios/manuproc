#include "Artikel_Bestellen.hh"
#include <gtk--/adjustment.h>
#include <Gtk_OStream.h>

Artikel_Bestellen::Artikel_Bestellen()
{
  menge();
  artikel();
  prozess();

  PB_prozess->activate.connect(SP_menge->grab_focus.slot());
  SP_menge->activate.connect(AB_artikel->grab_focus.slot());

//  show();
}


cH_Prozess Artikel_Bestellen::get_Prozess() const
{
  return PB_prozess->get_value();
}

ArtikelBase Artikel_Bestellen::get_Artikel() const
{
  return AB_artikel->get_value();
}

double Artikel_Bestellen::get_Menge() const
{
  gtk_spin_button_update(SP_menge->gtkobj());
  return SP_menge->get_value_as_float();
}

void Artikel_Bestellen::prozess()
{
 PB_prozess = manage(new ProzessBox());
 PB_prozess->set_value(Prozess::standard_id);
 PB_prozess->show();
 attach(*PB_prozess,0,1,0,1);
}


void Artikel_Bestellen::menge()
{
   Gtk::Label *label=manage(new Gtk::Label("Menge"));
   label->show();
   attach(*label,1,2,0,1);
   
   Gtk::Adjustment *SP_adj=manage(new class Gtk::Adjustment(1, 0, 10000, 1, 10, 10));
   SP_menge = manage(new class Gtk::SpinButton(*SP_adj, 1, 5));
   SP_menge->show();
//   SP_menge->activate.connect(activate.slot());
   attach(*SP_menge,2,3,0,1);
}

void Artikel_Bestellen::artikel()
{
 AB_artikel = manage(new ArtikelBox("Artikeleingabe","Bestellen"));
 AB_artikel->show_label(true);
 AB_artikel->show();
 AB_artikel->activate.connect(activate.slot());
 attach(*AB_artikel,0,4,1,2);
}

