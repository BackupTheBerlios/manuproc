// $Id: SimpleTreeStore.h,v 1.61 2005/11/07 07:29:27 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
#include <gtkmm/treedragsource.h>
#include <gtkmm/treedragdest.h>
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

#ifndef SIMPLE_TREE_NO_COMPAT
#define ST_DEPRECATED
#endif

struct SimpleTreeModel_Properties
{ enum column_type_t { ct_string, ct_bool };

  virtual ~SimpleTreeModel_Properties() {}
  virtual unsigned Columns() const=0;
  virtual gpointer ValueData() const
  { return 0; }
  virtual Glib::ustring Title(guint _seqnr) const=0;
  virtual gfloat Alignment(guint _seqnr) const
  { return 0.0; }
  virtual bool editable(guint _seqnr) const
  { return false; }
  virtual Handle<TreeRow> create_node(const Handle<const TreeRow> &suminit) const
  { return Handle<TreeRow>(); }
  virtual bool ColumnsAreEquivalent() const
  { return true; }
  virtual column_type_t get_column_type(unsigned idx) const
  { return ct_string; }
  virtual std::string ProgramName() const { return std::string(); }
  virtual std::string InstanceName() const { return std::string(); }
  /*virtual */ bool resizable(guint _seqnr) const { return true; }
  /* fixed_width etc. ? */
};

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

	__deprecated void setDataVec(const SimpleTreeModel::datavec_t &d) {  model->setDataVec(d); }
	__deprecated const SimpleTreeModel::datavec_t &getDataVec() const { return model->getDataVec(); }
//	void setTitles(const std::vector<std::string> &T) {  model->setTitles(T); }
//	void setTitleAt(unsigned idx, const std::string &s) {  model->setTitleAt(idx,s); }
	void clear() { model->clear(); }
};

class SimpleTreeModel_Properties_Proxy
{protected:
	SimpleTreeModel_Properties *props;
	bool we_own_props;
	SigC::Signal1<void,guint> title_changed;
public:
	SimpleTreeModel_Properties_Proxy(unsigned cols);
	SimpleTreeModel_Properties_Proxy(SimpleTreeModel_Properties *p)
	: props(p), we_own_props() {}
	~SimpleTreeModel_Properties_Proxy();
	
