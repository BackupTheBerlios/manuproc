// $Id: logwin.cc,v 1.8 2004/01/31 23:12:14 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include "logwin.h"
#include <gtkmm/adjustment.h>

void logwin::scroll() throw()
{  Gtk::Adjustment *adj=static_cast<Gtk::Adjustment *>(get_vadjustment());
   if (adj) adj->set_value(adj->gobj()->upper);
}

logwin::logwin(guint minimum_size) : color()
{  emu.attach_to(gtklist);
   add(gtklist);
   gtklist.show();
   set_size_request(-1,minimum_size);
   set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   emu.set_title("");
}

void logwin::set_color()
{  if (color) return;
   emu.add(colorcol);
   Gtk::CellRendererText *crt = dynamic_cast<Gtk::CellRendererText *>(gtklist.get_column(0)->get_first_cell_renderer());
   gtklist.get_column(0)->add_attribute(crt->property_foreground_gdk(),colorcol);
}

void logwin::append(const Glib::ustring &text, const Gdk::Color &col)
{  Gtk::TreeModel::iterator iter =get_store()->append();
   (*iter)[get_column()]=text;
   (*iter)[colorcol]=col;
}
