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

void AuftragFull::fillCList(Gtk::CList &list)
{
 list.freeze();
 list.clear();
 Gtk::OStream os(&list);
 Preis psum;

 for(const_iterator i = begin(); i!=end(); ++i)
    {
      cH_ArtikelBezeichnung AB(ArtikelBase(i->ArtId()));
      os << i->getStueck()<<"\t"<<AB->Bezeichnung()<<"\t"/*<<i->getMeter()*/<<"\t"
        << i->getRestStk()<<"\t"<<i->GPreis().Wert()<<"\t"
        <<i->getLieferdatum()
        << "\t"<<i->getEntryStatusStr()<<"\t"<<i->LastEditDate()<<"\n";
     psum+=(*i).GPreis();
    }

 os << "\t\t\t\t-----------\n";
 os << "\t\t\tAuftragswert\t";
 os << Formatiere(psum.Wert()) << "\n";

 for(guint i=0; i<list.columns().size();++i)
   list.set_column_auto_resize(i,true);
 list.thaw();
}

