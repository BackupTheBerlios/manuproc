// $Id: artbez.cc,v 1.4 2002/05/09 12:46:01 christof Exp $
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
#include <Artikel/ArtikelBezeichnung.h>
#include <Aux/exception.h>
#include <Aux/dbconnect.h>
#include <Artikel/ArtikelBaum.h>
#include <Artikel/Einheiten.h>
#include <Aux/Ausgabe_neu.h>

int main()
{  Petig::PrintUncaughtExceptions();
   Petig::dbconnect();
   std::cout << cH_ArtikelBezeichnung(209931,10000)->Bezeichnung() << '\n';
   std::cout << cH_ArtikelBezeichnung(123479)->Bezeichnung() << " Breite "
   	<< cH_ArtikelBezeichnung(123479)->Breite() << "mm\n";
   std::cout << "-----------\n";
   ArtikelBaum AB(127754); // 1812/25
   Einheit e(AB);
   std::cout << cH_ArtikelBezeichnung(AB)->Bezeichnung() << '\n';
   for (ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {  std::cout << "  " << Formatiere_short(i->menge) 
   	<< (std::string)Einheit(i->rohartikel) << '/' << (std::string)e 
   	<< ' ' << cH_ArtikelBezeichnung(i->rohartikel)->Bezeichnung() << '\n';
   }
   std::cout << "     (3.35g Ac, 1.25g Ku)\n";
   Petig::dbdisconnect();
}
