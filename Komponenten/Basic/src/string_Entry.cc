// $Id: string_Entry.cc,v 1.7 2004/05/03 07:53:27 christof Exp $
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

#include "string_Entry.hh"
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

void string_Entry::Connection::keypress()
{  any_change=true;
}

void string_Entry::Connection::on_activate()
{  on_focus_out(0);
}

bool string_Entry::Connection::on_focus_out(GdkEventFocus *ev)
{  if (any_change) controller2model();
//   select_region(0,0); // needed ?
   return false;
}

bool string_Entry::Connection::on_focus_in(GdkEventFocus *ev)
{  widget->select_region(0,-1);
   return false;
}

void string_Entry::Connection::model2widget()
{  widget->set_text(model.get_value());
   any_change=false;
}

void string_Entry::Connection::widget2model()
{  model=widget->get_text();
   any_change=false;
}

SigC::Connection string_Entry::Connection::connect()
{  cm_con2[0]=widget->signal_focus_out_event().connect(SigC::slot(*this,&string_Entry::Connection::on_focus_out),true);
   cm_con2[1]=widget->signal_focus_in_event().connect(SigC::slot(*this,&string_Entry::Connection::on_focus_in),true);
   // I'm not quite sure whether this is needed at all
   cm_con2[2]=widget->signal_activate().connect(SigC::slot(*this,&string_Entry::Connection::on_activate),true);
   return widget->signal_changed().connect(SigC::slot(*this,&string_Entry::Connection::keypress));
}

void string_Entry::Connection::disconnect()
{  cm_con2[0].disconnect();
   cm_con2[1].disconnect();
   cm_con2[2].disconnect();
   this_t::disconnect();
}

string_Entry::string_Entry(const Model_ref<T> &m)
	: conn(m,this)
{  
}
