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

// $Id: tclistnode.cc,v 1.6 2001/10/02 15:28:01 christof Exp $

#include <tclistnode.h>
#include <Aux/EntryValueIntString.h>

const cH_EntryValue TCListNode::Value(guint col, gpointer gp) const
{ return cH_EntryValueIntString("");
}

// errechnete Summe in CList anzeigen
void TCListNode::refreshSum(const TreeBase &tb)
{for(guint i=tb.Attrs();i<tb.Cols();i++)
   try { dynamic_cast<TCListRow*>(listrow)
   	->relabel(i, Value(i,tb.ValueData())->getStrVal());
   } catch(...)
   { assert(!"listrow is a TCListRow"); }

 for (TCListRow_API::iterator i = listrow->begin();i!=listrow->end();++i)
 {  if (!((TCListRowData*)(*i).get_user_data())->Leaf())
	 ((TCListNode*)(*i).get_user_data())->refreshSum(tb);
 }
}

const std::vector<std::string> TCListNode::getColEntries(const TreeBase &tb) const
{std::vector<std::string> v(tb.Cols());
 for(guint i=0; i<tb.Cols(); ++i)
    v[i]= (i==(guint)deep ? value->getStrVal() : "");
 return v;
}
