// $Id: Optionmenu_Instanz.cc,v 1.3 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

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
    else if(mode==ohneLagerZulieferer && (*i)->BestellungFuer()!=ppsInstanzID::None ) continue;
    else if(mode==nurLieferanten      && !(*i)->Lieferant() ) continue;
    
    if(nokunde && (*i)->Id()==ppsInstanzID::Kundenauftraege ) continue;

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
