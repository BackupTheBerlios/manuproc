// $Id: SimpleTreeStore.h,v 1.45 2004/05/06 09:24:51 christof Exp $
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
#include <gtkmm/treemodel.h>
#include <RowDataBase.h>
#include <TreeRow.h>
#include <deque>
#include <gdkmm/color.h>
#include <Misc/UniqueValue.h>
#include <list>
#include <utility>
#include <map>
#include <BaseObjects/Model_ref_bvector.h>

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  define STS_GTKMM_22_24(a,b) b
#else
#  define STS_GTKMM_22_24(a,b) a
#endif
#define STS_VFUNC_CONST STS_GTKMM_22_24(,const)

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
	void setTitles(const std::vector<std::string> &T) {  model->setTitles(T); }
	void clear() { model->clear(); }
};

struct SimpleTreeStoreNode
{	// Multimap wegen doppelter Zeilen (auf der untersten Ebene)
	typedef std::multimap<cH_EntryValue,SimpleTreeStoreNode> map_t;
	typedef map_t::const_iterator const_iterator;
	typedef map_t::iterator iterator;

	map_t children;
	Handle<TreeRow> row; // die Datenstruktur hinter nodes
	cH_RowDataBase leafdata; // die Datenstruktur hinter leaves
//	bool expanded;
//	int expanding_column; = childrens_deep
	SimpleTreeStoreNode *parent;
	unsigned deep,childrens_deep;
	// REMEMBER to change swap implementation if you add things here!
	
	// root?
	// columns? pics?
	// color, color_set?
	// background
	// value ist in map gespeichert (key)

	SimpleTreeStoreNode(guint _deep=0, SimpleTreeStoreNode *_parent=0, 
			const cH_RowDataBase &v=cH_RowDataBase(),
			unsigned c_deep=0) 
	        : leafdata(v),  
	          parent(_parent), deep(_deep), childrens_deep(c_deep) {}
		
	void swap(SimpleTreeStoreNode &b);
	void fix_pointer();
};

namespace std { // sigh
void swap(SimpleTreeStoreNode &a,SimpleTreeStoreNode &b);
};

class SimpleTreeStore : public Glib::Object, public Gtk::TreeModel, public SimpleTreeModel_Proxy
{public:

	// einen neuen Ast erzeugen, deep ist die Spalte, v der Wert dieser Spalte
	typedef Handle<TreeRow> (*NewNode_fp)(const Handle<const TreeRow> &suminit);
	typedef SimpleTreeStoreNode Node;

	// sadly there's no real const_iterator
	typedef Node::const_iterator const_iterator;
	typedef Node::iterator iterator;
	typedef std::deque<guint> sequence_t;
	// first value is ":order", second is ":visible"

	static const UniqueValue::value_t trace_channel;
	static const unsigned invisible_column=unsigned(-1);
protected:
        static std::pair<std::string,std::string> default_load(const std::string&program, const std::string&instance);
        static void default_save(const std::string&program, const std::string&instance, const std::pair<std::string,std::string>&value);
        
	friend class SimpleTree_Basic;
	sequence_t currseq; 
	SimpleTreeStoreNode root;

private:
	NewNode_fp node_creation;

	guint columns;
	guint max_column;
	guint showdeep;  // nicht hier ben�tigt
	std::vector<bool> vec_hide_cols; // index is index
	gpointer gp;

	bool auffuellen_bool; 
	bool expandieren_bool;
	bool block_save;
	bool color_bool; // or in Widget?, kann in SimpleTree, muss dann aber mitgespeichert werden

	std::string mem_prog,mem_inst;
	
	std::vector<Gdk::Color> colors;
	static const unsigned num_colors=8;
	unsigned sortierspalte;
	bool invert_sortierspalte;

	unsigned stamp;

	void save_remembered() const;
	void load_remembered();

