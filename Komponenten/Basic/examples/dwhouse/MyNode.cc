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

#include<Aux/Ausgabe_neu.h>
#include"MyNode.h"

MyNode::MyNode(int _seqnr, 
		const cH_RowDataBase &v, 
		int deep) 
 : TCListNode(_seqnr,v,deep), sum1(0),sum2(0)
{}


void MyNode::cumulate(const cH_RowDataBase &rd, int seqnr) const
{
 sum1+= (dynamic_cast<const MyRowData &>(*rd)).Data1();
 sum2+= (dynamic_cast<const MyRowData &>(*rd)).Data2();
}

void MyNode::resetSumValues(gpointer p)
{
 sum1=((MyNode*)p)->Sum1();
 sum2=((MyNode*)p)->Sum2();
}

const string MyNode::getSumCol(int col)
{
 switch(col)
   { case 0 :
	return Formatiere(sum1);
	break;
     case 1 :
	return Formatiere(sum2);
	break;
     default : return("-");
   }
}


const vector<string> MyNode::getColEntries(int cols)
{
 static vector<string> v;

 v=TCListRowData::getColEntries(cols);

 v[3]=Formatiere(sum1);
 v[4]=Formatiere(sum2);

 cout << "getColEntries Node\n";
 return v;

}
