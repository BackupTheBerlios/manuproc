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

#include"Auftrag.h"
#include<Gtk_OStream.h>
#include<Aux/Ausgabe_neu.h>

int Auftrag::insertNewEntry(aktAufEintrag &aufentry, const cH_ExtBezSchema &ebsh)
{
 AufEintrag *tmp;
 bool tried=false;
 int znr;

 try {tmp = new AufEintrag(aufentry, auftragid, wrkstatus, ebsh); }
 catch(SQLerror &e)
 {if((e.Code()==100) && !tried)
    {meldung->Message::Show("Artikel existiert noch nicht und\n"
		    "wird auch nicht mehr automatisch erzeugt");
     return 0; }
  else
    {meldung->Show(e);
     return 0;}
  }

 eintragliste.push_back(*tmp);   
 znr=tmp->getZnr();
 delete(tmp);
 return znr;
}



void Auftrag::fillCList(Gtk::CList &list)
{
 Gtk::OStream os(&list);
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


void Auftrag::fillCListEntry(Gtk::CList &list, int idx)
{
 Gtk::OStream os(&list, idx, (ios::out|ios::trunc), true);

 AufEintragList::const_iterator i;
 for(i = eintragliste.begin(); i!=eintragliste.end(); ++i);
 if(i!=eintragliste.end())
   os << (*i);	
}



