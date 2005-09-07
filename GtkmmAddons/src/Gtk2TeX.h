/*  Gtk--addons: a collection of gtk-- addons
 *  Copyright (C) 2001-2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// $Id: Gtk2TeX.h,v 1.10 2005/09/07 16:22:52 christof Exp $

#include <TeX.h>
#include <iostream>
#include <gtkmm/treeview.h>

namespace Gtk2TeX
{	using namespace TeX;

	typedef std::string (*ColumnType_cb)(int col,const std::string &typ,const std::string &title,gpointer user_data);
	typedef std::string (*ColumnTitle_cb)(int col,const std::string &title,gpointer user_data);
	typedef std::string (*Element_cb)(const Gtk::TreeModel::const_iterator &r,int col,const std::string &text,gpointer user_data);
	typedef std::string (*FirstRow_cb)(gpointer user_data, int deep, std::deque<guint> seq);
	typedef void (*Row_cb)(std::ostream &o,const Gtk::TreeModel::const_iterator &r,gpointer user_data);
	struct TableFlags
	{	bool longtable:1;
		bool headline:1; // output ...\endhead
		bool environment:1; // output \begin{}{}...\end{}
		bool multicolumn:1; // let columns grow into next ones if empty
		ColumnType_cb columntype_cb;
		Row_cb preline_cb,postline_cb;
		Simple_cb prehead_cb,posthead_cb,postlist_cb;
		FirstRow_cb firstrow_cb;
		int deep;
		std::deque<guint> sequence; // sequence of cols

		// e.g. by tv->get_selection()->get_selected();
		// because of a gtkmm bug :-( you have to use
		// const_cast<Gtk::TreeIter&>(tf.selection)
		//	=tv->get_selection()->get_selected();
//		Gtk::TreeModel::const_iterator selection;

		// This is much easier:
		// tf.selection=tv->get_selection()->get_selected_rows();
		Gtk::TreeSelection::ListHandle_Path selection;
		bool single_row_with_children;
		
		// Glib::RefPtr<Gtk::TreeSelection> selection;
		// there's no selection ctor ... so 
//		gint first_line,last_line; // perhaps as paths ?
		gpointer user_data;
		ColumnTitle_cb columntitle_cb;
		Element_cb element_cb;
	
		TableFlags()
		: longtable(true), headline(true), environment(true),
		  multicolumn(),
		  columntype_cb(), preline_cb(), postline_cb(),
		  prehead_cb(), posthead_cb(), postlist_cb(),
                  firstrow_cb(),
//		  first_line(0), last_line(-1),
		  selection(0,Glib::OWNERSHIP_NONE),
		  single_row_with_children(),
		  user_data(), columntitle_cb(),
		  element_cb()
		{}
	};
	
	std::ostream &TreeView2Table(std::ostream &os,const Gtk::TreeView *cl,const TableFlags &fl=TableFlags());
};
