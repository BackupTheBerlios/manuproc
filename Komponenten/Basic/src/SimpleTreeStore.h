// $Id: SimpleTreeStore.h,v 1.18 2002/12/05 08:39:19 christof Exp $
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

	void setDataVec(const SimpleTreeModel::datavec_t &d) {  model->setDataVec(d); }
	const SimpleTreeModel::datavec_t &getDataVec() const { return model->getDataVec(); }
	void setTitles(const std::vector<std::string>& T) {  model->setTitles(T); }
	void clear() { model->clear(); }
};

class SimpleTreeStore : public SigC::Object, public SimpleTreeModel_Proxy
{public:
	// einen neuen Ast erzeugen, deep ist die Spalte, v der Wert dieser Spalte
	typedef Handle<TreeRow> (*NewNode_fp)(const Handle<const TreeRow> &suminit);
	// sadly there's no real const_iterator
	typedef Gtk::TreeStore::iterator const_iterator;
	typedef Gtk::TreeStore::iterator iterator;

private:
	NewNode_fp node_creation;

	Glib::RefPtr<Gtk::TreeStore> m_refTreeStore;
	friend class SimpleTree_Basic;
	
	guint columns;
	guint showdeep;
	std::deque<guint> currseq; 
	std::vector<bool> vec_hide_cols;
	gpointer gp;

	bool auffuellen_bool:1; 
	bool expandieren_bool:1; 
	bool color_bool:1; // or in Widget?

	std::string mem_prog,mem_inst;
	
//	std::vector<Gdk_Color> colors;
	static const unsigned num_colors=8;

	void save_remembered() const;
	void load_remembered();

	SigC::Signal1<void,guint> title_changed;
	void on_title_changed(guint idx);
	
	void redisplay();
	void SummenAnzeigen(Gtk::TreeModel::Children parent);
	void SummenAnzeigen(const Gtk::TreeRow &row,const Handle<TreeRow> &htr);
	void InitColumns(Gtk::TreeRow &node, guint deep, const cH_EntryValue &ev, 
			const cH_RowDataBase &v);
	void insertLine(Gtk::TreeModel::Children parent,const cH_RowDataBase &d, 
			std::deque<guint>::const_iterator q,guint deep,
			bool summe_aktualisieren);
	Gtk::TreeRow CopyTree(Gtk::TreeRow src, Gtk::TreeModel::Children dest);
	Gtk::TreeStore::iterator MoveTree(
		Gtk::TreeStore::iterator current_iter,
		guint deep,guint child_s_deep,guint value_index);
	
	void on_line_appended(cH_RowDataBase);
	void on_line_removed(cH_RowDataBase);
public:
	struct ModelColumns : public Gtk::TreeModelColumnRecord
	{  std::vector<Gtk::TreeModelColumn<Glib::ustring> > cols;
	   // since we would also need to 
	   Gtk::TreeModelColumn<Handle<TreeRow> > row;
	   // our first printing column
	   Gtk::TreeModelColumn<guint> deep;
	   // childrens_deep=0 -> Leaf
	   Gtk::TreeModelColumn<guint> childrens_deep;
	   // the node's value (at deep)
	   Gtk::TreeModelColumn<cH_EntryValue> value;

	   // if we're a node this is not 'our' data
	   Gtk::TreeModelColumn<cH_RowDataBase> leafdata;
	   
	   ModelColumns(int cols);
	};
	
	ModelColumns m_columns;

	SimpleTreeStore(int cols,int attrs=-1);
	
	void set_showdeep(int i) {showdeep=i;}
	guint Cols() const  { return columns;}

	void set_value_data(gpointer _p) {gp = _p;}
	gpointer ValueData() const { return gp; }

	const std::deque<guint> &get_seq() const {return currseq;}
	void defaultSequence();

	void set_remember(const std::string &program, const std::string &instance);
	SigC::Signal1<void,guint> &signal_title_changed()
	{  return title_changed; }
	const std::string getColTitle(guint idx) const;
	
	void set_NewNode(NewNode_fp n)
	{  node_creation=n; }
	
	const_iterator begin() const
	{  return m_refTreeStore->children().begin();
	}
	const_iterator end() const
	{  return m_refTreeStore->children().end();
	}

	void set_tree_column_visibility(unsigned int column,bool visible);
};

#endif
