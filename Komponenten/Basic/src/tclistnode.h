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

// $Id: tclistnode.h,v 1.9 2001/10/02 15:28:01 christof Exp $

#ifndef KOMPONENTEN_TCLISTNODE
#define KOMPONENTEN_TCLISTNODE

#include"treebase.h"
#include"tclistrowdata.h"
#include"rowdata.h"

class TCListNode : public TCListRowData
{public:
 TCListNode(guint deep, const cH_EntryValue &v, bool expand)
 	: TCListRowData(v,deep,false,expand)
 {}

 // ehemals getSumCol (Vorsicht der Index hat sich geaendert!!!)
 virtual const cH_EntryValue Value (guint col,gpointer gp) const;
 virtual void cumulate(const cH_RowDataBase &rd) {} 
 virtual void orderit(const cH_RowDataBase &rd) {}

 // ruft Value auf
 void refreshSum(const TreeBase &tb);
 
 virtual const std::vector<std::string> getColEntries(const TreeBase &tb) const;
};
#endif
