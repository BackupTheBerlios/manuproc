// $Id: instanz_auftrag.cc,v 1.13 2003/01/08 09:46:58 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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
#include <Misc/exception.h>
#include <Misc/dbconnect.h>
#include <Auftrag/AuftragsBaum.h>
//#include <Instanzen/ppsInstanz.h>

void showChildren(AufEintragBase AEB)
{
  AuftragsBaum AB(AEB,true);
  std::cout << "Self: "<<AEB.Instanz()->Name()<<' '<<AEB.Id()<<' '<<AEB.ZNr()<<' '<<'\n';
  for (AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {
     std::cout <<i->AEB2.Instanz()->Name()<<' '<<i->AEB2.Id()<<' '<<i->AEB2.ZNr()
         <<' '<<cH_ArtikelBezeichnung(i->AB)->Bezeichnung()
         <<'\t'<<i->menge<<' '
         <<'\t'<<cH_ArtikelBezeichnung(i->AB)->Bezeichnung()<<' '
         <<i->AEB2.Instanz()->Name()<<'\n';
   }
}

std::vector<AufEintrag> showTest(Auftrag id)
{
   AuftragFull af(id);
   std::vector<AufEintrag> VAEB;
   for (AuftragFull::const_iterator i=af.begin(); i!=af.end(); ++i)
    {  
      showChildren(*i);
      VAEB.push_back(*i);
    }
 return VAEB;
}


int main()
{  ManuProC::PrintUncaughtExceptions();
   try{
   ManuProC::dbconnect();
   AuftragBase id;
   {  Auftrag a=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege), 10000);
      a.setStatusAuftragBase(OPEN);
      id=a;
      std::cout << "Auftrag " << a.Id() << '\n';
   
      ManuProC::Datum date(1,6,2012);
      int znr;
      znr=a.insertNewEntry(10000, date, 190449,OPEN,true);
      znr=a.insertNewEntry(10000, date, 190449,OPEN,true);
//      znr=a.insertNewEntry(2000, date, 218849,UNCOMMITED,true);
   }
   std::vector<AufEintrag> VAEB=showTest(id);
/*
   static const int TESTZEILE = 1;
   int znr=0;
//   ManuProC::Datum newdate(3,3,2100);
   for(std::vector<AufEintrag>::iterator i=VAEB.begin();i!=VAEB.end();++i)
    {
      if(++znr==TESTZEILE)
       {
//         std::cout << "TEST für AEB: "<<i->Instanz()<<' '<<i->Id()<<' '<<i->ZNr()<<' '<<i->getStueck()<<'\n';
//           i->updateStk(10000,true); // Neue Menge 
//           i->updateLieferdatum(newdate); // Neues Lieferdatum
//           i->setStatus(OPEN); // Neuer Status
//           i->split(500,newdate); // Auftrag spliten            
       }
    }

   std::cout << "\n Nach dem Test:\n\n";
   showTest(id);
*/
   Auftrag(id).setStatusAuftrag_(OPEN);
   }catch(SQLerror &e){std::cout << e<<'\n';}
   return 0;
}
