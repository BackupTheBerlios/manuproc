// $Id: checkInstanzen.cc,v 1.1 2002/11/22 15:39:40 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2002 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <Aux/exception.h>
#include <Aux/dbconnect.h>
#include <Instanzen/ppsInstanz.h>
#include <Aux/exception.h>
#include <Auftrag/AuftragBase.h>
#include <Kunde/Kunde.h>

void ausgabe(const std::string &s1, const std::string &s2, bool b1,bool b2)
{
  if(!b1) return;
  std::cout << "\t"<<s1<<" => "<<s2<<"\t...\t";
  if(b1 &&  b2) std::cout << "OK";
  if(b1 && !b2) std::cout << "N E I N";
  std::cout << "\n";
}


void check(cH_ppsInstanz I)
{
 std::cout << I <<'\n';
 
 ausgabe("AutomatischEinlagern","LagerInstanz",I->AutomatischEinlagern(),I->LagerInstanz());
 ausgabe("Lieferschein","ProduziertSelbst",I->Lieferschein(),I->ProduziertSelbst());
 ausgabe("ExterneBestellung","Kein Kundenauftrag",I->ExterneBestellung(),(I->Id()!=ppsInstanzID::Kundenauftraege));
 ausgabe("PlanungsInstanz","Nicht ProduziertSelbst",I->PlanungsInstanz(),!I->ProduziertSelbst());
 ausgabe("PlanungsInstanz","Nicht Lieferschein",I->PlanungsInstanz(),!I->Lieferschein());
}


int main(int argc, char *argv[])
{
 if(argc!=2) 
  {
    std::cout << argv[0]<<" testet, ob die Instanzen die Vorraussetzungen erf�llen \n"
            " Programmaufruf: "<<argv[0]<<" <DATENBANK>\n";
    exit(1);
  }
 ManuProC::PrintUncaughtExceptions();
 try{
   ManuProC::Connection conn;
   conn.setDbase(argv[1]);
   ManuProC::dbconnect(conn); 
      
   std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
   for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
    {
      check(*i);
    }

 }catch(SQLerror &e){std::cout << e<<'\n';}
 return 0;
}



