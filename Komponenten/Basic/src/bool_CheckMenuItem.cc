// $Id: bool_CheckMenuItem.cc,v 1.7 2004/05/03 11:47:18 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002-2003 Adolf Petig GmbH & Co. KG
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
#include <bvector_item_CheckMenuItem.hh>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

void bool_CheckMenuItem::Connection::model2widget()
{  widget->set_active(model.get_value());
}

void bool_CheckMenuItem::Connection::widget2model()
{  model=widget->get_active();
}

SigC::Connection bool_CheckMenuItem::Connection::connect()
{  return widget->signal_toggled().connect(SigC::slot(*this,&bool_CheckMenuItem::Connection::controller2model));
}

bool_CheckMenuItem::bool_CheckMenuItem(const Model_ref<T> &m, const std::string &text)
	: Gtk::CheckMenuItem(text), conn(m,this)
{  
};

bool_CheckMenuItem::bool_CheckMenuItem(const Model_ref<T> &m, Gtk::Widget &w)
	: conn(m,this)
{
 Gtk::CheckMenuItem::add(w);
}

void bool_CheckMenuItem::setSensitive(const Model_ref<bool> &s,bool i)
{  Gtk::AssociateSensitivity(this,s,i);
}

void bvector_item_CheckMenuItem::Connection::model2widget()
{  widget->set_active(model.get_value());
}

void bvector_item_CheckMenuItem::Connection::widget2model()
{  model=widget->get_active();
}

SigC::Connection bvector_item_CheckMenuItem::Connection::connect()
{  return widget->signal_toggled().connect(SigC::slot(*this,&bvector_item_CheckMenuItem::Connection::controller2model));
}

bvector_item_CheckMenuItem::bvector_item_CheckMenuItem(const Model_ref<T> &m, const std::string &text)
	: Gtk::CheckMenuItem(text), conn(m,this)
{  
};

bvector_item_CheckMenuItem::bvector_item_CheckMenuItem(const Model_ref<T> &m, Gtk::Widget &w)
	: conn(m,this)
{
 Gtk::CheckMenuItem::add(w);
}

