// $Id: WAuftragStatus.hh,v 1.17 2004/11/16 15:02:25 christof Exp $
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

#ifndef _WAuftragStatus_HH
#  define _WAuftragStatus_HH

#include <gtkmm/optionmenu.h>
#include <Auftrag/auftrag_status.h>
#include <Auftrag/AuftragBase.h>
#include <vector>

class WAuftragStatus : public Gtk::OptionMenu 
{
   private:
//      std::string status;
      void fuelleMenu();
      SigC::Signal0<void> activate;
   public:

      WAuftragStatus();
      ~WAuftragStatus(){}
      
      void set_value(AufStatVal a);
      AufStatVal get_value() const;
      SigC::Signal0<void> &signal_activate() { return activate; }

// Veraltet!
      void set_History(AufStatVal a) {set_value(a);} 
      AufStatVal get_Status() const { return get_value(); }
      const std::string get_Status_Name() const 
      { return AuftragBase::getStatusStr(get_value()); }
};
#endif
