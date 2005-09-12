// $Id: SimpleTree.hh,v 1.42 2005/09/12 10:30:48 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef KOMPONENTEN_TREEBASE
#define KOMPONENTEN_TREEBASE

#include <SimpleTreeStore.h>
#include <gtkmm/treeview.h>

class SimpleTreeStore_Proxy
{protected:
	Glib::RefPtr<SimpleTreeStore> sts;
public:
	typedef SimpleTreeStore::sequence_t sequence_t;
	typedef SimpleTreeStore::const_iterator const_iterator;

	SimpleTreeStore_Proxy(unsigned int max_cols) : 
		sts(SimpleTreeStore::create(max_cols)) 
	{}

	void set_remember(const std::string &program, const std::string &instance) {  sts->set_remember(program,instance); }
	guint Cols() const  { return sts->Cols();}

	// these are from model
	void setDataVec(const std::vector<cH_RowDataBase> &d) {  sts->getModel().setDataVec(d); }
	void setTitles(const std::vector<std::string> &T) {  sts->getModel().setTitles(T); }
	void setTitleAt(unsigned idx, const std::string &s) {  sts->getModel().setTitleAt(idx,s); }
	const Glib::RefPtr<SimpleTreeStore> &getStore() { return sts; }
	const Glib::RefPtr<Gtk::TreeModel> getTreeModel() { return Glib::RefPtr<Gtk::TreeModel>(sts); }
	SimpleTreeModel &getModel() { return sts->getModel(); }
	const Glib::RefPtr<const SimpleTreeStore> getStore() const { return Glib::RefPtr<const SimpleTreeStore>(sts); }
	const std::string getColTitle(guint idx) const { return sts->getColTitle(idx); }
	void set_NewNode(SimpleTreeStore::NewNode_fp n) { sts->set_NewNode(n); }

	const_iterator begin() const { return sts->begin(); }
	const_iterator end() const { return sts->end(); }
	void set_tree_column_visibility(unsigned index,bool visible)
	{  sts->set_tree_column_visibility(index,visible); }
	void clear() { sts->clear(); }
	unsigned ColumnFromIndex(unsigned i) const 
	{  return sts->ColumnFromIndex(i); }
	unsigned IndexFromColumn(unsigned c) const
	{  return sts->IndexFromColumn(c); }
	const SimpleTreeStore::sequence_t &get_seq() const {return sts->get_seq();}
	
	void set_column_visibility(unsigned index,bool on) { sts->set_tree_column_visibility(index,on); }
	void set_value_data(gpointer _p) { sts->set_value_data(_p); }
	
	void redisplay(cH_RowDataBase row, unsigned index) {  sts->redisplay_old(row,index); }
};

namespace Gtk { class Menu; }
// I took the more esoteric features out to SimpleTree, 
// so they do not confuse the beginner
class SimpleTree_Basic : public Gtk::TreeView, public SimpleTreeStore_Proxy
{public:
        typedef bool (*button_press_vfunc_t)(SimpleTree_Basic *_this, const cH_RowDataBase &row, int col_idx);
        button_press_vfunc_t button_press_vfunc;
private:
	sequence_t clicked_seq;
	Gtk::Menu *menu;

	SigC::Signal0<void> _leaf_unselected;
	SigC::Signal1<void,cH_RowDataBase> _leaf_selected;
	SigC::Signal1<void,const TreeRow &> _node_selected;
	SigC::Signal0<void> _reorder;
	
	std::vector<gfloat> alignment;
	std::vector<bool> resizeable;	
	
