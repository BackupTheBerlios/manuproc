// $Id: instanz_auftrag.cc,v 1.1 2002/01/02 15:31:59 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <iostream>
#include <Auftrag/AuftragFull.h>
#include <Kunde/Kunde.h>
#include <Aux/exception.h>
#include <Aux/dbconnect.h>

int main()
{  Petig::PrintUncaughtExceptions();
   Petig::dbconnect();
   
   AuftragBase id;
   {  Auftrag a=Auftrag(Auftrag::Anlegen(ppsInstanz::Kundenauftraege), Kunde::default_id);
      id=a;
      std::cout << "Auftrag " << a.Id() << '\n';
   
      Petig::Datum date(8,1,2010);
      a.insertNewEntry(3000, date, 218843,UNCOMMITED,true);
//      a.insertNewEntry(2000, date, 218849,UNCOMMITED,true);
   }
   
   AufEintragBase2 zeile;
   {  AuftragFull af(id);
      for (AuftragFull::const_iterator i=af.begin(); i!=af.end(); ++i)
      {  zeile=*i;
         std::cout << cH_ppsInstanz(i->Instanz())->Name()<<' '<<i->getStueck() << "m " 
            << i->getRestStk() << "mR ("
         	<< i->getRestStk() << "R) " << i->getLieferdatum() << " "
         	<< cH_ArtikelBezeichnung(i->Id())->Bezeichnung()
         	<< " " << int(i->getEntryStatus()) << '\n';
      }
   }
/*   
   cout << "1x abschreiben " << zeile.abschreiben(15000) << '\n';

   {  AuftragFull af(id);
      for (AuftragFull::const_iterator i=af.begin(); i!=af.end(); ++i)
      {  std::cout << i->getStueck() << "m " << i->getRestStk() << "mR ("
         	<< i->getRestStk() << "R) " << i->getLieferdatum() << " "
         	<< cH_ArtikelBezeichnung(i->Id())->Bezeichnung() 
         	<< " " << int(i->getEntryStatus()) << '\n';
      }
   }

   cout << "2x abschreiben " << zeile.abschreiben(16000) << '\n';

   {  AuftragFull af(id);
      for (AuftragFull::const_iterator i=af.begin(); i!=af.end(); ++i)
      {  std::cout << i->getStueck() << "m " << i->getRestStk() << "mR ("
         	<< i->getRestStk() << "R) " << i->getLieferdatum() << " "
         	<< cH_ArtikelBezeichnung(i->Id())->Bezeichnung()
         	<< " " << int(i->getEntryStatus()) << '\n';
      }
   }
*/
   Auftrag(id).setStatusAuftrag_(CLOSED);
   return 0;
}
