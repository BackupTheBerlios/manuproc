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

// $Id: tclistnode.h,v 1.9 2001/04/20 07:10:56 christof Exp $

#ifndef KOMPONENTEN_TCLISTNODE
#define KOMPONENTEN_TCLISTNODE

#include"treebase.h"
#include"tclistrowdata.h"
#include"rowdata.h"

class TCListNode : public TCListRowData
{
 bool show;
 mutable int sumvalue;
 
public:

 TCListNode(int _seqnr, const cH_RowDataBase &v,
 		int deep);
 TCListNode();
 TCListNode(const cH_EntryValue &v);

 virtual const string getSumCol(int col);
 virtual void cumulate(const cH_RowDataBase &rd, int seqnr) const;
 virtual void resetSumValues(gpointer);

 void initTCL(TCListRow_API *api, TCListRow_API::iterator davor,
 			const TreeBase &tb,int deep);
 void initTCL(TCListRow_API *api, const TreeBase &tb,int deep);
 
 int SumValue() const { return sumvalue; }
 void refreshSum(const TreeBase &tb);
 
};


#endif