	SigC::Signal1<void,guint> title_changed;
	SigC::Signal0<void> spaltenzahl_geaendert;
	SigC::Signal1<void,gpointer> signal_save;
	SigC::Signal1<void,bvector_iterator> signal_visibly_changed;
	void save_remembered1(gpointer) { save_remembered(); }
	void on_title_changed(guint idx);
	void on_visibly_changed(bvector_iterator it);
	
	SigC::Signal0<void> needs_redisplay;
	void redisplay();
	void insertLine(Node &parent,const cH_RowDataBase &d, 
			sequence_t::const_iterator q,guint deep,
			bool summe_aktualisieren);
	iterator MoveTree(iterator current_iter,
		guint deep,guint child_s_deep,guint value_index,bool live);
	
	void on_line_appended(cH_RowDataBase);
	void on_line_removed(cH_RowDataBase);
	std::list<iterator> find_row(const cH_RowDataBase &,bool optimize=false);
	bool find_row(Node &parent,const cH_RowDataBase &,bool optimize,std::list<iterator> &result);

   typedef STS_GTKMM_22_24(const GtkTreeIter*,const TreeModel::iterator&) vfunc_constiter_t;
   typedef STS_GTKMM_22_24(GtkTreeIter*,TreeModel::iterator&) vfunc_iter_t;

   int IterStamp() const;
   iterator &iterconv(vfunc_iter_t iter);
   const iterator &iterconv(vfunc_constiter_t iter) const;
   bool iter_valid(vfunc_constiter_t iter) const;
   void iterinit(vfunc_iter_t iter,const iterator &schema) const;
   void iterclear(vfunc_iter_t iter) const;
   void iterinit(vfunc_iter_t iter,const const_iterator &schema) const;
   Path getPath(iterator it) const;
   TreeModel::iterator getIter(iterator it) const;
   
   iterator iterbyNode(Node &nd) const;
   iterator iterbyValue(Node &parent,const cH_EntryValue &val) const;
   unsigned Node2nth_child(const Node &nd) const;

// vfunc overrides for my tree model
   virtual Gtk::TreeModelFlags get_flags_vfunc() STS_VFUNC_CONST;
   virtual int get_n_columns_vfunc() STS_VFUNC_CONST;
   virtual GType get_column_type_vfunc(int index) STS_VFUNC_CONST;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   virtual void get_value_vfunc(vfunc_constiter_t iter, int column, Glib::ValueBase& value) const;
   bool iter_next_vfunc(vfunc_constiter_t iter, vfunc_iter_t iter_next) const;
   virtual bool iter_children_vfunc(vfunc_constiter_t parent, vfunc_iter_t iter) const;
   virtual bool iter_has_child_vfunc(vfunc_constiter_t iter) const;
   virtual int iter_n_children_vfunc(vfunc_constiter_t iter) const;
   virtual int iter_n_root_children_vfunc() const;
   virtual bool iter_nth_child_vfunc(vfunc_constiter_t parent, int n, vfunc_iter_t iter) const;
   virtual bool iter_nth_root_child_vfunc(int n, vfunc_iter_t iter) const;
   virtual bool iter_parent_vfunc(vfunc_constiter_t child, vfunc_iter_t iter) const;
   virtual bool get_iter_vfunc(const Path& path, vfunc_iter_t iter) const;
#else
   virtual void get_value_vfunc(const TreeModel::iterator& iter, int column, GValue* value);
   virtual bool iter_next_vfunc(vfunc_iter_t iter);
   virtual bool iter_children_vfunc(vfunc_iter_t iter, vfunc_constiter_t parent);
   virtual bool iter_has_child_vfunc(vfunc_constiter_t iter);
   virtual int iter_n_children_vfunc(vfunc_constiter_t iter);
   virtual bool iter_nth_child_vfunc(vfunc_iter_t iter, vfunc_constiter_t parent, int n);
   virtual bool iter_parent_vfunc(vfunc_iter_t iter, vfunc_constiter_t child);
   virtual bool get_iter_vfunc(vfunc_iter_t iter, const Path& path);
#endif
   virtual Path get_path_vfunc(const TreeModel::iterator& iter) STS_VFUNC_CONST;
   
