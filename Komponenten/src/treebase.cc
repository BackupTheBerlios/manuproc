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

// $Id: treebase.cc,v 1.25 2001/04/20 07:10:59 christof Exp $

#include"treebase.h"
#include"rowdata.h"
#include"tclistnode.h"
#include"tclistleaf.h"
#include<typeinfo>

const string TreeBase::getColTitle(guint seq) const
{
 return "-";
}

void TreeBase::setSequence()
{
 for(guint i=0; i<attrcount; ++i)
   seq.push(i);
}

TreeBase::TreeBase(guint cols, guint attr) : 
	TCList(cols), showdeep(0),attrcount(attr ? attr : cols) 
{
 setSequence();
}

void TreeBase::setColTitles()
{
 for(size_t i=0;i<columns().size();++i)
    set_column_title(i, getColTitle(i));
}

TreeBase::TreeBase(const vector<string> &cols) : 
		TCList(cols),showdeep(0),attrcount(cols.size()) 
{
 for(guint i=1; i<=attrcount; ++i)
   seq.push(i);
}


bool TreeBase::stutzen(TCListRow_API *parent, TCListRow_API *we,
						TCList &tclist,guint deep)
{
 if (we->size()==1)
 {  guint d=deep;
    while (we->size()==1)
    {  TCListRow *child_and_brother_to_be= &* (we->begin());
       TCListRow *we_as_a_row=static_cast<TCListRow*>(we);

       we->reparent_children(*parent);
       // copy non-empty attribute cells
       for (unsigned int i=0;i<=d;++i)
       {  const string t(we_as_a_row->get_text(i));
          if (t.size())
             child_and_brother_to_be->set_text(i,t);
       }
       tclist.erase(TCListRow_iterator(parent,static_cast<TCListRow*>(we)));
       we=child_and_brother_to_be;
       d++;
    }
    return true;
 }

 if(we->size()>1)
   {TCListRow_API::iterator i = we->begin();
    while(i!=we->end())
      {
        if (stutzen(we,&*i,tclist,deep+1)) return true;
	i++;
      }
   }
  return false;
}


void TreeBase::fillTCL()
{
 fillDataVec();

 vector<cH_RowDataBase>::const_iterator i=datavec.begin();
 vector<cH_RowDataBase>::const_iterator j=datavec.end();
 queue<guint> selseq;

 for(; i!=j; ++i)
   {
    selseq=seq;
    insertIntoTCL((TCListRow_API*)this,*this,*i,selseq,0);
   }

 collapse();
 for(TCListRow_API::iterator i = begin(); i!=end();)
   {if (stutzen((TCListRow_API*)this,(TCListRow_API*)(&(*i)),*this,0))
      i=begin();
    else ++i;
   }

 for(TCListRow_API::iterator i = begin(); i!=end(); ++i)
   ((TCListRowData*)(*i).get_user_data())->refreshSum(*this);
 expand();
}


void TreeBase::insertIntoTCL(TCListRow_API *tclapi,const TreeBase &tb,
				const cH_RowDataBase &v,queue<guint> &selseq,guint deep)
{
 TCListRow_API::iterator lfind = tclapi->begin();
 TCListRow_API::iterator lend = tclapi->end();

 guint seqnr=selseq.front();	

 cH_EntryValue ev=v->Value(seqnr);

 while((lfind!=lend) &&
   *( reinterpret_cast<TCListRowData*>((*lfind).get_user_data())->Value()) < (*ev))
   lfind++;
	
 if(lfind!=lend)
   {
    if((*ev) == *reinterpret_cast<TCListRowData*>((*lfind).get_user_data())->Value())
     {
      reinterpret_cast<TCListNode*>((*lfind).get_user_data())->cumulate(v,seqnr);
      if(!reinterpret_cast<TCListNode*>((*lfind).get_user_data())->Leaf())
	{selseq.pop();
         insertIntoTCL((&*lfind),tb,v,selseq,++deep);
	}
      return;
     }

    if(selseq.size()>1) // noch nicht am Blatt
	{
	 TCListNode *newnode=NewNode(seqnr,v,deep);
	 newnode->cumulate(v,seqnr);
	 newnode->initTCL(tclapi,lfind,tb,deep);
         selseq.pop();
	 insertIntoTCL(newnode->getTCL_API(),tb,v,selseq,++deep);
	}
     else
	{
	 TCListLeaf *newleaf=NewLeaf(seqnr,v,deep); 
	 newleaf->initTCL(tclapi,lfind,tb,deep);
	}
   }
 else
   {
    if(selseq.size()>1) // noch nicht am Blatt
	{
	 TCListNode *newnode=NewNode(seqnr,v,deep);
	 newnode->cumulate(v,seqnr);
	 newnode->initTCL(tclapi,tb,deep);
         selseq.pop();
	 insertIntoTCL(newnode->getTCL_API(),tb,v,selseq,++deep);
	}
     else
	{
	 TCListLeaf *newleaf=NewLeaf(seqnr,v,deep); 
	 newleaf->initTCL(tclapi,tb,deep);
	}
   }

}


TCListNode *TreeBase::NewNode(guint _seqnr, const cH_RowDataBase &v, 
				guint deep)
{
 return new TCListNode(_seqnr,v,deep); 
}

TCListLeaf *TreeBase::NewLeaf(guint _seqnr, const cH_RowDataBase &v, 
				guint deep)
{
 return new TCListLeaf(_seqnr,v,deep); 
}


