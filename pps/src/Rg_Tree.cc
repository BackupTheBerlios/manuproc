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



#include"Rg_Tree.h"
#include"MyMessage.h"
#include<ExtBezSchema/ExtBezSchema.h>

extern MyMessage *meldung;

// Rg_TCList begin

Rg_TCList::Rg_TCList(guint _cols, guint _attrs) 
	:TreeBase(_cols,_attrs), rechnung(RechnungBase::none_id)
{
 setColTitles();
 column_titles_show();
}

const string Rg_TCList::getColTitle(guint col) const
{
 switch(col)
  {
	case LIEFNR_SEQ : return "Lieferschein"; break;
	case ARTIKEL_SEQ : return "Artikel"; break;
	case LIEFMNG_SEQ : return "Liefermenge"; break;
	case LIEFZEILE_SEQ : return "Zeile"; break;
	case PREIS_SEQ : return "E-Preis"; break;
	case GPREIS_SEQ : return "G-Preis"; break;
	default : return "-";
  }
}

TCListNode *Rg_TCList::NewNode(guint _seqnr,const cH_RowDataBase &v, guint deep)
{
 return new Rg_Node(_seqnr,v,deep);
}

TCListLeaf *Rg_TCList::NewLeaf(guint _seqnr,const cH_RowDataBase &v, guint deep)
{
 return new Rg_Leaf(_seqnr,v,deep);
}

void Rg_TCList::showRechnung(RechnungBase::ID rngid)
{
 if(rngid!=RechnungBase::none_id)
   {try{ rechnung=Rechnung(rngid); }
    catch(SQLerror &e)
	{ meldung->Show(e); return; }
   }
 else rechnung=Rechnung(RechnungBase::none_id);

 detach_from_clist();
 
 datavec.erase(datavec.begin(),datavec.end());

 ExtBezSchema::ID ebzid=rechnung.getSchema();

 Rechnung::const_iterator i=rechnung.begin();
 Rechnung::const_iterator j=rechnung.end();

 for(;i!=j;++i)
   datavec.push_back(cH_Rg_RowData(*i,ebzid));
 fillTCL();

 for(size_t i=0; i<columns().size(); ++i)
   set_column_auto_resize(i,true);

 attach_to_clist();

}

void Rg_TCList::newRechnung(const Kunde::ID kid)
{

 try{ rechnung=Rechnung(cH_Kunde(kid)); }
 catch(SQLerror &e)
   {meldung->Show(e);}

 showRechnung(rechnung.Id());
}

#include<Aux/Transaction.h>

bool Rg_TCList::deleteRngEntry()
{
 if(selection().size()==1)
   {
    TCListRow_API *tclapi=(TCListRow_API *)((*selection().begin()).get_data());
    TCListRowData *selectedrow=(TCListRowData*)(*tclapi).get_user_data();     
    if(!selectedrow->Leaf()) return false;
    cH_Rg_RowData entry((dynamic_cast<TCListLeaf*>(selectedrow))->LeafData());

    Transaction tr;

    try{
//	rechnung.deleteRow(entry->getRgEntry());
 
	tr.commit();
    }
    catch(SQLerror &e) 
	{ meldung->Show(e); return false; }
    return true;
   }
 return false;
}

// Rg_Tree end

// Rg_Node begin
#include<Aux/Ausgabe_neu.h>

Rg_Node::Rg_Node(int _seqnr, 
		const cH_RowDataBase &v, int deep) 
 : TCListNode(_seqnr,v,deep), sumpreis(0)
{}

void Rg_Node::cumulate(const cH_RowDataBase &rd, int seqnr) const
{
 sumpreis+=(dynamic_cast<const Rg_RowData &>(*rd)).preis().Wert();
}

void Rg_Node::resetSumValues(gpointer p)
{
 sumpreis=((Rg_Node*)p)->SumPreis();
}

const string Rg_Node::getSumCol(int col)
{
 return Formatiere(sumpreis);
}

const vector<string> Rg_Node::getColEntries(int cols)
{
 static vector<string> v;

 v=TCListRowData::getColEntries(cols);

 v[Rg_TCList::LIEFMNG_SEQ]=Formatiere(sumpreis);

// cout << "getColEntries Node\n";
 return v;
}

// Rg_Node end

// Rg_Leaf begin
const vector<string> Rg_Leaf::getColEntries(int cols)
{
 static vector<string> v;
 v=TCListRowData::getColEntries(cols);

 v[Rg_TCList::PREIS_SEQ]=
	Formatiere((dynamic_cast<const Rg_RowData&>(*leafdata)).preis().Wert());
 v[Rg_TCList::GPREIS_SEQ]=
	Formatiere((dynamic_cast<const Rg_RowData&>(*leafdata)).gpreis().Wert());
 return v;

}

// Rg_Leaf end

// Rg_RowData begin
const cH_EntryValue Rg_RowData::Value(int _seqnr) const
{
 switch(_seqnr)
   {
	case Rg_TCList::LIEFNR_SEQ :
		return cH_Rg_Value(Formatiere(rechnungentry.Lfrs_Id(),0,6,"","",'0'));
		break;
	case Rg_TCList::ARTIKEL_SEQ :
		return cH_Rg_Value(artikelbez->Bezeichnung());
		break;
	case Rg_TCList::LIEFMNG_SEQ :
		return cH_Rg_Value(rechnungentry.Menge());
		break;
	case Rg_TCList::LIEFZEILE_SEQ :
		return cH_Rg_Value(rechnungentry.Zeile());
		break;
	case Rg_TCList::PREIS_SEQ :
		return cH_Rg_Value(rechnungentry.getPreis().Wert());
		break;
	case Rg_TCList::GPREIS_SEQ :
		return cH_Rg_Value(rechnungentry.GPreis().Wert());
		break;
	default : return cH_Rg_Value("-");
   }
 return cH_Rg_Value("-");
}

// Rg_RowData end



