// $Id: SimpleTreeStore.h,v 1.1 2002/10/18 10:39:42 christof Exp $
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

#ifndef SIMPLE_TREE_STORE_H
#define SIMPLE_TREE_STORE_H

#include <SimpleTreeModel.h>
#include <gtkmm/TreeStore.h>
#include<deque>

class SimpleTreeStore : Gtk::TreeStore
{public:
	// einen neuen Ast erzeugen, deep ist die Spalte, v der Wert dieser Spalte
	typedef TreeRow *(*NewNode_fp)
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, 
 		 cH_RowDataBase child_s_data, bool expand, 
 		 const TreeRow &suminit);

private:
	SimpleTreeModel *model;
	NewNode_fp node_creation;
	
	guint showdeep;
	guint attrcount;
	std::deque<guint> currseq; 
	std::vector<bool> vec_hide_cols;
	gpointer gp;

	bool color_bool:1; // or in Widget?
	bool model_is_ours:1; // whether we need to destroy the model

	std::string mem_prog,mem_inst;

	void save_remembered() const;
	void load_remembered();
protected:

pubic:
	void set_showdeep(int i) {showdeep=i;}
	guint Attrs() const { return attrcount; }
	guint Cols() const { return columns().size();}

	void set_value_data(gpointer _p) {gp = _p;}
	gpointer ValueData() const { return gp; }

	const std::deque<guint> &get_seq() const {return currseq;}

	void set_remember(const std::string &program, const std::string &instance);
};

#endif
