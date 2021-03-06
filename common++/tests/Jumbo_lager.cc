// $Id: Jumbo_lager.cc,v 1.8 2003/01/08 09:46:58 christof Exp $
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
#include <Misc/exception.h>
#include <Misc/dbconnect.h>
#include <Instanzen/ppsInstanz.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Lager/Lager.h>

void showJumbo(const JumboRolle& J)
{
 std::cout << "Jumbo Rolle "<<J.Code()<<" befindet sich an Position "
    <<J.LagerPosition().IPosition()<<"\tArikel: "
    <<cH_ArtikelBezeichnung(J.Artikel())->Bezeichnung()
    << " L�nge: " <<J.Meter()<<"  Status: "<<J.getStatus()<<'\n';
}

int main()
{  ManuProC::PrintUncaughtExceptions();
   try{
   ManuProC::dbconnect();

   ManuProC::Datum datum(15,1,2011);
   Zeitpunkt_new zeit(datum);
   
   try{
    JumboRolle J(123);
    std::cout << "\nStartwert:\n";
    showJumbo(J);   
  
    }catch(SQLerror &e){std::cerr << "Fehler 1: "<<e<<'\n';}

   Lager L(ppsInstanz::Bandlager);

  try{
    JumboRolle J(123);
    LagerPlatz LP(ppsInstanz::Bandlager,418);
    J.setVerarbDatum_nodb(zeit);
    L.Jumbo_Einlagern(LP,J,Lager::Einlagern,"Malt");

    std::cout << "\nEingelagert:\n";
    J=JumboRolle(123);
    showJumbo(J);   
    }catch(SQLerror &e){std::cerr << "Fehler 2: "<<e<<'\n';}
/*
   try{
    JumboRolle J(123);
    L.Jumbo_Entnahme(J,Lager::Auslagern,"MalA");
    std::cout << "\nAusgelagert:\n";
    J=JumboRolle(123);
    showJumbo(J);   
    }catch(SQLerror &e){std::cerr << "Fehler 3: "<<e<<'\n';}
*/
   try{
    JumboRolle J(123);
     LagerPlatz LP2(ppsInstanz::Bandlager,41);
     L.Jumbo_Einlagern(LP2,J,Lager::Einlagern,"MalN");
     std::cout << "\nEingelagert an neue Position:\n";
     J=JumboRolle(123);
     showJumbo(J);   
     }catch(SQLerror &e){std::cerr << "Fehler 4: "<<e<<'\n';}

   }catch(SQLerror &e){std::cerr<<e<<'\n';}
   return 0;
}
