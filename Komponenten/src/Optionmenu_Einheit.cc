#include "Optionmenu_Einheit.hh"
#include <Gtk_OStream.h>
#include <SelectMatching.h>

Optionmenu_Einheit::Optionmenu_Einheit()
{ fuelle_menu();
  get_menu()->deactivate.connect(activate.slot());
}

Einheit Optionmenu_Einheit::get_value() const
{
  int i=int(get_menu()->get_active()->get_user_data());
  return Einheit(Einheit::ID(i));
//  return static_cast<Einheit*>(get_menu()->get_active()->get_user_data());
}

void Optionmenu_Einheit::fuelle_menu()
{
  std::vector<Einheit> V=Einheit::alleEinheiten();  
  Gtk::OStream os(this);
  for(std::vector<Einheit>::iterator i=V.begin();i!=V.end();++i)
   {
     os << i->Bezeichnung();  
     os.flush((void*)i->Id());
   }
}

static bool operator==(gpointer data,const Einheit &i)
{ // return *(static_cast<Einheit*>(data))==*i;
  return (int)(data)==i.Id();
}

void Optionmenu_Einheit::set_value(const Einheit &i)
{  Gtk::Menu_Helpers::SelectMatching(*this,i);
}
