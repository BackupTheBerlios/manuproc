// $Id: ArtikelSumme.cc,v 1.8 2004/07/26 16:21:43 christof Exp $
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
#include <Misc/dbconnect.h>

std::ostream &operator<<(std::ostream &o, const ArtikelMenge &am)
{  return o << cH_ArtikelBezeichnung(am.Artikel())->Bezeichnung() << ":\t"
	<< am.Menge() << "\t| " << am.abgeleiteteMenge();
}

std::ostream &operator<<(std::ostream &o, const ArtikelMengeSumme &ams)
{  return o << ams.Menge() << "\t| " << ams.abgeleiteteMenge();
}

int main()
{  ManuProC::dbconnect();
   ArtikelMengeSumme ams;
   ArtikelMenge am=ArtikelMenge(ArtikelBase(218290),100);
   std::cout << ams << '\n';
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(ArtikelBase(218219),32);
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(ArtikelBase(218325),4,3000);
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(ArtikelBase(126712),2,4000);
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(ArtikelBase(38213),1,1.5);
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   std::cout << "-----------------\n";
   ams=ArtikelMengeSumme();
   am=ArtikelMenge(ArtikelBase(210967),900000);
   std::cout << ams << '\n';
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(ArtikelBase(210967),100000);
   std::cout << ams << '\n';
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(ArtikelBase(221118),250);
   std::cout << ams << '\n';
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';
   am=ArtikelMenge(ArtikelBase(221120),1250);
   std::cout << ams << '\n';
   std::cout << '+' << am << '\n';
   ams.cumulate(am);
   std::cout << "=\t" << ams << '\n';

   std::cout << "\n----------\n";
   ams=ArtikelMengeSumme();
   ams.cumulate(ArtikelMenge(ArtikelBase(210967),20000000));
   std::cout << "=\t" << ams << '\n';
   ams.cumulate(ArtikelMenge(ArtikelBase(210967),20000000));
   ams.cumulate(ArtikelMenge(ArtikelBase(210967),20000000));
   std::cout << ams.Summe(Einheit(ManuProC::DynamicEnums::Einheiten::m)) << '\n';
   std::cout << (ams.Summe().begin()->second/100) << '\n';
   std::cout << fixedpoint<2,double,long long>(60000000) << '\n';
   std::cout << "=\t" << ams << '\n';
   return 0;
}
