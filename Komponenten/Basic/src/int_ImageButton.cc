// $Id: int_ImageButton.cc,v 1.3 2004/07/16 22:42:12 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#include "int_ImageButton.hh"
#include <gtkmm/tooltips.h>

bool int_ImageButton::Connection::toggle(GdkEventButton *ev)
{  if (props.empty()) return false;
   std::map<int,properties>::const_iterator i=props.find(model.Value());
   if (i==props.end()) i=props.begin();
   else 
   {  ++i;
      if (i==props.end()) i=props.begin();
   }
   model=i->first;
   return false;
}

void int_ImageButton::Connection::model2widget()
{  widget->set(props[model.Value()].pixbuf);
   if (tips) tips->set_tip(*eventbox, props[model.Value()].tooltip);
}

SigC::Connection int_ImageButton::Connection::connect()
{  if (eventbox) toggleconn=eventbox->signal_button_press_event().connect(SigC::slot(*this,&int_ImageButton::Connection::toggle));
   return SigC::Connection();
}

void int_ImageButton::Connection::disconnect()
{  toggleconn.disconnect();
}

int_ImageButton::int_ImageButton(const Model_ref<T> &m)
	: conn(m), imag()
{  set_events(Gdk::BUTTON_PRESS_MASK);
   imag=Gtk::manage(new Gtk::Image());
   add(*imag);
   conn.set_widget(imag,this);
   imag->show();
};

void int_ImageButton::Connection::set_widget(widget_t *w,Widget *ev)
{  eventbox=ev;
   this_t::set_widget(w);
}

void int_ImageButton::Connection::add_entry(int value, Glib::RefPtr<Gdk::Pixbuf> pixbuf, 
				const std::string &tooltip)
{  props[value].pixbuf=pixbuf;
   props[value].tooltip=tooltip;
   if (!!model && model.Value()==value) model2widget();
}
