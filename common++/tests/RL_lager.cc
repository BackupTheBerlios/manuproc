// $Id: RL_lager.cc,v 1.6 2002/06/20 06:29:53 christof Exp $
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
#include <Aux/exception.h>
#include <Aux/dbconnect.h>
#include <Instanzen/ppsInstanz.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Lager/Lager.hh>

void showLager(const Lager& L,const LagerPlatz& P)
{
 Lager::st_rohlager I=L.RL_Inhalt(P);
 std::cout << "An Position "<<P.SPosition()<<" befinden sich "
    << I.kartons<<" Kartons mit je "<<I.kg_pro_karton <<"kg und "
    << I.reste<<" ResteKartons mit insgesamt "<<I.reste_kg
    <<"kg von Artikel: ->"<<cH_ArtikelBezeichnung(I.artikel)->Bezeichnung()<<"<-\n";
}

int main()
{  Petig::PrintUncaughtExceptions();
   Petig::dbconnect();

   Petig::Datum datum(15,1,2011);
   ArtikelBase artikel(38223);
//   ArtikelBase artikel2(218267);
   
   std::string nachricht;
   LagerPlatz LP(ppsInstanz::INST_ROHLAGER,"08F2");
   Lager L(ppsInstanz::INST_ROHLAGER);
/*
   // Lagerplatz löschen:
   Lager::st_rohlager rohlager(LP,0,0,0,0,artikel,datum);
//   L.RL_Entnahme(rohlager,nachricht,true);
//   std::cout << nachricht<<'\n';
*/
   showLager (L,LP);


   // Etwas rein
   Lager::st_rohlager rohlager2(LP,100,100,0,0,artikel,datum);
   L.RL_Einlagern(LP,rohlager2,nachricht);
   std::cout << nachricht<<'\n';
   showLager(L,LP);

/*
   // Etwas dazu 
   Lager::st_rohlager rohlager3(LP,10,20,1,2,artikel,datum);
   L.RL_Einlagern(rohlager3,nachricht);
   std::cout << nachricht<<'\n';
   showLager(L,LP);
*/
/*
   // Etwas herausholen 
   Lager::st_rohlager rohlager4(LP,1,60,0,0,artikel,datum);
   L.RL_Entnahme(rohlager4,nachricht);
   std::cout << nachricht<<'\n';
   showLager(L,LP);
*/
/*
   // Noch etwas anderes rein:
   Lager::st_rohlager rohlager5(LP,100,20,5,22,artikel2,datum);
   L.RL_Einlagern(rohlager5,nachricht);
   std::cout << nachricht<<'\n';
   showLager(L,LP);

   Lager::st_rohlager rohlager6(LP,100,20,5,22,artikel2,datum);
   L.RL_Einlagern(rohlager6,nachricht,true);
   std::cout << nachricht<<'\n';
   showLager(L,LP);

*/
   return 0;
}
