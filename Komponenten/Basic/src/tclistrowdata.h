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

// $Id: tclistrowdata.h,v 1.8 2001/11/19 12:49:56 christof Exp $

// BAD name for this include file actually it's a TreeRow

#ifndef TCLISTROWDATA
#define TCLISTROWDATA

#include<TCListRow.h>
#include<string>
#include<vector>
#include "rowdata.h"

class cH_RowDataBase;
class TreeBase;

// better name this class TreeRow - or TreeNode ?

class TreeRow
{protected:
	TCListRow_API *listrow;
	cH_EntryValue value;
	bool expand:1;
	
	// we need to remember deep for the second stage ctor: initTCL
	guint deep;
	// childrens_deep=0 -> Leaf
	guint childrens_deep;
	// perhaps we should remember children's depth also
	
	// now we need it even for nodes ... perhaps it gets really simple now
	// if we're a node this is not 'our' data
	cH_RowDataBase leafdata;
	

	friend class TreeBase;
 
 	// wegen virtuellen Funktionen nicht Teil des Konstruktors
 	// wird direkt danach aufgerufen
 	// Hängt in die Liste ein
	void initTCL(TCListRow_API *api, TCListRow_API::iterator davor,
			const TreeBase &tb);
	void initTCL(TCListRow_API *api, const TreeBase &tb);
	
	TCListRow_API *getTCL_API() { return listrow; }
	const TCListRow_API *getTCL_API() const { return listrow; }

        virtual const std::vector<std::string> getColEntries(const TreeBase &tb) const;

//	void Leaf(bool l) { leaf=l; } // I prefer this one, CP
//	void setLeaf(bool l) { leaf=l; }
	
	// Wert verändern wird von TreeBase::insertIntoTCL verwendet
	// wenn sich die Tiefe ändert
	void ValueDeep(const cH_EntryValue &ev, guint _deep);
	void Children_s_Deep(guint _deep);

 // wird das noch benötigt? CP 10'2001
 virtual const std::string getColText() const { return value->getStrVal(); }

public:
 typedef TCListRow_API::const_iterator const_iterator;
 
	// deep2:=0 for a leaf
 TreeRow(guint _deep1, const cH_EntryValue &v, guint _deep2, 
 		cH_RowDataBase data, bool exp=false)
   : listrow(0), value(v), expand(exp), deep(_deep1), childrens_deep(_deep2),
   	leafdata(data)
 {}
 virtual ~TreeRow() {}

 static TreeRow *create(guint _deep1, const cH_EntryValue &v, guint _deep2, 
 		cH_RowDataBase data, bool exp=false)
 {  return new TreeRow(_deep1,v, _deep2, data, exp); }

 bool operator==(const TreeRow &v) const
 {  return (*value) == *(v.Value()); }
 bool operator<(const TreeRow &v) const
 {  return (*value) < *(v.Value()); }

 bool Leaf() const { return childrens_deep==0; }
 guint Children_s_Deep() const { return childrens_deep; }
 guint Deep() const { return deep; }
 const cH_EntryValue Value() const { return value; }

 const cH_RowDataBase LeafData() const { return leafdata; }
 virtual const cH_EntryValue Value(guint _seqnr,gpointer _g) const;
 
// primär für nodes von Interesse
 void refreshSum(const TreeBase &tb);
 virtual void cumulate(const cH_RowDataBase &rd) {} 
 // wenn mir jemand verrät, was das hier ist ... CP 10'01
// virtual void orderit(const cH_RowDataBase &rd) {}

 const_iterator begin() const
 {  return getTCL_API()->begin();
 }
 const_iterator end() const
 {  return getTCL_API()->end();
 }
};

typedef TreeRow TCListRowData;
typedef TCListRowData TCListNode;
typedef TCListRowData TCListLeaf; // no difference any longer

#endif
