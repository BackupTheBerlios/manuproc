// $Id: Instanzen.cc,v 1.6 2002/06/20 06:29:53 christof Exp $
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
#include <Lager/Lager_Vormerkungen.h>

int main(int argc, char *argv[])
{
 if(argc!=2) 
  {
    cout << argv[0]<<" legt Standardaufträge an, dieses Programm muß nur einmal \n"
            " bei der Datenbankinitilisierung laufen.\n"
            " Programmaufruf: "<<argv[0]<<" <DATNBANK>\n";
    exit(1);
  }
 cout <<"Initialisierung der Datenbank "<<argv[1]<<"\n";
 Petig::PrintUncaughtExceptions();
 try{
   Petig::Connection conn;
   conn.setDbase(argv[1]);
   Petig::dbconnect(conn); 
      
   std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
   for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
    {
     AuftragBase AB(*i,AuftragBase::ungeplante_id);
     AB.create_if_not_exists(OPEN,Kunde::default_id);
//     if(!(*i)->LagerInstanz()) continue;
     cout << "Lager Aufträge initialisieren: "<<(*i)->Name()<<' '
          <<(*i)->Id()<<' '<<(*i)->LagerInstanz()<<'\n';
     AuftragBase ABL(*i,AuftragBase::plan_auftrag_id);
     ABL.create_if_not_exists(OPEN,Kunde::default_id);
     cout << "Dispo Aufträge initialisieren: "<<(*i)->Name()<<' '<<(*i)->Id()<<'\n';
     AuftragBase ABD(*i,AuftragBase::dispo_auftrag_id);
     ABD.create_if_not_exists(OPEN,Kunde::default_id);
    }

 }catch(SQLerror &e){std::cout << e<<'\n';}
 return 0;
}



