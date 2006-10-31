// $Id: Optionmenu_OberKundengruppe.hh,v 1.3 2006/10/31 16:10:39 christof Exp $
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

#ifndef _OPTIONMENU_OBERKUNDENGRUPPE_HH
#  define _OPTIONMENU_OBERKUNDENGRUPPE_HH

#include <gtkmm/optionmenu.h>
#include <vector>
#include <string>
#include <Kunde/Kundengruppe.h>


class Optionmenu_OberKundengruppe :  public Gtk::OptionMenu
{
   void fuelle_menu(bool reload=false);
   SigC::Signal0<void> activate;
   bool unique_only;

 public:
   Optionmenu_OberKundengruppe(bool _uniqe_only=false) ;
   SigC::Signal0<void> &signal_activate() { return activate; }

   void set_value(const Kundengruppe::ID &_obergrpid);
   Kundengruppe::ID get_value() const;
   void reload();
   void register_value(Kundengruppe::ID maxgrpid,std::string const& _obergrp);
};
#endif
