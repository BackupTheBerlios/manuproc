// $Id: fixedpoint.cc,v 1.6 2001/10/01 12:55:40 christof Exp $
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
#include <Aux/fixedpoint.h>
#include <Aux/Ausgabe_neu.h>
#include <Aux/exception.h>

int main()
{  
   std::cout << zehnhochminus<3>() << '\n';
   fixedpoint<> a(4.60),b(3.2),c(4.599),d("4.599");
   std::cout << "a="<<a << " b="<<b<< " c="<<c<< " d="<<d<< '\n';
   std::cout << "a/7=" << a/7 << " b=a/7=" << (b=a/7) << '\n';
   std::cout << "float(b)="<< float(b) << '\n';
   std::cout << "a+b=" << a+b << '\n';
   
   // Formatiere
   std::cout << Formatiere(12345) << "\n";
   std::cout << Formatiere(12345,4) << "\n";
   std::cout << Formatiere(12345,2) << "\n";
   std::cout << Formatiere(12345678901234ULL,6) << "\n";
   std::cout << Formatiere(fixedpoint<2>(12.567)) << '\n';
   std::cout << FormatiereTeX_short(fixedpoint<5>(12.4)) << '\n';
   // compile time assertion
   std::cout << Formatiere(12.45) << '\n';
   
   std::cout << (fixedpoint<2>(1)==fixedpoint<2>(1.00)?"1==1.00":"1!=1.00") << '\n';
   
   // this is an overflow!!!
   std::cout << (fixedpoint<5>(15000.00015)*10l) << '\n';
   std::cout << (fixedpoint<5,double,long long>(15000.00015)*10ll) << '\n';
   std::cout << (fixedpoint<5,double,long long>(15000.00015)*=10) << '\n';
   
   return 0;
}
