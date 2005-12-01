/* $Id: MyNode.h,v 1.6 2005/12/01 18:36:17 christof Exp $ */
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


#include"tclistnode.h"
#include"MyRowData.hh"

class MyNode : public TreeRow
{
 int sum1;
 int sum2;

public:

 MyNode(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data, bool expand)
 	: TreeRow(deep,v,child_s_deep,child_s_data,expand), sum1(0), sum2(0) 
 {}
 virtual void cumulate(const cH_RowDataBase &rd);
 virtual const cH_EntryValue Value(guint col, gpointer) const;

 int Sum1() const { return sum1; }
 int Sum2() const { return sum2; }
};
