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

class TCListNode;
class TCListLeaf;
class cH_RowDataBase;


class TreeBase : public TCList
{
 guint showdeep;
 guint attrcount;
 deque<guint> currseq; 
 deque<guint> clicked_seq;
 vector<bool> vec_hide_cols;

 Gtk::Menu *menu;  
 bool auffuellen_bool; 
 bool expandieren_bool; 

 void Auffuellen(Gtk::CheckMenuItem *auffuellen);
 void Expandieren(Gtk::CheckMenuItem *expandieren);
 void summen_knoepfe();
 void on_neuordnen_clicked();
 void on_zuruecksetzen_clicked();
 void on_abbrechen_clicked();
 void on_row_select(int row, int col, GdkEvent* b); // XX
 void fillMenu();
 gint MouseButton(GdkEventButton *event);
 void welche_Spalten(guint i,const Gtk::CheckMenuItem *sp);
 void show_or_hide_Spalten();
 void on_click_column(int col);
 bool col_schon_ausgewaehlt(int col);
 void insertIntoTCL(TCListRow_API *tclapi,const TreeBase &tb,
		 	const cH_RowDataBase &d, deque<guint> q,guint deep);
 static bool stutzen(TCListRow_API *parent, TCListRow_API *child,
 				TCList &tclist,guint deep);

protected: 
 vector<cH_RowDataBase> datavec;

 // set column names, fill data and display
 // also see discussion in treebase.cc
 void init();
 // StandardReihenfolge setzen
 virtual void setSequence();
 virtual const string getColTitle(guint seq) const;
 virtual TCListNode *NewNode(guint _seqnr, const cH_RowDataBase &v,
 				guint deep);			 	
 virtual TCListLeaf *NewLeaf(guint _seqnr, const cH_RowDataBase &v,
 				guint deep);			 	 				
 virtual void setColTitles();
 virtual void fillDataVec() {};
 void fillTCL();
 void refillTCL();

public:

 TreeBase(guint cols, guint attr=0);
 ~TreeBase();
 guint Attrs() const { return attrcount; }
 guint Cols() const { return columns().size();}
 void setDataVec(const vector<cH_RowDataBase> &d) 
 { datavec=d; 
   refillTCL();
 };
 
 void clear();
 SigC::Signal1<void,cH_RowDataBase> leaf_selected;
};

class SimpleTree : public TreeBase
{
 vector<string> titles;
 
public:
 SimpleTree(guint cols, guint attr, const vector<string>& T
                                ,const vector<cH_RowDataBase>& D)
   : TreeBase(cols,attr), titles(T) 
   {  datavec=D;
      // make sure this is not called if you derive this from class !
      init(); 
   }
 SimpleTree(guint cols, guint attr, const vector<string>& T)
   : TreeBase(cols,attr), titles(T) 
   {  // make sure this is not called if you derive this from class !
      init(); 
   }
 SimpleTree(guint cols, guint attr=0)
   : TreeBase(cols,attr)
   {  }
 
 void setTitles(const vector<string>& T)
   {  titles=T;
      setColTitles();
   }

 const string getColTitle(guint seq) const
   {  if (seq>=0 && seq<titles.size()) return titles[seq];
      return ""; 
   }
};
  
#endif
