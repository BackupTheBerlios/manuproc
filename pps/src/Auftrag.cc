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

#include"AuftragFull.h"
#include<Gtk_OStream.h>
#include<Aux/Ausgabe_neu.h>
#include <gtk--/clist.h>

int AuftragFull::insertNewEntry(const aktAufEintrag &aufentry, const cH_ExtBezSchema &ebsh) throw (SQLerror)
{
 AufEintrag tmp(aufentry, auftragid, wrkstatus, ebsh);
 eintragliste.push_back(tmp);
 return tmp.getZnr();
}

void AuftragFull::fillCList(Gtk::CList &std::list)
{
 Gtk::OStream os(&std::list);
 Preis psum;

 for(AufEintragList::const_iterator i = eintragliste.begin();
	i!=eintragliste.end(); ++i)
    {os << (*i);	
     psum+=(*i).GPreis();
    }

 os << "\t\t\t\t\t\t\t-----------\n";
 os << "\t\t\t\t\t\tAuftragswert\t";
 os << Formatiere(psum.Wert()) << "\n";

 for(guint i=0; i<list.columns().size();++i)
   list.set_column_auto_resize(i,true);
}


void AuftragFull::fillCListEntry(Gtk::CList &std::list, int idx)
{
 Gtk::OStream os(&std::list, idx, (std::ios::out|std::ios::trunc), true);

 AufEintragList::const_iterator i;
 for(i = eintragliste.begin(); i!=eintragliste.end(); ++i);
 if(i!=eintragliste.end())
   os << (*i);	
}
