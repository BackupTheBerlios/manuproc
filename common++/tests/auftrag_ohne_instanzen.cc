// $Id: auftrag_ohne_instanzen.cc,v 1.1 2002/02/08 21:51:26 christof Exp $
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
#include <Aux/exception.h>
#include <Aux/dbconnect.h>
#include <Auftrag/AuftragsBaum.h>
//#include <Aux/ppsInstanz.h>
#include <unistd.h>

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

vector<AufEintrag> showTest(Auftrag id)
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


int main(int argc,char* argv[] )
{  
   Petig::PrintUncaughtExceptions();
   try{
   Petig::dbconnect();
   AuftragBase id;
   int x;
   while((x=getopt(argc,argv,"c"))!=-1) 
    {
     switch (x) {
      case 'c' :
       {
         Auftrag a=Auftrag(Auftrag::Anlegen(ppsInstanz::Kundenauftraege), 10000);
         a.setStatusAuftragBase(OPEN);
         id=a;
         std::cout << "Auftrag " << a.Id() << '\n';
      
         Petig::Datum date(1,6,2012);
         int znr;
//         znr=a.insertNewEntry(10000, date, 190449,OPEN,false);
         znr=a.insertNewEntry(1000, date, 219233,OPEN,true);
         showTest(id);
       }
      }
    }
//   Auftrag(id).setStatusAuftrag_(OPEN);
   }catch(SQLerror &e){std::cout << e<<'\n';}
   return 0;
}
