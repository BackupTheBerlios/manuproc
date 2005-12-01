/* $Id: MyNode.h,v 1.7 2005/12/01 18:36:27 christof Exp $ */
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


#include <TreeRow.h>
#include"MyRowData.hh"

class MyNode : public TreeRow
{
 int sum1;
 int sum2;

public:

 MyNode(const Handle<const TreeRow> &suminit)
 	: sum1(), sum2() 
 {}
 virtual void cumulate(const cH_RowDataBase &rd);
 virtual void deduct(const cH_RowDataBase &rd);
 virtual cH_EntryValue Value(guint col, gpointer) const;

 int Sum1() const { return sum1; }
 int Sum2() const { return sum2; }
};
