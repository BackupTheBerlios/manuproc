// $Id: ArtikelSumme.cc,v 1.2 2002/01/22 09:15:55 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#include <Artikel/ArtikelMengeSumme.h>
#include <iostream>
#include <Artikel/ArtikelBezeichnung.h>
#include <Aux/dbconnect.h>

std::ostream &operator<<(std::ostream &o, const ArtikelMenge &am)
{  return o << cH_ArtikelBezeichnung(am.Artikel())->Bezeichnung() << ":\t"
	<< am.Menge() << "\t| " << am.abgeleiteteMenge();
}

std::ostream &operator<<(std::ostream &o, const ArtikelMengeSumme &ams)
{  return o << ams.Menge() << "\t| " << ams.abgeleiteteMenge();
}

int main()
{  Petig::dbconnect();
   ArtikelMengeSumme ams;
   ArtikelMenge am=ArtikelMenge(218290,100);
   std::cout << ams << '\n';
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(218219,32);
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(218325,4,3000);
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(126712,2,4000);
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(38213,1,1.5);
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   return 0;
}
