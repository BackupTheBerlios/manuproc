// $Id: tclistrowdata.cc,v 1.8 2002/07/05 12:36:56 christof Exp $
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

// $Id: tclistrowdata.cc,v 1.8 2002/07/05 12:36:56 christof Exp $

#include <tclistrowdata.h>
#include <treebase.h>

void TreeRow::initTCL(TCListRow_API *api, TCListRow_API::iterator davor,
			const TreeBase &tb) // ,int deep)
{
 listrow = api->insert(davor, getColEntries(tb),Leaf()?-1:int(Children_s_Deep()),expand);
 listrow->set_user_data(this);
}

void TreeRow::initTCL(TCListRow_API *api, const TreeBase &tb) // ,int deep)
{
 listrow = api->push_back(getColEntries(tb),Leaf()?-1:int(Children_s_Deep()),expand);
 listrow->set_user_data(this);
}

// deep kann sinnvollerweise eigentlich nur erhöht werden
void TreeRow::ValueDeep(const cH_EntryValue &ev, guint _deep)
{  // Felder löschen !
   for (guint i=Deep();i<_deep;++i)
      static_cast<TCListRow*>(getTCL_API())->set_text(i,"");
   value=ev; 
   deep=_deep;
}

// childrens_deep kann sinnvollerweise eigentlich nur erniedrigt werden
void TreeRow::Children_s_Deep(guint _deep)
{  // Felder löschen !
   for (guint i=_deep;i<Children_s_Deep();++i)
      static_cast<TCListRow*>(getTCL_API())->set_text(i,"");
   childrens_deep=_deep;
}

// errechnete Summe in CList anzeigen, nur für Nodes sinnvoll
// rekursiv!
void TreeRow::refreshSum(const TreeBase &tb)
{if (Leaf() || tb.Attrs()==tb.Cols()) return;
// std::deque<guint> order=tb.get_seq();
 
 for(guint i=tb.Attrs();i<tb.Cols();i++)
   try { dynamic_cast<TCListRow*>(listrow)
   	->relabel(i, Value(i,tb.ValueData())->getStrVal());
   } catch(...)
   { assert(!"listrow is a TCListRow"); }

 for (TCListRow_API::iterator i = listrow->begin();i!=listrow->end();++i)
 {  if (!((TreeRow*)(*i).get_user_data())->Leaf())
	 ((TreeRow*)(*i).get_user_data())->refreshSum(tb);
 }
}

// leaves/nodes zeigen erst ab deep an
// leaves zeigen bis zum Ende an, Nodes bis Children_s_Deep
// Summen kommen hier nicht so vor ...

// Sehr optimiert, da sämtliche Aufrufe an TreeRow::Value(col,gp) 
// abgekürzt sind
const std::vector<std::string> TreeRow::getColEntries(const TreeBase &tb) const
{std::vector<std::string> v(tb.Cols());
 std::deque<guint> order=tb.get_seq();

 v[deep]=value->getStrVal();
 
 if (Leaf())
 {  for(guint i=deep+1;i<(guint)tb.Attrs();++i)
        v[i]=LeafData()->Value(order[i],tb.ValueData())->getStrVal();
    for(guint i=tb.Attrs();i<(guint)tb.Cols();++i)
        v[i]=LeafData()->Value(i,tb.ValueData())->getStrVal();
 }
 else // Summen werden später gesetzt
    for(guint i=deep+1;i<(guint)Children_s_Deep();++i)
        v[i]=LeafData()->Value(order[i],tb.ValueData())->getStrVal();
 return v;
}

const cH_EntryValue TreeRow::Value(guint col, gpointer gp) const
{ if (col<Deep() || (!Leaf() && col>=Children_s_Deep()))
     return cH_EntryValue();
  return LeafData()->Value(col, gp);
}

