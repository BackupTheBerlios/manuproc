// $Id: MyTreeBase.cc,v 1.8 2005/12/01 18:36:27 christof Exp $
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


#include "MyTreeBase.hh"
#include "MyRowData.hh"
#include "MyNode.h"

Handle<TreeRow> MyTreeBase::NewNode(const Handle<const TreeRow>&suminit)
{
 return new  MyNode(suminit);
}

MyTreeBase::MyTreeBase(guint cols)
 :SimpleTree(cols)
{ std::vector<cH_RowDataBase> datavec;
 datavec.push_back(cH_MyRowData(1,3,3,10,11)); 
 datavec.push_back(cH_MyRowData(2,2,3,10,11)); 
 datavec.push_back(cH_MyRowData(2,2,4,10,11)); 
 datavec.push_back(cH_MyRowData(2,3,4,10,11)); 
 datavec.push_back(cH_MyRowData(3,2,3,10,11)); 
 datavec.push_back(cH_MyRowData(3,2,4,11,11)); 
 datavec.push_back(cH_MyRowData(4,2,4,10,11)); 
 (getModel())=datavec;
 std::vector<std::string> titles(5);
 titles[0]="Attribut 1"; 
 titles[1]="Attribut 2"; 
 titles[2]="Attribut 3"; 
 titles[3]="Summe 1"; 
 titles[4]="Summe 2"; 
 setTitles(titles);
 set_NewNode(&MyTreeBase::NewNode);
}
