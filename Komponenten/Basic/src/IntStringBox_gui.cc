// $Id: IntStringBox_gui.cc,v 1.2 2006/02/23 16:12:57 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include "IntStringBox_gui.hh"
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/bind.h>
#include <sigc++/compatibility.h>
#endif

//static GlademmData gd=GlademmData(Glib::RefPtr<Gtk::AccelGroup>());

IntStringPopup::IntStringPopup() : IntStringPopup_glade()
{  }

IntStringBox_gui::IntStringBox_gui() 
: IntStringBox_glade(), menu(0)
{  fuelleMenu();
  this->signal_button_press_event().connect(SigC::slot(*this,&IntStringBox_gui::MouseButton));
  // request smaller entries
  sc_int->get_entry()->set_width_chars(3);
  sc1_string->get_entry()->set_width_chars(8);
  sc2_string->get_entry()->set_width_chars(8);
}

void IntStringBox_gui::fuelleMenu()
{
  if (menu)   
    {  delete menu;
       menu=0;
    }
  menu=new IntStringPopup();
  menu->signal_zurueck().connect(SigC::slot(*this,&IntStringBox_gui::reset));
}

bool IntStringBox_gui::MouseButton(GdkEventButton *event)
{
  if ((event->type == GDK_BUTTON_PRESS) && menu)
   {  menu->popup(event->button,event->time);
      return true;
   }
  return false;
}

IntStringBox_gui::~IntStringBox_gui()
{  if (menu) delete menu;
}