   void resort(SimpleTreeStoreNode&, unsigned);
   void test();
   void test_sub(unsigned indent,const GtkTreeIter *i,const GtkTreeIter *parent);
   
	enum e_spalten
	{  s_row, s_deep, s_childrens_deep, s_leafdata, s_background,
	   s_text_start
        };

	SimpleTreeStore(int max_col); // use create instead of this ctor
public:
	struct ModelColumns : public Gtk::TreeModelColumnRecord
	{  // since we would also need to 
	   Gtk::TreeModelColumn<Handle<TreeRow> > row;
	   // our first printing column
	   Gtk::TreeModelColumn<guint> deep;
	   // childrens_deep=0 -> Leaf
	   Gtk::TreeModelColumn<guint> childrens_deep;
	   // the node's value (at deep)
	   // Gtk::TreeModelColumn<cH_EntryValue> value;
	   // if we're a node this is not 'our' data
	   Gtk::TreeModelColumn<cH_RowDataBase> leafdata;
	   
	   Gtk::TreeModelColumn<Gdk::Color> background;
	   
	   std::vector<Gtk::TreeModelColumn<Glib::ustring> > cols;
	   
	   ModelColumns(int cols);
	};
	
	ModelColumns m_columns;

	static Glib::RefPtr<SimpleTreeStore> create(int max_colidx);
	
	void set_showdeep(int i) {showdeep=i;}
	guint Cols() const  { return columns;}
	guint MaxCol() const  { return max_column;}

	void set_value_data(gpointer _p) {gp = _p;}
	gpointer ValueData() const { return gp; }

	const sequence_t &get_seq() const {return currseq;}
	void defaultSequence();
	void fillSequence(sequence_t &seq,bool standard=false) const;
	void fillSequence() { fillSequence(currseq,true); }

	void set_remember(const std::string &program, const std::string &instance);
	SigC::Signal1<void,guint> &signal_title_changed()
	{  return title_changed; }
	SigC::Signal0<void> &signal_spaltenzahl_geaendert()
	{  return spaltenzahl_geaendert; }
	const std::string getColTitle(guint idx) const;
	
	void set_NewNode(NewNode_fp n)
	{  node_creation=n; }
	
	const_iterator begin() const
	{  return root.children.begin();
	}
	const_iterator end() const
	{  return root.children.end();
	}
	TreeModel::const_iterator getIter(const_iterator it) const;

	void setSequence(const sequence_t &seq);
	
	unsigned ColumnFromIndex(unsigned) const;
	unsigned IndexFromColumn(unsigned c) const
	{  return currseq[c]; }
	
	bool ColumnVisible(unsigned idx) const
	{  return vec_hide_cols.at(idx); }
	void set_tree_column_visibility(unsigned index,bool visible);
	
	void redisplay_old(cH_RowDataBase row, unsigned index);
	
	// all lines have changed - redisplay is needed!
	SigC::Signal0<void> &signal_redisplay() {  return needs_redisplay; }
	
	// these are accessors for SimpleTreeStates
	Model_ref<guint> ShowDeep() { return Model_ref<guint>(showdeep,signal_save); }
//	__deprecated__ Model_ref<bool> ShowColor() { return Model_ref<bool>(color_bool,signal_save); }
	Model_ref<bool> OptionColor() { return Model_ref<bool>(color_bool,signal_save); }
	Model_ref<bool> OptionAuffullen() { return Model_ref<bool>(auffuellen_bool,signal_save); }
	Model_ref<bool> OptionExpandieren() { return Model_ref<bool>(expandieren_bool,signal_save); }
	Model_ref<bvector_item> ShowColumn(unsigned idx) 
	{ return Model_ref<bvector_item>(vec_hide_cols.begin()+idx,signal_visibly_changed); }
	
	unsigned visible_size() { return currseq.size(); }
	void setSortierspalte(unsigned idx=invisible_column, bool invert=false);
	bool getInvert() const { return invert_sortierspalte; }	
};

#endif
