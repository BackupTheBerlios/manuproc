// $Id: Instanzen.cc,v 1.12 2003/01/08 09:46:57 christof Exp $
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

#include <Misc/exception.h>
#include <Misc/dbconnect.h>
#include <Instanzen/ppsInstanz.h>
#include <Misc/exception.h>
#include <Auftrag/AuftragBase.h>
#include <Kunde/Kunde.h>

int main(int argc, char *argv[])
{
 if(argc!=2) 
  {
    std::cout << argv[0]<<" legt Standardauftr�ge an, dieses Programm mu� nur einmal \n"
            " bei der Datenbankinitilisierung laufen.\n"
            " Programmaufruf: "<<argv[0]<<" <DATENBANK>\n";
    exit(1);
  }
 std::cout <<"Initialisierung der Datenbank "<<argv[1]<<"\n";
 ManuProC::PrintUncaughtExceptions();
 try{
   ManuProC::Connection conn;
   conn.setDbase(argv[1]);
   ManuProC::dbconnect(conn); 
      
   std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
   for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
    {
     AuftragBase AB(*i,AuftragBase::ungeplante_id);
     AB.create_if_not_exists(OPEN,Kunde::default_id);
//     if(!(*i)->LagerInstanz()) continue;
     std::cout << "Lager Auftr�ge initialisieren: "<<(*i)->Name()<<' '
          <<(*i)->Id()<<' '<<(*i)->LagerInstanz()<<'\n';
     AuftragBase ABL(*i,AuftragBase::plan_auftrag_id);
     ABL.create_if_not_exists(OPEN,Kunde::default_id);
     std::cout << "Dispo Auftr�ge initialisieren: "<<(*i)->Name()<<' '<<(*i)->Id()<<'\n';
     AuftragBase ABD(*i,AuftragBase::dispo_auftrag_id);
     ABD.create_if_not_exists(OPEN,Kunde::default_id);
    }

 }catch(SQLerror &e){std::cout << e<<'\n';}
 return 0;
}

