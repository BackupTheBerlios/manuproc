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

#include <Auftrag/AuftragFull.h>
#include <Aux/Ausgabe_neu.h>
#include <Gtk_OStream.h>
#include <gtk--/clist.h>
#include "AufEintrag.h"

int AuftragFull::insertNewEntry(const aktAufEintrag &aufentry, const cH_ExtBezSchema &ebsh) throw (SQLerror)
{
// AufEintrag tmp(aufentry, AuftragBase((ppsInstanz::ID)instanz,auftragid), wrkstatus, ebsh);
 AufEintrag tmp(aufentry, AuftragBase(instanz,auftragid), wrkstatus, ebsh);
 eintragliste.push_back(tmp);
 return tmp.getZnr();
}

void AuftragFull::fillCList(Gtk::CList &list)
{
 Gtk::OStream os(&list);
 Preis psum;

 for(const_iterator i = begin(); i!=end(); ++i)
    {
      cH_ArtikelBezeichnung AB(ArtikelBase(i->ArtikelID()));
      os << i->getStueck()<<"\t"<<AB->Bezeichnung()<<"\t"<<i->getMeter()<<"\t"
        << i->getRest()<<"\t"<<i->GPreis().Wert()<<"\t"
        <<i->getLieferdatum()
        << "\t"<<i->getEntryStatusStr()<<"\t"<<i->LastEditDate()<<"\n";
//    os << AufEintrag(*i);	
     psum+=(*i).GPreis();
    }

/*
 os << "\t\t\t\t\t\t\t-----------\n";
 os << "\t\t\t\t\t\tAuftragswert\t";
 os << Formatiere(psum.Wert()) << "\n";
*/
 os << "\t\t\t\t-----------\n";
 os << "\t\t\tAuftragswert\t";
 os << Formatiere(psum.Wert()) << "\n";

 for(guint i=0; i<list.columns().size();++i)
   list.set_column_auto_resize(i,true);
}

#if 0
void AuftragFull::fillCListEntry(Gtk::CList &list, int idx)
{
 Gtk::OStream os(&list, idx, (ios::out|ios::trunc), true);

 for(const_iterator i = begin(); i!=end(); ++i)
    os << AufEintrag(*i);	
}
#endif
