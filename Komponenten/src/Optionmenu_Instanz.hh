// $Id: Optionmenu_Instanz.hh,v 1.13 2006/05/17 08:15:40 christof Exp $
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

#ifndef _OPTIONMENU_INSTANZ_HH
#  define _OPTIONMENU_INSTANZ_HH

#include <gtkmm/optionmenu.h>
#include <vector>
#include <string>
#include <Instanzen/ppsInstanz.h>


class Optionmenu_Instanz :  public Gtk::OptionMenu
{
 public:
   enum emode {allesAnzeigen,alles_und_none,nurLager,//ohneLagerZulieferer,
               nurLieferanten,keineKunden,Lager_und_None};

   void fuelle_menu(emode mode);
   void set_mode(emode mode);

   SigC::Signal0<void> activate;
 public:
   Optionmenu_Instanz(emode mode=alles_und_none) ;
   SigC::Signal0<void> &signal_activate() { return activate; }

   void set_value(const cH_ppsInstanz &i);
   cH_ppsInstanz get_value() const;
};
#endif
