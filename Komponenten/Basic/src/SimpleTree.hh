// $Id: SimpleTree.hh,v 1.5 2002/12/03 08:44:30 christof Exp $
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
};

class SimpleTree : public Gtk::TreeView, public SimpleTreeStore_Proxy
{private:
	void on_title_changed(guint nr);
	SigC::Signal1<void,cH_RowDataBase> _leaf_selected;
	SigC::Signal1<void,const TreeRow &> _node_selected;
	
public:
	SimpleTree(unsigned int cols,int attrs=-1);
	SigC::Signal1<void,cH_RowDataBase> &signal_leaf_selected()
	{ return _leaf_selected; }
	// perhaps Handle<const TreeRow> is more sensible now?
	SigC::Signal1<void,const TreeRow &> &signal_node_selected()
	{ return _node_selected; }
};

#if 0
#include <deque>
#include <gtkmm/menu.h>
#include <gtkmm/checkmenuitem.h>

class SimpleTree : public Gtk::TreeView
{
	std::deque<guint> clicked_seq;
 
 Gtk::Menu *menu;  
 Gtk::CheckMenuItem *titles_menu;
 bool titles_bool:1; 
 

 std::vector<Gdk_Color> colors;
 static const unsigned int num_colors=8;
 
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

 // set column names, fill data and display
 // also see discussion in treebase.cc
 void init();
 // StandardReihenfolge setzen
 virtual void setSequence();
 virtual const std::string getColTitle(guint seq) const;

// virtual void setColTitles();
// virtual void fillDataVec() {};
 void fillTCL();
 void refillTCL(bool clear_me=true);

public:
 TreeBase(guint cols, guint attr=0);
 ~TreeBase();
 void setDataVec(const std::vector<cH_RowDataBase> &d,bool clear_me=true) 
 { datavec=d; 
   refillTCL(clear_me);
 };
 void redisplay(cH_RowDataBase row,guint index);
 
 static void Expand_recursively(TCListRow_API &api);
 void Expand_recursively();
 void Collapse(){collapse();}
 
 void show_titles(bool show);
 void set_tree_column_visibility(unsigned int column,bool visible);
 
 void clear();
 SigC::Signal1<void,cH_RowDataBase> leaf_selected;
 SigC::Signal1<void,const TreeRow &> node_selected;
 SigC::Signal0<void> reorder;
 
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
 

private:
 template <class T> 
  static void ForEachLeaf2(TCListRow_API::const_iterator b,
                         TCListRow_API::const_iterator e, T &t)
 {  for (TCListRow_API::const_iterator i=b;i!=e;++i)
    {  const TreeRow *tlr=reinterpret_cast<const TreeRow *>((*i).get_user_data());
       if (!tlr->Leaf())
       {  ForEachLeaf2(tlr->begin(),tlr->end(),t);
       }
       else t(tlr->LeafData());
    }
 }
 
 template <class T> void selectMatchingLines2(TCListRow_API::const_iterator b, 
 			TCListRow_API::const_iterator e, const T &t,
 			bool only_one_line)
 {  for (TCListRow_API::const_iterator i=b;i!=e;++i)
    {  const TreeRow *tlr=reinterpret_cast<const TreeRow *>((*i).get_user_data());
       if (tlr->LeafData()==t)
       {  int rowno=static_cast<const TCListRow&>(*i).get_lineno();
          if (rowno!=-1) 
             { row(rowno).select();
               if (only_one_line) return;
             }
       }
       if ((*i).begin()!=(*i).end()) selectMatchingLines2((*i).begin(),
                                        (*i).end(),t,only_one_line);
    }
 }

public:
 template <class T> void selectMatchingLines(const T &t)
 {  selection().clear();
    selectMatchingLines2(begin(),end(),t,false);
 }
 template <class T> void selectFirstMatchingLine(const T &t)
 {  selection().clear();
    selectMatchingLines2(begin(),end(),t,true);
 }
 template <class T> void ForEachLeaf(T &t) const
 {  ForEachLeaf2(begin(),end(),t); }
 
protected:

// @ ins cc file ?
 static TreeRow *defaultNewNode
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, 
 		cH_RowDataBase child_s_data, bool expand,
 		const TreeRow &suminit);
 virtual TreeRow *NewNode
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, 
 		cH_RowDataBase child_s_data, bool expand,
 		const TreeRow &suminit)
 {  return (*node_creation)(deep,v,child_s_deep,child_s_data,expand,suminit); }
public:
 SimpleTree(guint cols, guint attr, const std::vector<std::string>& T
                                ,const std::vector<cH_RowDataBase>& D)
   : TreeBase(cols,attr), titles(T), node_creation(&defaultNewNode)
   {  datavec=D;
      // make sure this is not called if you derive this from class !
      init(); 
   }
 SimpleTree(guint cols, guint attr, const std::vector<std::string>& T)
   : TreeBase(cols,attr), titles(T), node_creation(&defaultNewNode)
   {  // make sure this is not called if you derive this from class !
      init(); 
   }
 SimpleTree(guint cols, guint attr=0)
   : TreeBase(cols,attr), node_creation(&defaultNewNode)
   {  }
 
 void setTitles(const std::vector<std::string>& T)
   {  titles=T;
      setColTitles();
   }

 const std::string getColTitle(guint seq) const
   {  if (seq>=0 && seq<titles.size()) return titles[seq];
      return ""; 
   }
  
 void set_NewNode(NewNode_fp x) { node_creation=x; }
};
  
#endif
#endif
