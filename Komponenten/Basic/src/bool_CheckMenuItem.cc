// $Id: bool_CheckMenuItem.cc,v 1.4 2003/03/07 08:10:25 christof Exp $
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

#include "bool_CheckMenuItem.hh"
#include "bool_properties.hh"

void bool_CheckMenuItem::refresh(gpointer x)
{  if (model.matches(x))
   {  my_ch_con.block();
      Gtk::CheckMenuItem::set_active(model.get_value());
      my_ch_con.unblock();
   }
}

bool_CheckMenuItem::bool_CheckMenuItem(const Model_ref<T> &m, const std::string &text)
	: Gtk::CheckMenuItem(text), model(m)
{  
   init();
};

bool_CheckMenuItem::bool_CheckMenuItem(const Model_ref<T> &m, Gtk::Widget &w)
	: model(m)
{
 Gtk::CheckMenuItem::add(w);
 init();
}

void bool_CheckMenuItem::init()
{
 Gtk::CheckMenuItem::set_active(model.get_value());
 my_ch_con=signal_toggled().connect(SigC::slot(*this,&bool_CheckMenuItem::on_toggled));
 ch_con=model.signal_changed().connect(SigC::slot(*this,&bool_CheckMenuItem::refresh));
}

void bool_CheckMenuItem::setSensitive(const Model_ref<bool> &s,bool i)
{  Gtk::AssociateSensitivity(this,s,i);
}

void bool_CheckMenuItem::on_toggled()
{  ch_con.block();
   model=Gtk::CheckMenuItem::get_active();
   ch_con.unblock();
}
