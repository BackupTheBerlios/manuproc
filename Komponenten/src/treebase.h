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



#ifndef MYTREEBASE
#define MYTREEBASE

#include<TCList.h>
#include<queue>
#include<vector>
#include"tclistrowdata.h"

class TCListNode;
class TCListLeaf;
class cH_RowDataBase;


class TreeBase : public TCList
{
 guint showdeep; // int oder bool
 guint attrcount;
 queue<guint> seq; 
 
protected: 
 vector<cH_RowDataBase> datavec;

public:

 TreeBase(guint cols, guint attr=0);
 TreeBase(const vector<string> &cols);
 virtual void fillDataVec()=0;
 void fillTCL();
 
 static bool stutzen(TCListRow_API *parent, TCListRow_API *child,
 				TCList &tclist,guint deep);
 
 virtual const string getColTitle(guint seq) const;
 virtual void setColTitles();
 virtual void setSequence();
 guint Attrs() const { return attrcount; }
 guint Cols() const { return columns().size();}
 void insertIntoTCL(TCListRow_API *tclapi,const TreeBase &tb,
		 	const cH_RowDataBase &d, queue<guint> &q,guint deep);
 virtual TCListNode *NewNode(guint _seqnr, const cH_RowDataBase &v,
 				guint deep);			 	
 virtual TCListLeaf *NewLeaf(guint _seqnr, const cH_RowDataBase &v,
 				guint deep);			 	 				
};

#endif
