// $Id: SimpleTreeStore.h,v 1.8 2002/11/28 08:09:09 christof Exp $
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
#include <gtkmm/treestore.h>
#include <RowDataBase.h>
#include <TreeRow.h>
#include<deque>

// for easily accessing model methods
class SimpleTreeModel_Proxy
{protected:
	SimpleTreeModel *model;
	bool model_is_ours:1; // whether we need to destroy the model
public:
	SimpleTreeModel_Proxy();
	~SimpleTreeModel_Proxy();
	
	SimpleTreeModel &getModel() { return *model; }
	const SimpleTreeModel &getModel() const { return *model; }
	void setModel(SimpleTreeModel &model);

	void setDataVec(const std::vector<cH_RowDataBase> &d) {  model->setDataVec(d); }
	void setTitles(const std::vector<std::string>& T) {  model->setTitles(T); }
};

class SimpleTreeStore : public SigC::Object, public SimpleTreeModel_Proxy
{public:
	// einen neuen Ast erzeugen, deep ist die Spalte, v der Wert dieser Spalte
	typedef TreeRow *(*NewNode_fp)
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, 
 		 cH_RowDataBase child_s_data, bool expand, 
 		 const TreeRow &suminit);

private:
	NewNode_fp node_creation;

	Glib::RefPtr<Gtk::TreeStore> m_refTreeStore;
friend class SimpleTree;
	
	guint columns;
	guint showdeep;
	guint attrcount;
	std::deque<guint> currseq; 
	std::vector<bool> vec_hide_cols;
	gpointer gp;

	bool auffuellen_bool:1; 
	bool expandieren_bool:1; 
	bool color_bool:1; // or in Widget?

	std::string mem_prog,mem_inst;

	void save_remembered() const;
	void load_remembered();

	SigC::Signal1<void,guint> title_changed;
	void on_title_changed(guint idx);
	
	void redisplay();
	void insertLine(/* ??? */const cH_RowDataBase &d, std::deque<guint> q,guint deep);
public:
	struct ModelColumns : public Gtk::TreeModelColumnRecord
	{  std::vector<Gtk::TreeModelColumn<Glib::ustring> > cols;
	   // since we would also need to 
	   Gtk::TreeModelColumn<cH_TreeRow> row;
	   // if we're a node this is not 'our' data
//	   Gtk::TreeModelColumn<cH_RowDataBase> row; // once leafdata
//	   Gtk::TreeModelColumn<cH_EntryValue> node_val;
	   Gtk::TreeModelColumn<guint> deep;
	   // childrens_deep=0 -> Leaf
//	   Gtk::TreeModelColumn<guint> childrens_deep;
	   
	   ModelColumns(int cols);
	};
	
	ModelColumns m_columns;

	SimpleTreeStore(int cols,int attrs=-1);
	
	void set_showdeep(int i) {showdeep=i;}
	guint Attrs() const { return attrcount; }
	guint Cols() const  { return columns;}

	void set_value_data(gpointer _p) {gp = _p;}
	gpointer ValueData() const { return gp; }

	const std::deque<guint> &get_seq() const {return currseq;}
	void defaultSequence();

	void set_remember(const std::string &program, const std::string &instance);
	SigC::Signal1<void,guint> &signal_title_changed()
	{  return title_changed; }
	const std::string getColTitle(guint idx) const;
};

#endif
