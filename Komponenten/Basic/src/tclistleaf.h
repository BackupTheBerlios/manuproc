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

// $Id: tclistleaf.h,v 1.4 2001/08/05 20:37:15 thoma Exp $

#ifndef KOMPONENTEN_TCLISTLEAF
#define KOMPONENTEN_TCLISTLEAF

#include"tclistrowdata.h"
#include"rowdata.h"
#include"treebase.h"

class TCListLeaf : public TCListRowData
{protected:
	cH_RowDataBase leafdata;
  
public:
 TCListLeaf(guint deep, const cH_EntryValue &v, const cH_RowDataBase &d)
	: TCListRowData(v,deep,true,false), leafdata(d)
{}

// virtual void cumulate(const cH_RowDataBase &rd)
// {};
// virtual void refreshSum(const TreeBase &tb) {};

 
 const cH_RowDataBase LeafData() const { return leafdata; }
 const std::vector<std::string> getColEntries(const TreeBase &tb) const;
};
#endif