	void on_title_changed(guint nr);
	void on_selection_changed();
	void on_redisplay();
	void on_title_clicked(unsigned no);
	void fillMenu();
	void on_neuordnen_clicked();
	void on_zuruecksetzen_clicked();
	void on_abbrechen_clicked();
	void on_spaltenzahl_geaendert();
	bool MouseButton(GdkEventButton *event);
	void sel_change_cb(const Gtk::TreeModel::iterator&it,
			std::vector<cH_RowDataBase> *l,
			std::vector<Handle<TreeRow> > *n);
        void on_column_edited(const Glib::ustring &path,
                  const Glib::ustring&new_text,unsigned idx);
	
public:
	SimpleTree_Basic(unsigned max_col);
	~SimpleTree_Basic();
	
	SigC::Signal1<void,cH_RowDataBase> &signal_leaf_selected()
	{ return _leaf_selected; }
	SigC::Signal0<void> &signal_leaf_unselected()
	{ return _leaf_unselected; }
	// perhaps Handle<const TreeRow> is more sensible now?
	SigC::Signal1<void,const TreeRow &> &signal_node_selected()
	{ return _node_selected; }
	SigC::Signal0<void> &signal_reorder()
	{ return _reorder; }
	
	void detach();
	void attach();
	void setDataVec(const std::vector<cH_RowDataBase> &d) 
	{  detach(); SimpleTreeStore_Proxy::setDataVec(d); attach(); }

	void Expand_recursively();
	void Collapse();
	void setTitles(const std::vector<std::string> &T);
	void setAlignment(const std::vector<gfloat> &A);
	void setResizeable(const std::vector<bool> &R);	
	void setResizeable(const bool b);		
	
	void debug();
};

// I put the more esoteric features here, so they may not confuse the
// beginner
class SimpleTree : public SimpleTree_Basic
{
public:
	// attr is not needed any longer
	SimpleTree(guint maxcol) : SimpleTree_Basic(maxcol)
	{}
	SimpleTree(guint maxcol, const std::vector<std::string>& T
                                ,const std::vector<cH_RowDataBase>& D)
	: SimpleTree_Basic(maxcol)
	{  setTitles(T);
	   setDataVec(D);
	}
	SimpleTree(guint maxcol, const std::vector<std::string>& T)
	: SimpleTree_Basic(maxcol)
	{  setTitles(T);
	}	
private:
 // strictly this belongs into SimpleTreeStore
 template <class T> 
  void ForEachLeaf2(const_iterator b,const_iterator e, T &t) const
 {  for (const_iterator i=b;i!=e;++i)
    {  if (!i->second.childrens_deep) t(i->second.leafdata);
       else if (!i->second.children.empty())
          ForEachLeaf2(i->second.children.begin(),i->second.children.end(),t);
    }
 }
 
 template <class T> void selectMatchingLines2(const_iterator b, 
 			const_iterator e, const T &t,
 			bool only_one_line)
 {  for (const_iterator i=b;i!=e;++i)
    {  if (!i->second.childrens_deep && i->second.leafdata==t)
       {  get_selection()->select(getStore()->getIter(i));
          if (only_one_line) return;
       }
       else if (!i->second.children.empty())
          selectMatchingLines2(i->second.children.begin(),i->second.children.end(),t,only_one_line);
    }
 }
 void getSelectedRowDataBase_vec_cb(const Gtk::TreeModel::iterator&it, 
		std::vector<cH_RowDataBase> *res);

public:
 struct SelectionError : public std::exception
 {  virtual const char* what() const throw() { return "TreeBase::SelectionError"; }
    SelectionError() {}
 };
 struct noRowSelected : public SelectionError
 {  virtual const char* what() const throw() { return "TreeBase::noRowSelected"; }
    noRowSelected() {}
 };
 struct multipleRowsSelected : public SelectionError
 {  virtual const char* what() const throw() { return "TreeBase::multipleRowsSelected"; }
    multipleRowsSelected() {}
 };
 struct notLeafSelected : public SelectionError
 {  virtual const char* what() const throw() { return "TreeBase::notLeafSelected"; }
    notLeafSelected() {}
 };
 struct noNodeSelected : public SelectionError
 {  virtual const char* what() const throw() { return "TreeBase::noNodeSelected"; }
    noNodeSelected() {}
 };
 struct multipleNodesSelected : public SelectionError
 {  virtual const char* what() const throw() { return "TreeBase::multipleNodesSelected"; }
    multipleNodesSelected() {}
 };
 struct notNodeSelected : public SelectionError
 {  virtual const char* what() const throw() { return "TreeBase::notNodeSelected"; }
    notNodeSelected() {}
 };

