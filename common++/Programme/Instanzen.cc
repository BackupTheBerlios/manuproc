// $Id: Instanzen.cc,v 1.5 2002/05/09 12:46:01 christof Exp $
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
#include <Aux/ppsInstanz.h>
#include <Aux/exception.h>
#include <Auftrag/AuftragBase.h>
#include <Kunde/Kunde.h>
#include <Lager/Lager_Vormerkungen.h>

int main()
{
 Petig::PrintUncaughtExceptions();
 try{
   Petig::dbconnect();
   std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
   for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
    {
     AuftragBase AB(*i,0);
     AB.create_if_not_exists(OPEN,Kunde::default_id);
//     if(!(*i)->LagerInstanz()) continue;
     cout << "Lager Aufträge initialisieren: "<<(*i)->Name()<<' '
          <<(*i)->Id()<<' '<<(*i)->LagerInstanz()<<'\n';
     AuftragBase ABL(*i,Lager_Vormerkungen::LagerAuftragsId);
     ABL.create_if_not_exists(OPEN,Kunde::default_id);
    }

 }catch(SQLerror &e){std::cout << e<<'\n';}
 return 0;
}



