// $Id: logwin.cc,v 1.5 2002/12/12 09:30:43 christof Exp $
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

logwin::logwin(guint minimum_size)
{  m_refStore=Gtk::ListStore::create(m_columns);
   add(gtklist);
   gtklist.show();
   set_size_request(-1,minimum_size);
   set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   gtklist.set_model(m_refStore);
   gtklist.append_column("", m_columns.col);
}

logwin::ModelColumns::ModelColumns()
{  add(col);
}
