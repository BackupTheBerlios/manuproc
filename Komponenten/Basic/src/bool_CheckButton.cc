// $Id: bool_CheckButton.cc,v 1.5 2002/12/12 10:58:12 christof Exp $
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

#include "bool_CheckButton.hh"

void bool_CheckButton::refresh(gpointer x)
{  if (x==&model.Value())
   {  my_ch_con.block();
      Gtk::CheckButton::set_active(model.get_value());
      my_ch_con.unblock();
   }
}

bool_CheckButton::bool_CheckButton(const Model_ref<T> &m, const std::string &text)
	: Gtk::CheckButton(text), model(m)
{  init();
}

bool_CheckButton::bool_CheckButton(const Model_ref<T> &m, Gtk::Widget &w)
	: model(m)
{  Gtk::ToggleButton::add(w);
   init();
}   

void bool_CheckButton::init()
{  Gtk::ToggleButton::set_active(model.get_value());
   my_ch_con=signal_toggled().connect(SigC::slot(*this,&bool_CheckButton::on_toggled));
   ch_con=model.signal_changed().connect(SigC::slot(*this,&bool_CheckButton::refresh));
};

void bool_CheckButton::on_toggled()
{  ch_con.block();
   model=Gtk::CheckButton::get_active();
   ch_con.unblock();
}
