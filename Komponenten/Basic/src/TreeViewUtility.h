// $Id: TreeViewUtility.h,v 1.10 2004/01/30 16:59:27 christof Exp $
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

#ifndef TREEVIEWUTILITY_H
#define TREEVIEWUTILITY_H

#include <vector>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

namespace TreeViewUtility {  

class CListEmulator : public Gtk::TreeModelColumnRecord
{	std::vector<Gtk::TreeModelColumn<Glib::ustring> > cols;
	std::vector<Glib::ustring> titles;
	Glib::RefPtr<Gtk::ListStore> m_refStore;
	Gtk::TreeView *view;
public:	
	CListEmulator() {}
	// CList like
	CListEmulator(const std::vector<Glib::ustring> &titles);
	// List like
	CListEmulator(const Glib::ustring &title);
	const Gtk::TreeModelColumn<Glib::ustring> &Column(unsigned i) const
	{  return cols.at(i); }
	void attach_to(Gtk::TreeView &tv);
	const std::vector<Gtk::TreeModelColumn<Glib::ustring> > &Columns() const
	{  return cols; }
	Glib::RefPtr<Gtk::ListStore> get_store()
	{ return m_refStore; }
	void set_titles(const std::vector<Glib::ustring> &titles);
	// one column only
	void set_title(const Glib::ustring &title);
	
	int get_selected_row_num() const;
	bool selection_empty() const;
	void set_selection_mode(Gtk::SelectionMode x)
	{  view->get_selection()->set_mode(x); }
	void set_column_justification(int col, Gtk::AlignmentEnum just)
	{  view->get_column(col)->set_alignment(just);
	}
	void set_column_justification(int col, Gtk::Justification just);
	void column_titles_passive() {}
	void append(const gchar* text[]);
};

class CList : public Gtk::TreeView
{	CListEmulator emu;
public:
	CList(const std::vector<Glib::ustring> &titles);
	CList(const char *title1,...);
	const Gtk::TreeModelColumn<Glib::ustring> &Column(unsigned i) const
	{  return emu.Column(i); }
	Glib::RefPtr<Gtk::ListStore> get_store()
	{ return emu.get_store(); }
};

}
#endif
