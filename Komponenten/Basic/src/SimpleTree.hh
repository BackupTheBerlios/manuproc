// $Id: SimpleTree.hh,v 1.9 2002/12/05 08:39:19 christof Exp $
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
	SimpleTreeStore sts;
public:
	SimpleTreeStore_Proxy(unsigned int cols,int attrs) : sts(cols,attrs) {}

	void set_remember(const std::string &program, const std::string &instance) {  sts.set_remember(program,instance); }
	guint Cols() const  { return sts.Cols();}

	// these are from model
	void setDataVec(const std::vector<cH_RowDataBase> &d) {  sts.getModel().setDataVec(d); }
	void setTitles(const std::vector<std::string>& T) {  sts.getModel().setTitles(T); }
	SimpleTreeStore &getStore() { return sts; }
	const SimpleTreeStore &getStore() const { return sts; }
	const std::string getColTitle(guint idx) const { return sts.getColTitle(idx); }
	void set_NewNode(SimpleTreeStore::NewNode_fp n) { sts.set_NewNode(n); }

	typedef SimpleTreeStore::const_iterator const_iterator;
	const_iterator begin() const { return sts.begin(); }
	const_iterator end() const { return sts.end(); }
	void set_tree_column_visibility(unsigned index,bool visible)
	{  sts.set_tree_column_visibility(index,visible); }
	void clear() { sts.clear(); }
};

// I took the more esoteric features out to SimpleTree, 
// so they do not confuse the beginner
class SimpleTree_Basic : public Gtk::TreeView, public SimpleTreeStore_Proxy
{private:
	std::deque<guint> clicked_seq;

	void on_title_changed(guint nr);
	void on_selection_changed();
	SigC::Signal1<void,cH_RowDataBase> _leaf_selected;
	SigC::Signal1<void,const TreeRow &> _node_selected;
	SigC::Signal0<void> _reorder;
	
	void on_title_clicked(unsigned no);
	
public:
	SimpleTree_Basic(unsigned int cols,int attrs=-1);
	SigC::Signal1<void,cH_RowDataBase> &signal_leaf_selected()
	{ return _leaf_selected; }
	// perhaps Handle<const TreeRow> is more sensible now?
	SigC::Signal1<void,const TreeRow &> &signal_node_selected()
	{ return _node_selected; }
	SigC::Signal0<void> &signal_reorder()
	{ return _reorder; }
};

// I put the more esoteric features here, so they may not confuse the
// beginner
class SimpleTree : public SimpleTree_Basic
{
public:
	// attr is not needed any longer
	SimpleTree(guint columns=0,guint attr=-1) : 
		SimpleTree_Basic(columns,attr)
	{}
	SimpleTree(guint cols, guint attr, const std::vector<std::string>& T
                                ,const std::vector<cH_RowDataBase>& D)
	: SimpleTree_Basic(cols,attr)
	{  setTitles(T);
	   setDataVec(D);
	}
	SimpleTree(guint cols, guint attr, const std::vector<std::string>& T)
	: SimpleTree_Basic(cols,attr)
	{  setTitles(T);
	}	
private:
 // strictly this belongs into SimpleTreeStore
 template <class T> 
  void ForEachLeaf2(const_iterator b,const_iterator e, T &t) const
 {  for (const_iterator i=b;i!=e;++i)
    {  if (!(*i)[getStore().m_columns.childrens_deep])
          t((*i)[getStore().m_columns.leafdata]);
       else if (i->children().begin()!=i->children().end())
          ForEachLeaf2(i->children().begin(),i->children().end(),t);
    }
 }
 
 template <class T> void selectMatchingLines2(const_iterator b, 
 			const_iterator e, const T &t,
 			bool only_one_line)
 {  for (const_iterator i=b;i!=e;++i)
    {  if (!(*i)[getStore().m_columns.childrens_deep]
          && static_cast<cH_RowDataBase>((*i)[getStore().m_columns.leafdata])
          	==t)
       {  get_selection()->select(*i);
          if (only_one_line) return;
       }
       else if (i->children().begin()!=i->children().end())
          selectMatchingLines2(i->children().begin(),i->children().end(),t,only_one_line);
    }
 }

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
 std::vector<cH_RowDataBase> getSelectedRowDataBase_vec() const 
 	throw(notLeafSelected);

 template <class T,class CT> T getSelectedRowDataBase_as2() const
// this could be optimzed to avoid the dynamic_cast within 
// cH_RowDataBase::operator*, but it does not hurt that much
 {  return T(dynamic_cast<CT*>(&*getSelectedRowDataBase()));
 }
 template <class T> T getSelectedRowDataBase_as() const
 {  return getSelectedRowDataBase_as2<T,typename T::ContentType>(); 
 }
 TreeRow &getSelectedNode() const 
 	throw(noNodeSelected,multipleNodesSelected,notNodeSelected);
 template <class T> T &getSelectedNode_as() const
 {  return dynamic_cast<T&>(getSelectedNode());
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
 
 void Expand_recursively();
 void Collapse();
};

#if 0
#include <gtkmm/menu.h>
#include <gtkmm/checkmenuitem.h>

class SimpleTree : public Gtk::TreeView
{Gtk::Menu *menu;  
 Gtk::CheckMenuItem *titles_menu;
 bool titles_bool:1; 
 
 void Titles(Gtk::CheckMenuItem *titles);
 void Auffuellen(Gtk::CheckMenuItem *auffuellen);
 void Expandieren(Gtk::CheckMenuItem *expandieren);
 void summen_knoepfe();
 void on_neuordnen_clicked();
 void on_zuruecksetzen_clicked();
 void on_abbrechen_clicked();
 void on_row_select(int row, int col, GdkEvent* b);
 void fillMenu();
 gint MouseButton(GdkEventButton *event);
 void welche_Spalten(guint i,const Gtk::CheckMenuItem *sp);
 void show_or_hide_Spalten();
 void on_click_column(int col);
 bool col_schon_ausgewaehlt(int col);
 void insertIntoTCL(TCListRow_API *tclapi,const TreeBase &tb,
		 	const cH_RowDataBase &d, std::deque<guint> q,guint deep);
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
