// $Id: Optionmenu_OberKundengruppe.cc,v 1.1 2006/10/31 16:10:30 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG
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

#include "Optionmenu_OberKundengruppe.hh"
#include <Gtk_OStream.h>
#include <SelectMatching.h>
#include <Misc/i18n.h>

Optionmenu_OberKundengruppe::Optionmenu_OberKundengruppe()
{ fuelle_menu();
  get_menu()->signal_deactivate().connect(activate.slot());
}

Kundengruppe::ID Optionmenu_OberKundengruppe::get_value() const
{
  int i=long(const_cast<Gtk::MenuItem*>(get_menu()->get_active())->get_data("user_data"));
  return (Kundengruppe::ID)i;
}

void Optionmenu_OberKundengruppe::fuelle_menu()
{
  Query q("select max(grpnr), obergruppe from ku_gruppe "
	 " group by obergruppe order by obergruppe");

  Gtk::OStream os(this);
  Kundengruppe::ID grpid;
  std::string obername;
  Query::Row fi=q.Fetch();

  while(q.good())
   { fi >> grpid >> obername;
     os << dbgettext(obername);
     os.flush((void*)grpid);
     fi=q.Fetch();
   }
}

void Optionmenu_OberKundengruppe::register_value(
			Kundengruppe::ID maxgrpid,std::string const& name)
{ Gtk::OStream os(this,std::ios::app);
  os << dbgettext(name);
  os.flush(gpointer(maxgrpid));
}

namespace Gtk {
static bool operator==(gpointer data,const Kundengruppe::ID &s)
{ 
  return (long)data==s;
}
}

void Optionmenu_OberKundengruppe::set_value(const Kundengruppe::ID &s)
{  Gtk::Menu_Helpers::SelectMatching(*this,s);
}

