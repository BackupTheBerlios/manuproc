// $Id: int_RadioButtons.cc,v 1.2 2002/12/03 09:09:05 christof Exp $
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

#include "MVC_int_RadioButtons.hh"

void MVC_int_RadioButtons::refresh(gpointer x)
{  if (x==&model.Value())
   {  my_ch_con.disconnect();
      Gtk::CheckButton::set_active(model.get_value());
      my_ch_con=toggled.connect(SigC::slot(this,&MVC_int_RadioButtons::on_toggled));
   }
}

MVC_int_RadioButtons::MVC_int_RadioButtons(const Model_ref<T> &m, const std::string &text)
	: Gtk::CheckButton(text), model(m)
{  Gtk::ToggleButton::set_active(m.get_value());
   my_ch_con=toggled.connect(SigC::slot(this,&MVC_int_RadioButtons::on_toggled));
   ch_con=model.changed.connect(SigC::slot(this,&MVC_int_RadioButtons::refresh));
};

void MVC_int_RadioButtons::on_toggled()
{  ch_con.disconnect();
   model=Gtk::CheckButton::get_active();
   ch_con=model.changed.connect(SigC::slot(this,&MVC_int_RadioButtons::refresh));
}
