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

#include<TCList.h>
#include<deque>
#include <gtk--/menu.h>
#include<vector>
#include"tclistrowdata.h"

class TCListRowData;
class cH_RowDataBase;

class TreeBase : public TCList
{
 guint showdeep;
 guint attrcount;
 std::deque<guint> currseq; 
 std::deque<guint> clicked_seq;
 std::vector<bool> vec_hide_cols;
 gpointer gp;
 
 Gtk::Menu *menu;  
 Gtk::CheckMenuItem *titles_menu;
 bool titles_bool:1; 
 bool auffuellen_bool:1; 
 bool expandieren_bool:1; 
 bool stutzen_bool:1;
 
 
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
 static bool stutzen(TCListRow_API *parent, TCListRow_API *child,
 				TCList &tclist);
 bool redisplay_recurse(TCListRow_API *a, const RowDataBase *r, guint col);
 
protected: 
 std::vector<cH_RowDataBase> datavec;

 // set column names, fill data and display
 // also see discussion in treebase.cc
 void init();
 // StandardReihenfolge setzen
 virtual void setSequence();
 virtual const std::string getColTitle(guint seq) const;
 // einen neuen Ast erzeugen, deep ist die Spalte, v der Wert dieser Spalte
 virtual TCListRowData *NewNode
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data, bool expand);
 // ein neues Blatt erzeugen, deep ist die Spalte, seqnr der Werteindex
 // deep == Attrs() !
 // eigentlich Unsinn, das hier zu überladen ...
 virtual TCListRowData *NewLeaf
 		(guint deep, const cH_EntryValue &v, const cH_RowDataBase &d);

 virtual void setColTitles();
 virtual void fillDataVec() {};
 void fillTCL();
 void refillTCL();

public:
 TreeBase(guint cols, guint attr=0);
 ~TreeBase();
 void set_showdeep(int i) {showdeep=i;}
 guint Attrs() const { return attrcount; }
 guint Cols() const { return columns().size();}
 void setDataVec(const std::vector<cH_RowDataBase> &d) 
 { datavec=d; 
   refillTCL();
 };
 void set_value_data(gpointer _p) {gp = _p;}
 void redisplay(cH_RowDataBase row,guint index);
 gpointer ValueData() const { return gp; }
 void Stutzen ( bool s) {stutzen_bool=s;}
 
 static void Expand_recursively(TCListRow_API &api);
 void Expand_recursively();
 
 const std::deque<guint> &get_seq() const {return currseq;}
 void show_titles(bool show);
 
 void clear();
 SigC::Signal1<void,cH_RowDataBase> leaf_selected;
 SigC::Signal1<void,const TCListRowData &> node_selected;
 
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

 template <class T,class CT> T getSelectedRowDataBase_as() const
// this could be optimzed to avoid the dynamic_cast within 
// cH_RowDataBase::operator*, but it does not hurt that much
 {  return T(dynamic_cast<CT*>(&*getSelectedRowDataBase()));
 }
 template <class T> T getSelectedRowDataBase_as() const
 {  return getSelectedRowDataBase_as<T,typename T::ContentType>(); 
 }


// TCListNode &getSelectedNode() const 
 TCListRowData &getSelectedNode() const 
 	throw(noNodeSelected,multipleNodesSelected,notNodeSelected);
 template <class T> T &getSelectedNode_as() const
 {  return dynamic_cast<T&>(getSelectedNode());
 }

private:
 template <class T> void selectMatchingLines(TCListRow_API::const_iterator b, 
 			TCListRow_API::const_iterator e, const T &t)
 {  for (TCListRow_API::const_iterator i=b;i!=e;++i)
    {  const TCListRowData *tlr=reinterpret_cast<const TCListRowData *>((*i).get_user_data());
       if (tlr->LeafData()==t)
       {  int rowno=static_cast<const TCListRow&>(*i).get_lineno();
          if (rowno!=-1) row(rowno).select();
       }
       if ((*i).begin()!=(*i).end()) selectMatchingLines((*i).begin(),(*i).end(),t);
    }
 }
 
public:
 template <class T> void selectMatchingLines(const T &t)
 {  selection().clear();
    selectMatchingLines(begin(),end(),t);
 }
};

///////////////////////////////////////////////////////////////////
// newer, more simplyfied API:
class SimpleTree : public TreeBase
{protected:
 typedef TCListRowData *(*NewNode_fp)
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data, bool expand);

 std::vector<std::string> titles;
 NewNode_fp node_creation;
 
// @ ins cc file ?
 static TCListRowData *defaultNewNode
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data, bool expand);
 virtual TCListRowData *NewNode
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data, bool expand)
 {  return (*node_creation)(deep,v,child_s_deep,child_s_data,expand); }
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
