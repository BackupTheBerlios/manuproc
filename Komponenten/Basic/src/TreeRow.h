/* $Id: TreeRow.h,v 1.4 2002/11/28 10:08:50 christof Exp $ */
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

// $Id: TreeRow.h,v 1.4 2002/11/28 10:08:50 christof Exp $

#ifndef TCLISTROWDATA
#define TCLISTROWDATA

#include<string>
#include<vector>
#include "RowDataBase.h"

//class TreeBase;

class TreeRow : public HandleContent
{protected:
	cH_EntryValue value;
	bool expand:1;
	
	// childrens_deep=0 -> Leaf
	guint childrens_deep;
	
	// now we need it even for nodes ... perhaps it gets really simple now
	// if we're a node this is not 'our' data
	cH_RowDataBase leafdata;
	

	// Wert verändern wird von TreeBase::insertIntoTCL verwendet
	// wenn sich die Tiefe ändert
	void ValueDeep(const cH_EntryValue &ev, guint _deep);
	void Children_s_Deep(guint _deep);

 // wird das noch benötigt? CP 10'2001
 virtual const std::string getColText() const { return value->getStrVal(); }

public:
// typedef TCListRow_API::const_iterator const_iterator;
 class const_iterator {};
 
	// deep2:=0 for a leaf
 TreeRow(guint dummy_deep1, const cH_EntryValue &v, guint _deep2, 
 		cH_RowDataBase data, bool exp=false)
   : value(v), expand(exp), childrens_deep(_deep2),
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
// guint Deep() const { return deep; }
 const cH_EntryValue Value() const { return value; }
 const cH_RowDataBase LeafData() const { return leafdata; }

 virtual const cH_EntryValue Value(guint _seqnr,gpointer _g) const;
 virtual void cumulate(const cH_RowDataBase &rd) {} 
};

#endif
