// $Id: MyRowData.hh,v 1.3 2002/07/05 12:36:56 christof Exp $
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


#ifndef KOMPONENTENEXAMP_MYROWDATA
#define KOMPONENTENEXAMP_MYROWDATA

#include"rowdata.h"
#include"MyValue.h"

class MyRowData : public RowDataBase
{
 int attr1,attr2,attr3;
 int data1,data2;

public:

 MyRowData(int a1,int a2, int a3, int d1 ,int d2)
	: attr1(a1),attr2(a2),attr3(a3),data1(d1),data2(d2) {}
	
 virtual const cH_EntryValue Value(guint _seqnr,gpointer x) const
	{switch(_seqnr)
		{case 0 : return cH_MyValue(attr1); break;
		 case 1 : return cH_MyValue(attr2); break;
		 case 2 : return cH_MyValue(attr3); break;
		 case 3 : return cH_MyValue(data1); break;
		 case 4 : return cH_MyValue(data2); break;
 		 default : return cH_MyValue("-"); break;
		}
	}

 int Data1() const { return data1; }
 int Data2() const { return data2; }
};


class cH_MyRowData : public cH_RowDataBase
{

public:
 cH_MyRowData() : cH_RowDataBase() {}
 cH_MyRowData(const MyRowData *r) : cH_RowDataBase(r) {}

 cH_MyRowData(int a1,int a2, int a3, int d1, int d2)
	: cH_RowDataBase(new MyRowData(a1,a2,a3,d1,d2)) {}

};
#endif
