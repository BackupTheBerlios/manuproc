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

// $Id: tclistrowdata.h,v 1.5 2001/07/05 12:58:06 christof Exp $

#ifndef TCLISTROWDATA
#define TCLISTROWDATA

#include<TCListRow.h>
#include<string>
#include<vector>
#include "rowdata.h"

class cH_RowDataBase;
class TreeBase;

class TCListRowData 
{protected:
	TCListRow_API *listrow;
	cH_EntryValue value;
	bool leaf:1, expand:1;
	int deep;
	
	friend class TreeBase;
 
 	// wegen virtuellen Funktionen nicht Teil des Konstruktors
 	// wird direkt danach aufgerufen
	void initTCL(TCListRow_API *api, TCListRow_API::iterator davor,
			const TreeBase &tb);  // ,int deep);
	void initTCL(TCListRow_API *api, const TreeBase &tb); // ,int deep);
	TCListRow_API *getTCL_API() { return listrow; }
	const TCListRow_API *getTCL_API() const { return listrow; }

	void Leaf(bool l) { leaf=l; } // I prefer this one, CP
	void setLeaf(bool l) { leaf=l; }
public:
 TCListRowData(const cH_EntryValue &v, int _deep, bool _leaf, bool exp)
   : listrow(0), value(v), leaf(_leaf), expand(exp), deep(_deep)
 {}
 virtual ~TCListRowData() {}

 virtual const std::vector<std::string> getColEntries(const TreeBase &tb) const=0;
// nur fuer nodes
// virtual void cumulate(const cH_RowDataBase &rd, int seqnr,gpointer gp)=0;
// nur fuer nodes
// virtual void refreshSum(const TreeBase &tb)=0;
 
 bool operator==(const TCListRowData &v) const
 {  return (*value) == *(v.Value()); }
 bool operator<(const TCListRowData &v) const
 {  return (*value) < *(v.Value()); }

 const cH_EntryValue Value() const { return value; }
 virtual const std::string getColText() const { return value->getStrVal(); }

 bool Leaf() { return leaf; }
};

#endif
