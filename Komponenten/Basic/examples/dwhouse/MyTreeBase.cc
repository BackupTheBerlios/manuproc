// $Id: MyTreeBase.cc,v 1.6 2002/07/05 12:36:56 christof Exp $
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


#include"MyTreeBase.hh"
#include"MyRowData.hh"
#include"MyNode.h"

void MyTreeBase::fillDataVec()
{
 datavec.push_back(cH_MyRowData(1,3,3,10,11)); 
 datavec.push_back(cH_MyRowData(2,2,3,10,11)); 
 datavec.push_back(cH_MyRowData(2,2,4,10,11)); 
 datavec.push_back(cH_MyRowData(2,3,4,10,11)); 
 datavec.push_back(cH_MyRowData(3,2,3,10,11)); 
 datavec.push_back(cH_MyRowData(3,2,4,11,11)); 
 datavec.push_back(cH_MyRowData(4,2,4,10,11)); 

}


const std::string MyTreeBase::getColTitle(guint col) const
{   
 switch(col)
  {
        case 0 : return "Attribut 1"; break;
        case 1 : return "Attribut 2"; break;
        case 2 : return "Attribut 3"; break;
        case 3 : return "Summe 1"; break;
        case 4 : return "Summe 2"; break;
        default : return "-";
  } 
}

 
TCListNode *MyTreeBase::NewNode(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data,bool expand)
{
 return new  MyNode(deep, v, child_s_deep,child_s_data,expand);
}

