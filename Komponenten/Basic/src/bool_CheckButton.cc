// $Id: bool_CheckButton.cc,v 1.8 2004/04/30 13:31:02 christof Exp $
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

#include "bool_CheckButton.hh"
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

void bool_CheckButton::Connection::model2widget()
{  widget->set_active(model.get_value());
}

void bool_CheckButton::Connection::widget2model()
{  model=widget->get_active();
}

SigC::Connection bool_CheckButton::Connection::connect()
{  return widget->signal_toggled().connect(SigC::slot(*this,&bool_CheckButton::Connection::controller2model));
}

bool_CheckButton::bool_CheckButton(const Model_ref<T> &m, const std::string &text)
	: Gtk::CheckButton(text), conn(m,this)
{  
}

bool_CheckButton::bool_CheckButton(const Model_ref<T> &m, Gtk::Widget &w)
	: conn(m,this)
{  Gtk::ToggleButton::add(w);
}   

