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

// $Id: tclistrowdata.h,v 1.2 2001/06/25 08:20:23 christof Exp $

#ifndef TCLISTROWDATA
#define TCLISTROWDATA

#include<TCListRow.h>
#include<string>
#include<vector>
#include "rowdata.h"

class cH_RowDataBase;
class TreeBase;

class TCListRowData 
{
 cH_EntryValue value;
 int deep;

protected:
 TCListRow_API *listrow;
 bool leaf;
 
public: 
 virtual void cumulate(const cH_RowDataBase &rd, int seqnr,gpointer gp) const=0;
 virtual const vector<string> getColEntries(int cols);
 virtual const string getColText();
 virtual void refreshSum(const TreeBase &tb)=0;
 
 TCListRowData(const cH_EntryValue &v, int _deep, bool _leaf);
 TCListRow_API *getTCL_API() const { return listrow; }
 virtual ~TCListRowData() {}

 bool operator==(const TCListRowData &v) const;
 bool operator<(const TCListRowData &v) const;

 const cH_EntryValue Value() const { return value; }

 bool Leaf() { return leaf; }
 void setLeaf(bool l) { leaf=l; }
};

#endif