 cH_RowDataBase getSelectedRowDataBase() const
 	throw(noRowSelected,multipleRowsSelected,notLeafSelected);
 cH_RowDataBase getCursorRowDataBase() const
 	throw(noRowSelected,multipleRowsSelected,notLeafSelected);
 std::vector<cH_RowDataBase> getSelectedRowDataBase_vec() const 
 	throw(notLeafSelected);

#if 1 // deprecated
 template <class T,class CT> T getSelectedRowDataBase_as2() const
// this could be optimzed to avoid the dynamic_cast within 
// cH_RowDataBase::operator*, but it does not hurt that much
 {  return T(dynamic_cast<CT*>(&*getSelectedRowDataBase()));
 }
#endif
 template <class T> T getSelectedRowDataBase_as() const
 {  return getSelectedRowDataBase_as2<T,typename T::ContentType>(); 
// {  return T::cast_dynamic(getSelectedRowDataBase()); 
 }
 template <class T> T getCursorRowDataBase_as() const
 {  return T::cast_dynamic(getCursorRowDataBase()); 
 }
 Handle<const TreeRow> getSelectedNode() const 
 	throw(noNodeSelected,multipleNodesSelected,notNodeSelected);
 template <typename T> Handle<const T> getSelectedNode_as() const
 {  return Handle<const T>::cast_dynamic(getSelectedNode()); 
 }
 
 template <class T> void selectMatchingLines(const T &t)
 {  get_selection()->unselect_all();
    selectMatchingLines2(begin(),end(),t,false);
 }
 template <class T> void selectFirstMatchingLine(const T &t)
 {  get_selection()->unselect_all();
    selectMatchingLines2(begin(),end(),t,true);
 }
 template <class T> void ForEachLeaf(T &t) const
 {  ForEachLeaf2(begin(),end(),t); }
 
 // very basic method for scrolling to a specific line - tell me if you 
 // need more - CP
 void ScrollToSelection();
};

#if 0
#include <gtkmm/menu.h>
#include <gtkmm/checkmenuitem.h>

class SimpleTree : public Gtk::TreeView
{Gtk::CheckMenuItem *titles_menu;
 bool titles_bool:1; 
 
 void Titles(Gtk::CheckMenuItem *titles);
 void Auffuellen(Gtk::CheckMenuItem *auffuellen);
 void Expandieren(Gtk::CheckMenuItem *expandieren);
 void summen_knoepfe();
 void on_neuordnen_clicked();
 void on_zuruecksetzen_clicked();
 void on_abbrechen_clicked();
 void on_row_select(int row, int col, GdkEvent* b);
 gint MouseButton(GdkEventButton *event);
 void welche_Spalten(guint i,const Gtk::CheckMenuItem *sp);
 void show_or_hide_Spalten();
 void on_click_column(int col);
 bool col_schon_ausgewaehlt(int col);
 void insertIntoTCL(TCListRow_API *tclapi,const TreeBase &tb,
		 	const cH_RowDataBase &d, sequence_t q,guint deep);
 bool redisplay_recurse(TCListRow_API *a, const RowDataBase *r, guint col);
 void reihenfolge_anzeigen();
 void initDepth(TreeRow *tr, guint depth) const;
 void on_Color(const Gtk::CheckMenuItem *sp);
 
protected: 
 // StandardReihenfolge setzen
 virtual void setSequence();

public:
 void show_titles(bool show);
};
  
#endif
#endif
