#include "Optionmenu_Instanz.hh"
#include <Gtk_OStream.h>
#include <SelectMatching.h>

Optionmenu_Instanz::Optionmenu_Instanz(emode mode,bool nokunde)
{ fuelle_menu(mode,nokunde);
  get_menu()->deactivate.connect(activate.slot());
}

cH_ppsInstanz Optionmenu_Instanz::get_value() const
{  return static_cast<ppsInstanz*>(get_menu()->get_active()->get_user_data());
}

void Optionmenu_Instanz::fuelle_menu(emode mode,bool nokunde)
{
  std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
  Gtk::OStream os(this);
  for (std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
   {
    if     (mode== nurLager           && !(*i)->LagerInstanz() ) continue;
    else if(mode==ohneLagerZulieferer && (*i)->BestellungFuer()!=ppsInstanz::None ) continue;
    else if(mode==nurLieferanten      && !(*i)->Lieferant() ) continue;
    
    if(nokunde && (*i)->Id()==ppsInstanz::Kundenauftraege ) continue;

    os << (*i)->Name();
    os.flush((*i)->ref(), &HandleContent::unref);      
   }  
}

static bool operator==(gpointer data,const cH_ppsInstanz &i)
{  return *(static_cast<ppsInstanz*>(data))==*i;
}

void Optionmenu_Instanz::set_value(const cH_ppsInstanz &i)
{  Gtk::Menu_Helpers::SelectMatching(*this,i);
}
