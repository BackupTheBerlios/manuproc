// $Id: Event.cc,v 1.3 2004/05/04 07:53:14 christof Exp $
/*  ManuProcWidgets: ManuProC's GUI element library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/Event.h>
#include <iostream>
#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/label.h>
#include <Misc/dbconnect.h>
#include <Misc/Event.h>
#include <Misc/FetchIStream.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

class MeinFenster : public Gtk::Window
{  Gtk::Label label;
   void Callback(const std::string &a, const std::string &b, const std::string &c);
public:
   MeinFenster();
};

MeinFenster::MeinFenster()
  : Gtk::Window(Gtk::WINDOW_TOPLEVEL), label("noch kein Event")
{  add(label);
   label.show();
   show();
   ManuProC::Event::signal_event().connect(SigC::slot(*this,&MeinFenster::Callback));
}

void MeinFenster::Callback(const std::string &a, const std::string &b, const std::string &c)
{  label.set_text("["+a+","+b+"] "+c);
}

int main(int argc, char **argv)
{  Gtk::Main m(&argc, &argv);
   ManuProC::dbconnect();
   Query("set names 'utf-8'");
   ManuProC::Event::connect_glib();
   MeinFenster *f=new MeinFenster();
   m.run(*f);
   ManuProC::dbdisconnect();
   return 0;   
}
