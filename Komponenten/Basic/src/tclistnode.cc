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

// $Id: tclistnode.cc,v 1.1 2001/06/21 09:05:27 christof Exp $

#include"tclistnode.h"
#include"rowdata.h"
#include"Aux/Ausgabe_neu.h"

void TCListNode::cumulate(const cH_RowDataBase &rd, int seqnr) const
{
 sumvalue+=rd->Value(seqnr)->getIntVal();
}

const string TCListNode::getSumCol(int col)
{
 return Formatiere(sumvalue);
}

TCListNode::TCListNode(int _seqnr, const cH_RowDataBase &v, 
			int deep) :
	TCListRowData(v->Value(_seqnr),deep,false),show(false), sumvalue(0)
{}

//#include <typeinfo>

void TCListNode::initTCL(TCListRow_API *api, TCListRow_API::iterator davor,
			const TreeBase &tb,int deep)
{
// cerr << typeid(*this).name() << '\n';
 listrow = api->insert(davor, getColEntries(tb.Cols()),deep+1,show);
 listrow->set_user_data(this);
}

void TCListNode::initTCL(TCListRow_API *api, const TreeBase &tb,int deep)
{
//cerr << typeid(*this).name() << '\n';
 listrow = api->push_back(getColEntries(tb.Cols()),deep+1,show);
 listrow->set_user_data(this);
}


void TCListNode::refreshSum(const TreeBase &tb)
{
 TCListRow_API::iterator i = listrow->begin();
 
 int scols=tb.Cols()-tb.Attrs();

 for(int i=0;i<scols;i++)
   dynamic_cast<TCListRow*>(listrow)->relabel(tb.Attrs()+i, getSumCol(i));

 while(i!=listrow->end())
   {
    if( !((TCListRowData*)(*i).get_user_data())->Leaf() )
	 ((TCListNode*)(*i).get_user_data())->refreshSum(tb);
    ++i;
   }
}

