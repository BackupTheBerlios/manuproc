// $Id: Optionmenu_AuftragLabel.hh,v 1.2 2005/09/07 13:27:37 christof Exp $
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

#ifndef _OPTIONMENU_AUFTRAGLABEL_HH
#  define _OPTIONMENU_AUFTRAGLABEL_HH

#include <gtkmm/optionmenu.h>
#include <vector>
#include <string>

class Optionmenu_AuftragLabel :  public Gtk::OptionMenu
{
   void fuelle_menu();
   SigC::Signal0<void> activate;

 public:
   Optionmenu_AuftragLabel() ;

   SigC::Signal0<void> &signal_activate()
   { return activate; }

   void set_value(const int alid);
   int get_value() const;
};
#endif
