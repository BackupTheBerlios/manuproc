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

class MyNode : public TCListNode
{
 mutable int sum1;
 mutable int sum2;

public:

 virtual void cumulate(const cH_RowDataBase &rd, int seqnr) const;
 virtual const vector<string> getColEntries(int cols);
 virtual void resetSumValues(gpointer p);
 virtual const string getSumCol(int col);

 MyNode::MyNode(int _seqnr, const cH_RowDataBase &v, int deep);
 int Sum1() const { return sum1; }
 int Sum2() const { return sum2; }

};
