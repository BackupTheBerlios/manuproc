// $Id: TreeViewUtility.h,v 1.18 2004/06/30 13:26:22 christof Exp $
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
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

// see webkartei/src/Kette.cc for a nice example on how to use this

namespace TreeViewUtility {  

class CListEmulator : public SigC::Object, public Gtk::TreeModelColumnRecord
{	std::vector<Gtk::TreeModelColumn<Glib::ustring> > cols;
	std::vector<Glib::ustring> titles;
	Glib::RefPtr<Gtk::ListStore> m_refStore;
	Gtk::TreeView *view;
	
	SigC::Signal2<void,int,int> select_row_sig;
	bool button_press_handler(GdkEventButton *event);
	SigC::Connection con;
public:	
	CListEmulator(const std::vector<Glib::ustring> &titles);
	// List like
	CListEmulator(const Glib::ustring &title="");
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
	void set_column_justification(int col, Gtk::AlignmentEnum just);
	void set_column_justification(int col, Gtk::Justification just);
	void column_titles_passive() { view->set_headers_clickable(false); }
	Gtk::TreeModel::iterator append(const gchar* text[]);
	Gtk::TreeModel::iterator append(const std::vector<Glib::ustring> &text);
	void freeze() {}
	void thaw() {}
	void clear() { get_store()->clear(); }
	
	void add(Gtk::TreeModelColumnBase& column);
	// clicking on a cell
	SigC::Signal2<void,int,int> &signal_select_row() { return select_row_sig; }
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

void MakeColumnEditable(Gtk::TreeView *tv,unsigned col,const SigC::Slot2<void,const Glib::ustring&,const Glib::ustring&> &slot);

}
#endif
