// $Id: Optionmenu_Einheit.cc,v 1.4 2004/11/08 14:35:52 christof Exp $
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

#include "Optionmenu_Einheit.hh"
#include <Gtk_OStream.h>
#include <SelectMatching.h>

Optionmenu_Einheit::Optionmenu_Einheit()
{ fuelle_menu();
  get_menu()->signal_deactivate().connect(activate.slot());
}

Einheit Optionmenu_Einheit::get_value() const
{
  int i=int(const_cast<Gtk::MenuItem*>(get_menu()->get_active())->get_data("user_data"));
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
