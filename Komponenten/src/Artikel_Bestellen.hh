// $Id: Artikel_Bestellen.hh,v 1.10 2004/11/08 09:13:37 christof Exp $
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

#ifndef _ARTIKEL_BESTELLEN_HH
#  define _ARTIKEL_BESTELLEN_HH


#include "Optionmenu_Instanz.hh"
#include <gtkmm/table.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <Artikel/ArtikelBase.h>
//#include <Instanzen/ppsInstanz.h>
#include "ArtikelBox.hh"
#include "ProzessBox.hh"

class Artikel_Bestellen :  public Gtk::Table
{
   Gtk::SpinButton *SP_menge;
   ArtikelBox *AB_artikel;
   ProzessBox *PB_prozess;
   Gtk::OptionMenu *OM_einheit;
   SigC::Signal0<void> activate;

   void menge();
   void artikel();
   void prozess();
 public: 
   Artikel_Bestellen();

   cH_Prozess get_Prozess()const ;
   void set_Prozess(const cH_Prozess& p) {PB_prozess->set_value(p);}
   ArtikelBase get_Artikel() const;
   double get_Menge() const ;
   void grab_focus_Artikel()
   {  AB_artikel->grab_focus();
   }

   SigC::Signal0<void> &signal_activate()
   { return activate; }
};
#endif
