// $Id: MVC_int_Widget.cc,v 1.1 2002/07/08 08:25:20 christof Exp $
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

#include "MVC_int_Widget.hh"
#include "gtk--/adjustment.h"

void MVC_int_Widget::refresh(gpointer x)
{  if (x==&model.Value())
   {  my_ch_con.disconnect();
      Gtk::SpinButton::set_value(model.get_value());
      any_change=false;
      my_ch_con=changed.connect(SigC::slot(this,&MVC_int_Widget::keypress));
   }
}

gint MVC_int_Widget::on_focus_out(GdkEventFocus *ev)
{  if (any_change)
   {  ch_con.disconnect();
      update();
      model=Gtk::SpinButton::get_value_as_int();
      ch_con=model.changed.connect(SigC::slot(this,&MVC_int_Widget::refresh));
      any_change=false;
   }
//   select_region(0,0); // needed ?
   return false;
}

gint MVC_int_Widget::on_focus_in(GdkEventFocus *ev)
{  select_region(0,-1);
   return false;
}

MVC_int_Widget::MVC_int_Widget(const Model_ref<T> &m,T min,T max)
	: Gtk::SpinButton(1), any_change(false), model(m)
{  set_update_policy(GTK_UPDATE_ALWAYS);
   set_numeric(true);
   get_adjustment()->set_lower(min);
   get_adjustment()->set_upper(max);
   Gtk::SpinButton::set_value(model.get_value());
   focus_out_event.connect_after(SigC::slot(this,&MVC_int_Widget::on_focus_out));
   focus_in_event.connect_after(SigC::slot(this,&MVC_int_Widget::on_focus_in));
   // I'm not quite sure whether this is needed at all
   activate.connect_after(SigC::slot(this,&MVC_int_Widget::on_activate));
   ch_con=model.changed.connect(SigC::slot(this,&MVC_int_Widget::refresh));
   my_ch_con=changed.connect(SigC::slot(this,&MVC_int_Widget::keypress));
};

void MVC_int_Widget::keypress()
{  any_change=true;
}

void MVC_int_Widget::on_activate()
{  on_focus_out(0);
}

