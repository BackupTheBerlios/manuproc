// $Id: artikelbaum.cc,v 1.1 2002/06/20 06:43:31 christof Exp $
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
#include <Artikel/ArtikelStamm.h>
#include <Artikel/Einheiten.h>
#include <Aux/Ausgabe_neu.h>

void zeige_hier(const ArtikelBase &art, std::string indent="")
{  ArtikelBaum AB(art);
   ArtikelStamm AS(art);
   Einheit e=AS.getEinheit();
   std::cout << indent << art.Id() << ": " 
   	<< cH_ArtikelBezeichnung(AB)->Bezeichnung() << ' '
   	<< AS.BestellenBei()->Name()
   	<< " (" << AS.Warengruppe() << ','
   	<< AS.InternWarengruppe() << ','
   	<< AS.defaultSchema()
   	<< ") " << (std::string)e << "\n";
   for (ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {  indent+=' ';
      std::cout << indent << Formatiere_short(i->menge) 
   	<< (std::string)Einheit(i->rohartikel) << '/' << (std::string)e 
   	<< '\n';
      indent+=' ';
      zeige_hier(i->rohartikel,indent);
   }
}

int main(int argc, char **argv)
{  if (argc!=2) { std::cerr << "USAGE: "<<argv[0]<< " <ID>\n"; return 2; }
   Petig::PrintUncaughtExceptions();
   try
   {  Petig::dbconnect();
      zeige_hier(atoi(argv[1]));
   }
   catch (SQLerror &e)
   {  std::cerr << e << '\n';
   }
   Petig::dbdisconnect();
}
