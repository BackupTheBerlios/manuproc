// $Id: MyTreeBase.hh,v 1.7 2005/12/01 18:36:17 christof Exp $
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

#include"treebase.h"
#include"MyRowData.hh"

class MyTreeBase : public TreeBase
{
// int custom_data; // Kundennr z.B.

public:
 virtual void fillDataVec();

 MyTreeBase(guint cols, guint attr=0):TreeBase(cols,attr)
	{ // make sure this is not called if you derive this from class !
	  init(); 
	}

 TreeRow *NewNode(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data,bool expand);

 const std::string getColTitle(guint seq) const;
};
