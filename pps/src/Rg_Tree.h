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



#ifndef PPS_RECHNUNGTREE
#define PPS_RECHNUNGTREE

#include<treebase.h>
#include<Lieferschein/Rechnung.h>
#include<Lieferschein/RechnungEntry.h>
#include<Aux/EntryValueIntString.h>
#include<Aux/Datum.h>
#include<rowdata.h>
#include<Artikel/ArtikelBezeichnung.h>
#include<Artikel/Preis.h>

// Rg_TCList begin

class Rg_TCList : public TreeBase
{

 Rechnung rechnung;
 
public:

 typedef enum {LIEFZEILE_SEQ,LIEFNR_SEQ,ARTIKEL_SEQ,LIEFMNG_SEQ,PREIS_SEQ,GPREIS_SEQ} SeqNr;

 Rg_TCList(guint _cols, guint _attrs);
 const std::string getColTitle(guint seq) const;
 void fillDataVec() {}
 void showRechnung(const Rechnung::ID rngid); 
 void newRechnung(const Kunde::ID kid);
 const Rechnung &getRechnung() const { return rechnung; }
 // brauche ich zum Preis setzen, einverstanden? CP
 Rechnung &getRechnung() { return rechnung; }
 void setRechnung(const RechnungBase::ID rngid) 
 			{ showRechnung(rngid); }
 			
 bool deleteRngEntry();

// TCListNode *NewNode(guint _seqnr, gpointer gp,const cH_RowDataBase &v, guint deep);
// TCListLeaf *NewLeaf(guint _seqnr, gpointer gp,const cH_RowDataBase &v, guint deep);

};

// Rg_TCList end

// Rg_RowData begin 

#include<Lieferschein/LieferscheinBase.h>

class Rg_RowData : public RowDataBase
{
 friend class cH_RowData;
 friend class const_Handle<Rg_RowData>;

 RechnungEntry rechnungentry;
 cH_ArtikelBezeichnung artikelbez;
  
public:
 Rg_RowData(const RechnungEntry &le, ExtBezSchema::ID ebzid) : 
 	rechnungentry(le), artikelbez(le.ArtikelID(),ebzid,ArtikelBezeichnung::dont_throw()) {}

 const cH_EntryValue Value(guint _seqnr,gpointer _gp) const;
 const ArtikelBase::ID ArtikelID() const { return artikelbez->Id(); }
 int Zeile() const { return rechnungentry.Zeile(); }
 LieferscheinBase::ID LiefId() const { return rechnungentry.Lfrs_Id(); }
 int LiefZNr() const { return rechnungentry.Lfrs_ZNr(); }
 const Preis preis()  const { return rechnungentry.getPreis(); }
 const Preis gpreis()  const { return rechnungentry.GPreis(); }
 const RechnungEntry &getRgEntry() const { return rechnungentry; }
};


class cH_Rg_RowData : public cH_RowDataBase
{
private:
 
 cH_Rg_RowData(const Rg_RowData *r) : cH_RowDataBase(r) {}
 cH_Rg_RowData() : cH_RowDataBase() {}
 
public:
 cH_Rg_RowData(const RechnungEntry &le, ExtBezSchema::ID ebzid)
   : cH_RowDataBase(new Rg_RowData(le,ebzid))
 {}
 cH_Rg_RowData(const cH_RowDataBase &b):
 	cH_RowDataBase(dynamic_cast<const Rg_RowData*>(&*b))
 {}
 
 const Rg_RowData &operator*() const
        {  return (Rg_RowData&)(this->cH_RowDataBase::operator*());
        }         
        
 const Rg_RowData *operator->() const
        {  return (Rg_RowData*)(this->cH_RowDataBase::operator->());
        }                 
};


// Rg_RowData end

// Rg_Node begin

#include<tclistnode.h>

class Rg_Node : public TCListNode
{
 mutable fixedpoint<2> sumpreis;
 
public:
 
 virtual void cumulate(const cH_RowDataBase &rd);
 const cH_EntryValue Value(guint index,gpointer gp) const;
  
 Rg_Node::Rg_Node(guint deep,const cH_EntryValue &v, bool expand);
 fixedpoint<2> SumPreis() const { return sumpreis; }

};

// Rg_Node end

// Rg_Value begin

class Rg_Value: public EntryValueIntString
{
 fixedpoint<2> fval;

public:
 Rg_Value(int v) : EntryValueIntString(v), fval(v) {}
 Rg_Value(const std::string &s) : EntryValueIntString(s), fval(atof(s.c_str())) {}
 Rg_Value(fixedpoint<2> f) : EntryValueIntString(f.String()), fval(f) {}
};

class cH_Rg_Value : public cH_EntryValue
{
public:

 cH_Rg_Value() : cH_EntryValue() {}
 cH_Rg_Value(const EntryValueBase *r) : cH_EntryValue(r) {}
 
 cH_Rg_Value(int v) : cH_EntryValue(new Rg_Value(v)) {}
 cH_Rg_Value(const std::string &s) : cH_EntryValue(new Rg_Value(s)) {}
 cH_Rg_Value(fixedpoint<2> f) : cH_EntryValue(new Rg_Value(f)) {}
    
};

// Rg_Value end

#endif
