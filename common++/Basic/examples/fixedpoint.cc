// $Id: fixedpoint.cc,v 1.3 2001/05/10 16:33:00 christof Exp $
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
   cout << zehnhochminus<3>() << '\n';
   fixedpoint<> a(4.60),b(3.2),c(4.599),d("4.599");
   cout << "a="<<a << " b="<<b<< " c="<<c<< " d="<<d<< '\n';
   cout << "a/7=" << a/7 << " b=a/7=" << (b=a/7) << '\n';
   cout << "float(b)="<< float(b) << '\n';
   cout << "a+b=" << a+b << '\n';
   
   // Formatiere
   cout << Formatiere(12345) << "\n";
   cout << Formatiere(12345,4) << "\n";
   cout << Formatiere(12345,2) << "\n";
   cout << Formatiere(12345678901234ULL,6) << "\n";
   cout << Formatiere(fixedpoint<2>(12.567)) << '\n';
   cout << FormatiereTeX_short(fixedpoint<5>(12.4)) << '\n';
   // compile time assertion
   cout << Formatiere(12.45) << '\n';
   return 0;
}
