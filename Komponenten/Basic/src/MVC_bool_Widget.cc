// $Id: MVC_bool_Widget.cc,v 1.3 2002/09/27 09:48:44 christof Exp $
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

#include "MVC_bool_Widget.hh"

void MVC_bool_Widget::refresh(gpointer x)
{  if (x==&model.Value())
   {  my_ch_con.disconnect();
      Gtk::CheckButton::set_active(model.get_value());
      my_ch_con=toggled.connect(SigC::slot(this,&MVC_bool_Widget::on_toggled));
   }
}

MVC_bool_Widget::MVC_bool_Widget(const Model_ref<T> &m, const std::string &text,gfloat x=0.5,gfloat y=0.5)
	: Gtk::CheckButton(text,x,y), model(m)
{  Gtk::ToggleButton::set_active(m.get_value());
   my_ch_con=toggled.connect(SigC::slot(this,&MVC_bool_Widget::on_toggled));
   ch_con=model.changed.connect(SigC::slot(this,&MVC_bool_Widget::refresh));
};

void MVC_bool_Widget::on_toggled()
{  ch_con.disconnect();
   model=Gtk::CheckButton::get_active();
   ch_con=model.changed.connect(SigC::slot(this,&MVC_bool_Widget::refresh));
}
