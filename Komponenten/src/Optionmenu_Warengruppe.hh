// $Id: Optionmenu_Warengruppe.hh,v 1.7 2004/11/16 14:58:30 christof Exp $
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

#ifndef _OPTIONMENU_WARENGRUPPE_HH
#  define _OPTIONMENU_WARENGRUPPE_HH

#include <gtkmm/optionmenu.h>
#include <vector>
#include <string>
#include <Artikel/ArtikelTyp.h>
#include <gtkmm/menu.h>
#include <ExtBezSchema/ExtBezSchema.h>

class Optionmenu_Warengruppe :  public Gtk::OptionMenu
{
   SigC::Signal0<void> activate;
 public:
   Optionmenu_Warengruppe(int extbezschema=ExtBezSchema::none_id); // extartbezid=kundenid
   SigC::Signal0<void> &signal_activate() { return activate; }

   void set_extartbezid(int i);
   void set_value(const ArtikelTyp& a);
   ArtikelTyp get_value() const; // liefert den artikeltyp

// veraltet
   std::string get_value_as_Name() const
   {  return ArtikelTyp::get_string(get_value());
   }
};
#endif
