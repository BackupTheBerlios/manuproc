/*  pps: ManuProC's ProductionPlanningSystem
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


#ifndef PPS_OFFLIEFTREE
#define PPS_OFFLIEFTREE

#include<treebase.h>
#include<Lieferschein/Lieferschein.h>
#include"Lief_Value.h"
#include<Aux/Datum.h>
#include<rowdata.h>
#include<Kunde/Kunde.h>

// OffLief_TCList begin

#include<Aux/SQLerror.h>

class OffLief_TCList : public TreeBase
{

 cH_Kunde kunde;

 void loadOffLief() throw(SQLerror);
  
public:

 typedef enum {LIEFNR_SEQ,LIEFDATUM_SEQ,LIEFKUNDE_SEQ} SeqNr;

 OffLief_TCList(guint _cols, guint _attrs);
 const string getColTitle(guint seq) const;
 void fillDataVec() {}
 void showOffLief();
 void setKunde(const cH_Kunde kd) { kunde=kd; };
 			
 TCListNode *NewNode(guint _seqnr, const cH_RowDataBase &v, guint deep);

 TCListLeaf *NewLeaf(guint _seqnr, const cH_RowDataBase &v, guint deep);

};



// OffLief_TCList end

// OffLief_RowData begin 

#include<Lieferschein/LieferscheinBase.h>

class OffLief_RowData : public RowDataBase
{
 friend class cH_RowData;
 friend class const_Handle<OffLief_RowData>;

 Lieferschein lieferschein;
  
public:
 OffLief_RowData(const Lieferschein &l) : lieferschein(l) {}
 const cH_EntryValue Value(int _seqnr) const;

 LieferscheinBase::ID LiefId() const { return lieferschein.Id(); }
 const Lieferschein &Lief() const  { return lieferschein; }
};


class cH_OffLief_RowData : public cH_RowDataBase
{
private:
 
 cH_OffLief_RowData(const OffLief_RowData *r) : cH_RowDataBase(r) {}
 cH_OffLief_RowData() : cH_RowDataBase() {}
 
public:
 cH_OffLief_RowData(const Lieferschein &l)
   : cH_RowDataBase(new OffLief_RowData(l))
 {}
 cH_OffLief_RowData(const cH_RowDataBase &b):
 	cH_RowDataBase(dynamic_cast<const OffLief_RowData*>(&*b))
 {}
 
 const OffLief_RowData &operator*() const
        {  return (OffLief_RowData&)(this->cH_RowDataBase::operator*());
        }         
        
 const OffLief_RowData *operator->() const
        {  return (OffLief_RowData*)(this->cH_RowDataBase::operator->());
        }                 
};


// OffLief_RowData end



// OffLief_Node begin

#include<tclistnode.h>

class OffLief_Node : public TCListNode
{
 
public:
 
 OffLief_Node::OffLief_Node(int _seqnr, const cH_RowDataBase &v, int deep);

};

// OffLief_Node end


// OffLief_Leaf begin
#include<tclistleaf.h>

class OffLief_Leaf : public TCListLeaf
{
 
public:
 
 OffLief_Leaf::OffLief_Leaf(int _seqnr, const cH_RowDataBase &v,int deep) 
		: TCListLeaf(_seqnr,v,deep) {}
};
// OffLief_Leaf end


#endif
