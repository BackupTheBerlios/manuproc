// $Id: Lief_Tree.cc,v 1.18 2001/10/02 16:04:46 cvs_christof Exp $
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

#include"Lief_TCList.h"
#include "Lief_RowData.h"
#include"MyMessage.h"
#include<ExtBezSchema/ExtBezSchema.h>
#include<Aux/Transaction.h>
#include<Aux/Ausgabe_neu.h>
#include <Artikel/Einheiten.h>
#include <tclistleaf.h>

extern MyMessage *meldung;

Lief_TCList::Lief_TCList(guint _cols, guint _attrs) 
	: TreeBase(_cols,_attrs)
{
 setColTitles();
 column_titles_show();
}


const std::string Lief_TCList::getColTitle(guint col) const
{
 switch(col)
  {
	case AUFNR_SEQ : return "Auftrag"; break;
	case ARTIKEL_SEQ : return "Artikel"; break;
	case LIEFMNG_SEQ : return "Liefermenge"; break;
	case LIEFZEILE_SEQ : return "Lief.Zeile"; break;
	default : return "-";
  }
}


TCListNode *Lief_TCList::NewNode(guint deep, const cH_EntryValue &v, bool expand)
{
 return new Lief_Node(deep,v,expand);
}

void Lief_TCList::showLieferschein(LieferscheinBase::ID lfrsid)
{
 if(lfrsid!=LieferscheinBase::none_id)
   {try{ lieferschein=LieferscheinVoll(lfrsid); }
    catch(SQLerror &e)
	{ meldung->Show(e); return; }
   }
 else lieferschein=LieferscheinVoll();

 detach_from_clist();
 
 datavec.erase(datavec.begin(),datavec.end());

 ExtBezSchema::ID ebzid=lieferschein.getSchema();

 std::vector<LieferscheinEntry>::const_iterator i=lieferschein.LsEntries().begin();
 std::vector<LieferscheinEntry>::const_iterator j=lieferschein.LsEntries().end();

 for(;i!=j;++i)
   datavec.push_back(cH_Lief_RowData(*i,ebzid));
 fillTCL();

 for(size_t i=0; i<columns().size(); ++i)
   set_column_auto_resize(i,true);

 attach_to_clist();

}


void Lief_TCList::newLieferschein(const Kunde::ID kid)
{
 Lieferschein *ls;
 try{ ls=new Lieferschein(cH_Kunde(kid)); }
 catch(SQLerror &e)
   {meldung->Show(e);}
 showLieferschein(ls->Id());
}


bool Lief_TCList::deleteLiefEntry()
{
 if(selection().size()==1)
   {
    TCListRow_API *tclapi=(TCListRow_API *)((*selection().begin()).get_data());
    TCListRowData *selectedrow=(TCListRowData*)(*tclapi).get_user_data();     
    if(!selectedrow->Leaf()) return false;
    cH_Lief_RowData entry((dynamic_cast<TCListLeaf*>(selectedrow))->LeafData());

    Transaction tr;

    try{
	lieferschein.deleteRow(entry->getLiefEntry());
 
	tr.commit();
    }
    catch(SQLerror &e) 
	{ meldung->Show(e); return false; }
    return true;
   }
 return false;
}

const cH_EntryValue Lief_RowData::Value(guint _seqnr,gpointer _gp) const
{
 switch(_seqnr)
   {
	case Lief_TCList::AUFNR_SEQ :
	   {  int aufid=liefentry.AufId();
	      if (aufid)
		 return cH_EntryValueIntString(Formatiere(aufid,0,6,"","",'0'));
	      else return cH_EntryValueIntString("");
	   }
	case Lief_TCList::ARTIKEL_SEQ :
	        if (liefentry.ZusatzInfo()) return cH_EntryValueIntString("");
		return cH_EntryValueIntString(artikelbez->Bezeichnung());
	case Lief_TCList::LIEFMNG_SEQ :
	        {  int stueck(GeliefertS());
	           fixedpoint<3> menge(GeliefertM());
                   std::string a;
                   if (stueck!=1) 
                   {  a=Formatiere(stueck)
                   	+ Einheit(liefentry.ArtikelID()).StueckEinheit();
                   }
                   if (menge.Scaled()!=0)
                   {  if (stueck!=1) a+="*";
                      a+=Formatiere_short(menge)
              		+ Einheit(liefentry.ArtikelID()).MengenEinheit();
                   }
                   if (liefentry.ZusatzInfo()) a="("+a+")";
                   return cH_EntryValueIntString(a);
		}
	case Lief_TCList::LIEFZEILE_SEQ :
		return cH_EntryValueIntString(liefentry.Zeile());
	default : return cH_EntryValue();
   }
}

Lief_Node::Lief_Node(guint deep, const cH_EntryValue &v, bool expand) 
 : TCListNode(deep,v,expand), sumgeliefert(0)
{}

void Lief_Node::cumulate(const cH_RowDataBase &rd)
{fixedpoint<3> menge(dynamic_cast<const Lief_RowData &>(*rd).GeliefertM());
 if (!menge) menge=1.0;
 sumgeliefert+=float(menge)*(dynamic_cast<const Lief_RowData &>(*rd)).GeliefertS();
}

const cH_EntryValue Lief_Node::Value(guint index,gpointer gp) const
{
 return cH_EntryValueIntString(sumgeliefert);
}
