// $Id: char_Pixmap.cc,v 1.4 2003/03/07 08:10:25 christof Exp $
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

#include "char_Pixmap.hh"

void char_Pixmap::refresh(gpointer x)
{
  if (model.matches(x))
   { set(Gdk::Pixbuf::create_from_xpm_data(model.get_value()));
   }
}

char_Pixmap::char_Pixmap(const Model_ref<T> &m)
	:  Gtk::Image(Gdk::Pixbuf::create_from_xpm_data(m.get_value())),model(m)
{  
 init();
};

void char_Pixmap::init()
{
  ch_con=model.signal_changed().connect(SigC::slot(*this,&char_Pixmap::refresh));
}

