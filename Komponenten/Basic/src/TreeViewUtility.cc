// $Id: TreeViewUtility.cc,v 1.3 2003/03/17 17:59:26 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include "TreeViewUtility.h"

TreeViewUtility::CListEmulator::CListEmulator(const std::vector<Glib::ustring> &t)
	: cols(t.size()), titles(t)
{  for (std::vector<Gtk::TreeModelColumn<Glib::ustring> >::iterator i=cols.begin();
		i!=cols.end();++i)
      add(*i);
   m_refStore=Gtk::ListStore::create(*this);
}

TreeViewUtility::CListEmulator::CListEmulator(const Glib::ustring &title)
{  cols.push_back(Gtk::TreeModelColumn<Glib::ustring>());
   add(cols.back());
   titles.push_back(title);
   m_refStore=Gtk::ListStore::create(*this);
}

void TreeViewUtility::CListEmulator::attach_to(Gtk::TreeView &tv)
{  tv.set_model(m_refStore);
   for (unsigned i=0;i<cols.size();++i)
      tv.append_column(titles.at(i),cols.at(i));
}

TreeViewUtility::CListEmulator2::CListEmulator2(const std::vector<Glib::ustring> &titles)
  : emu(titles)
{  emu.attach_to(*this);
}
