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


#include"OffLief_Tree.h"
#include"MyMessage.h"

extern MyMessage *meldung;

// OffLief_TCList begin

OffLief_TCList::OffLief_TCList(guint _cols, guint _attrs)
:TreeBase(_cols,_attrs), kunde(Kunde::none_id)
{
 setColTitles();
 column_titles_show();
}


const string OffLief_TCList::getColTitle(guint col) const
{
 switch(col)
  {
	case LIEFNR_SEQ : return "Lieferschein"; break;
	case LIEFDATUM_SEQ : return "Liefersch. Datum"; break;
	case LIEFKUNDE_SEQ: return "Lieferung an";
	default : return "-";
  }
}


void OffLief_TCList::showOffLief()
{
 freeze();

 if(kunde->Id() != Kunde::none_id)
   {
    try{loadOffLief();}
    catch(SQLerror &e) { meldung->Show(e); return; }
    fillTCL();
   }
 else
   {datavec.erase(datavec.begin(),datavec.end());
    clear();
   }

 for(size_t i=0; i<columns().size(); ++i)
   set_column_auto_resize(i,true);

 thaw();
}

/*
TCListNode *OffLief_TCList::NewNode(guint deep,const cH_EntryValue &v, bool expand)
{
 return new OffLief_Node(deep,v,expand);
}
*/
// OffLief_TCList end



// OffLief_Node begin

/*
OffLief_Node::OffLief_Node(guint deep, const cH_EntryValue &v, bool expand) 
 : TCListNode(deep,v,expand)
{}
*/

// OffLief_Node end
#include<Aux/Ausgabe_neu.h>
#include <Aux/EntryValueIntString.h>

const cH_EntryValue OffLief_RowData::Value(guint _seqnr,gpointer _gp) const
{
 switch(_seqnr)
   {
	case OffLief_TCList::LIEFNR_SEQ :
		return cH_EntryValueIntString(Formatiere(lieferschein.Id(),0,6,"","",'0'));
	case OffLief_TCList::LIEFDATUM_SEQ :
		return cH_EntryValueIntString(lieferschein.LsDatum().c_str());
	case OffLief_TCList::LIEFKUNDE_SEQ:
		return cH_EntryValueIntString(cH_Kunde(lieferschein.KdNr())->firma());
	default : return cH_EntryValueIntString("-");
   }
}
