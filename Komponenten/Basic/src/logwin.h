// $Id: logwin.h,v 1.6 2002/12/16 08:29:33 christof Exp $
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

#ifndef LOGWIN_HH
#define LOGWIN_HH

#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/viewport.h>
#include <TreeViewUtility.h>

class logwin : public Gtk::ScrolledWindow
{  	Gtk::TreeView gtklist;
	TreeViewUtility::CListEmulator emu;
   public:
	logwin(guint minimum_size=200);
	Glib::RefPtr<Gtk::ListStore> get_list()
	{ return emu.get_list(); } 
	const Gtk::TreeModelColumn<Glib::ustring> &get_column() const
	{ return emu.Column(0); }
	void scroll() throw();
}; 
#endif