	void setProperties(SimpleTreeModel_Properties &p);
	const SimpleTreeModel_Properties &Properties() const
	{ return *props; }
	class Standard;
	Standard &stdProperties();
	const std::string getColTitle(guint idx) const
	{ return Properties().Title(idx); }
	SigC::Signal1<void,guint> &signal_title_changed()
	{  return title_changed; }
	bool is_editable(unsigned idx) const
	{ return Properties().editable(idx); }
	SimpleTreeModel_Properties::column_type_t get_column_type(unsigned idx) const
	{ return Properties().get_column_type(idx); }
	gpointer ValueData() const { return Properties().ValueData(); }
  std::string ProgramName() const { return Properties().ProgramName(); }
  std::string InstanceName() const { return Properties().InstanceName(); }
  Handle<TreeRow> create_node(const Handle<const TreeRow> &suminit) const
  { return Properties().create_node(suminit); }
  bool ColumnsAreEquivalent() const { return Properties().ColumnsAreEquivalent(); }
#ifdef ST_DEPRECATED
	typedef Handle<TreeRow> (*NewNode_fp)(const Handle<const TreeRow> &suminit);
	__deprecated void setTitles(const std::vector<std::string> &T);
	__deprecated void setTitleAt(unsigned idx,const std::string &s);
	__deprecated void set_editable(unsigned idx,bool v=true);
	__deprecated void set_column_type(unsigned idx, SimpleTreeModel_Properties::column_type_t t);
	__deprecated void set_value_data(gpointer _p);
	__deprecated void set_remember(const std::string &program, const std::string &instance);
	__deprecated void set_NewNode(NewNode_fp n);
	__deprecated void RedisplayOnReorder();
#endif
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

class SimpleTreeStore : virtual public Glib::Object, public Gtk::TreeModel, 
		public Gtk::TreeDragSource, public Gtk::TreeDragDest,
		public SimpleTreeModel_Proxy,
		public SimpleTreeModel_Properties_Proxy
{public:

	// einen neuen Ast erzeugen, deep ist die Spalte, v der Wert dieser Spalte
#ifdef ST_DEPRECATED
	typedef SimpleTreeModel_Properties_Proxy::NewNode_fp NewNode_fp;
#endif
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

	guint columns;
	guint max_column;
	guint showdeep;  // nicht hier ben�igt
	std::vector<bool> vec_hide_cols; // index is index

	bool auffuellen_bool; 
	bool expandieren_bool;
	bool block_save;
	bool color_bool; // or in Widget?, kann in SimpleTree, muss dann aber mitgespeichert werden

	std::vector<Gdk::Color> colors;
	static const unsigned num_colors=8;
	// vielleicht noch in Properties?
	unsigned sortierspalte;
	bool invert_sortierspalte;

	unsigned stamp;

	void save_remembered() const;
	void load_remembered();

	SigC::Signal1<void,guint> title_changed;
	SigC::Signal0<void> please_detach;
	SigC::Signal0<void> please_attach;
	SigC::Signal0<void> spaltenzahl_geaendert;
	SigC::Signal1<void,gpointer> signal_save;
	SigC::Signal1<void,bvector_iterator> signal_visibly_changed;
	void save_remembered1(gpointer) { save_remembered(); }
	void on_title_changed(guint idx);
	void on_visibly_changed(bvector_iterator it);
	void value_change_impl(cH_RowDataBase row,unsigned idx,std::string const& newval, bool &has_changed);
	
//	SigC::Signal0<void> needs_redisplay;
	void redisplay();
	void init();
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

   unsigned IterStamp() const;
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
   virtual bool row_draggable_vfunc(const Gtk::TreeModel::Path& path) const;
   virtual bool drag_data_get_vfunc(const Gtk::TreeModel::Path& path, Gtk::SelectionData& selection_data) const;
   virtual bool drag_data_delete_vfunc(const Gtk::TreeModel::Path& path);
   virtual bool drag_data_received_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data);
   virtual bool row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data) const;

   virtual Gtk::TreeModelFlags get_flags_vfunc() STS_VFUNC_CONST;
   virtual int get_n_columns_vfunc() STS_VFUNC_CONST;
   virtual GType get_column_type_vfunc(int index) STS_VFUNC_CONST;
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
   virtual Path get_path_vfunc(const TreeModel::iterator& iter) STS_VFUNC_CONST;
   
   void resort(SimpleTreeStoreNode&, unsigned);
   void test();
   void test_sub(unsigned indent,const GtkTreeIter *i,const GtkTreeIter *parent);
   
	enum e_spalten
	{  s_row, s_deep, s_childrens_deep, s_leafdata, s_background,
	   s_text_start
        };

	SimpleTreeStore(int max_col); // use create instead of this ctor
	SimpleTreeStore(SimpleTreeModel_Properties &props);
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
	static Glib::RefPtr<SimpleTreeStore> create(SimpleTreeModel_Properties &props);
#ifdef ST_DEPRECATED
	__deprecated void set_remember(const std::string &program, const std::string &instance);
#endif
	
	void set_showdeep(int i) {showdeep=i;}
	guint Cols() const  { return columns;}
	guint MaxCol() const  { return max_column;}

	const sequence_t &get_seq() const {return currseq;}
	void defaultSequence();
	void fillSequence(sequence_t &seq,bool standard=false) const;
	void fillSequence() { fillSequence(currseq,true); }

	SigC::Signal1<void,guint> &signal_title_changed()
	{  return title_changed; }
	SigC::Signal0<void> &signal_spaltenzahl_geaendert()
	{  return spaltenzahl_geaendert; }
	SigC::Signal0<void> &signal_please_detach()
	{  return please_detach; }
	SigC::Signal0<void> &signal_please_attach()
	{  return please_attach; }
	const std::string getColTitle(guint idx) const;
	
	const_iterator begin() const
	{  return root.children.begin();
	}
	const_iterator end() const
	{  return root.children.end();
	}
	TreeModel::const_iterator getIter(const_iterator it) const;
	// get the path for a given data line
        Path getPath(const cH_RowDataBase &data) const;

	void setSequence(const sequence_t &seq);
	
	unsigned ColumnFromIndex(unsigned) const;
	unsigned IndexFromColumn(unsigned c) const
	{  return currseq[c]; }
	
	bool ColumnVisible(unsigned idx) const
	{  return vec_hide_cols.at(idx); }
	void set_tree_column_visibility(unsigned index,bool visible);
	
	// better use the new API (which maintains a consistent tree (e.g.sums))
	// getModel()->about_to_change(row); XYZ; getModel()->has_changed(row);
	void redisplay_old(cH_RowDataBase row, unsigned index);
	void redisplay_old(Gtk::TreeModel::iterator row, unsigned index);
	
	// all lines have changed - redisplay is needed!
//	SigC::Signal0<void> &signal_redisplay() {  return needs_redisplay; }
	
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
