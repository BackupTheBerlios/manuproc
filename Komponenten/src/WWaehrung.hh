// $Id: WWaehrung.hh,v 1.12 2002/09/18 08:53:42 christof Exp $
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

#ifndef _WWaehrung_HH
#  define _WWaehrung_HH

#include <gtk--/optionmenu.h>
#include <Aux/Waehrung.h>
#include <vector>

class WWaehrung : public Gtk::OptionMenu //, Waehrung 
{
   public:
      enum darstellung {KURZ,LANG};

      WWaehrung(darstellung kl=KURZ);
      ~WWaehrung(){}

      void set_value(cP_Waehrung et);
      cP_Waehrung get_value() const;

      SigC::Signal0<void> activate;

   private:
      void fuelleMenu(darstellung kl);
      
   public: // veraltet
      std::string Kurzbezeichnung() const
      {  return get_value()->Kurzbezeichnung(); }
      std::string Langbezeichnung() const
      {  return get_value()->Langbezeichnung(); }
      void set_History(WaehrungID::enum_t et) 
      {  set_value(et);}
      WaehrungID::enum_t get_enum() const
      {  return get_value()->get_enum(); }
};
#endif
