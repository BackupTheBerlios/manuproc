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


#include"OffAuf_TCList.hh"
#include<vector>
#include"MyMessage.h"
#include"OffAuf_RowData.h"
#include"OffAuf_Node.h"
#include"OffAuf_Leaf.h"

extern MyMessage *meldung;

OffAuf_TCList::OffAuf_TCList(guint _cols, guint _attrs) 
	: TreeBase(_cols,_attrs),kundennr(Kunde::default_id)
{

 setColTitles();
 column_titles_show();
}

void OffAuf_TCList::showOffAuf()
{
 if(kundennr!=Kunde::none_id)
   {
    try{loadOffAuf();}
    catch(SQLerror &e) { meldung->Show(e); return; }
   }

 freeze();
 fillTCL();

 for(size_t i=0; i<columns().size(); ++i)
   set_column_auto_resize(i,true);
 thaw();

}

const string OffAuf_TCList::getColTitle(guint col) const
{
 switch(col)
  {
	case AUFNR_SEQ : return "Auftrag"; break;
	case ARTIKEL_SEQ : return "Artikel"; break;
	case OFFMNG_SEQ : return "offen"; break;
	case GELIEF_SEQ : return "geliefert"; break;
	case LIEFDAT_SEQ : return "Lieferdatum"; break;
	default : return "-";
  }
}

TCListNode *OffAuf_TCList::NewNode(guint deep, const cH_EntryValue &v, bool expand)
{
 return new OffAuf_Node(deep,v,expand);
}

#include<Aux/Ausgabe_neu.h>
#include"OffAuf_Node.h"
#include"OffAuf_RowData.h"

OffAuf_Node::OffAuf_Node(guint deep, const cH_EntryValue &v, bool expand) 
 : TCListNode(deep,v,expand), sumoffen(0),sumgeliefert(0)
{}

void OffAuf_Node::cumulate(const cH_RowDataBase &rd)
{
 sumoffen+= (dynamic_cast<const OffAuf_RowData &>(*rd)).Offen();
 sumgeliefert+=(dynamic_cast<const OffAuf_RowData &>(*rd)).Geliefert();
}

const cH_EntryValue OffAuf_Node::Value(guint index,gpointer gp) const
{
 switch(index)
   { case 0 :
	return cH_EntryValueIntString(sumoffen);
	break;
     case 1 :
	return cH_EntryValueIntString(sumgeliefert);
	break;
     default : return(cH_EntryValueIntString("-"));
   }
}

#include<Aux/Ausgabe_neu.h>
#include"OffAuf_Leaf.h"
#include"OffAuf_RowData.h"

#include"OffAuf_RowData.h"
#include"OffAuf_TCList.hh"
#include <Aux/Ausgabe_neu.h>

#warning cH_OffAuf_Value kann weg

const cH_EntryValue OffAuf_RowData::Value(guint _seqnr,gpointer _gp) const
{
 switch(_seqnr)
   {
	case OffAuf_TCList::AUFNR_SEQ :
		return cH_EntryValueIntString(Formatiere(auftrag.AufId(),0,6,"","",'0'));
	case OffAuf_TCList::ARTIKEL_SEQ :
		return cH_EntryValueIntString(artikel->Bezeichnung());
	case OffAuf_TCList::LIEFDAT_SEQ :
		return cH_EntryValueIntString(liefdatum.c_str());
	default : return cH_EntryValueIntString("-");
   }
}

