// $Id: bool_ImageButton.cc,v 1.1 2003/03/07 08:10:25 christof Exp $
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

#include "bool_ImageButton.hh"

void bool_ImageButton::refresh(gpointer x)
{  if (model.matches(x))
   {  my_ch_con.block();
      Gtk::CheckButton::set_active(model.get_value());

      imag->set(model.Value()?on:off,Glib::RefPtr<Gdk::Bitmap>());
      my_ch_con.unblock();
   }
}

// mask?
bool_ImageButton::bool_ImageButton(const Model_ref<T> &m, 
		Glib::RefPtr<Gdk::Pixmap> _off, Glib::RefPtr<Gdk::Pixmap> _on)
	: off(_off), on(_on), model(m), imag(0)
{  imag=Gtk::manage(new Gtk::Image(model.Value()?on:off,Glib::RefPtr<Gdk::Bitmap>()));
   imag->show();
   Gtk::ToggleButton::add(*imag);
   set_mode(false);
   Gtk::ToggleButton::set_active(model.get_value());
   my_ch_con=signal_toggled().connect(SigC::slot(*this,&bool_ImageButton::on_toggled));
   ch_con=model.signal_changed().connect(SigC::slot(*this,&bool_ImageButton::refresh));
};

void bool_ImageButton::on_toggled()
{  ch_con.block();
   model=Gtk::CheckButton::get_active();
   imag->set(model.Value()?on:off,Glib::RefPtr<Gdk::Bitmap>());
   ch_con.unblock();
}
