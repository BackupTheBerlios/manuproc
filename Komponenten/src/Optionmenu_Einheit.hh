// $Id: Optionmenu_Einheit.hh,v 1.4 2004/11/16 14:58:30 christof Exp $
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

#ifndef _OPTIONMENU_EINHEIT_HH
#  define _OPTIONMENU_EINHEIT_HH

#include <gtkmm/optionmenu.h>
#include <vector>
#include <string>
#include <Artikel/Einheiten.h>


class Optionmenu_Einheit :  public Gtk::OptionMenu
{
   void fuelle_menu();

   SigC::Signal0<void> activate;
 public:
   Optionmenu_Einheit() ;
   SigC::Signal0<void> &signal_activate() { return activate; }

   void set_value(const Einheit &i);
   Einheit get_value() const;

};
#endif
