// $Id: int_ImageButton.cc,v 1.1 2004/07/15 08:10:00 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG
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

bool int_ImageButton::Connection::toggle(GdkEventButton *ev)
{  model=!model.Value();
   return false;
}

void int_ImageButton::Connection::model2widget()
{  widget->set(model.Value()?on:off);
}

SigC::Connection int_ImageButton::Connection::connect()
{  if (eventbox) toggleconn=eventbox->signal_button_press_event().connect(SigC::slot(*this,&int_ImageButton::Connection::toggle));
   return SigC::Connection();
}

void int_ImageButton::Connection::disconnect()
{  toggleconn.disconnect();
}

int_ImageButton::int_ImageButton(const Model_ref<T> &m, 
		const Glib::RefPtr<Gdk::Pixbuf> &_off,
		const Glib::RefPtr<Gdk::Pixbuf> &_on)
	: conn(m), imag(0)
{  set_events(Gdk::BUTTON_PRESS_MASK);
   imag=Gtk::manage(new Gtk::Image());
   add(*imag);
   conn.set_images(_off,_on);
   conn.set_widget(imag,this);
   imag->show();
};

void int_ImageButton::Connection::set_widget(widget_t *w,Widget *ev)
{  eventbox=ev;
   this_t::set_widget(w);
}

void int_ImageButton::Connection::set_images(const Glib::RefPtr<Gdk::Pixbuf> &_off,
				const Glib::RefPtr<Gdk::Pixbuf> &_on)
{  off=_off;
   on=_on;
   if (widget) model2widget();
}
