// $Id: preis.cc,v 1.5 2002/05/06 13:41:23 christof Exp $
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
#include <Artikel/Preis.h>
#include <Aux/exception.h>
#include <Aux/dbconnect.h>

int main()
{  Petig::PrintUncaughtExceptions();
   Petig::dbconnect();
   Preis p(1.00,Waehrung::DM,100);
   std::cout << "1DM/100 "<<p<<'\n';
   Preis b(2.0,Waehrung::DM,100);
   std::cout << "2DM/100 " << b << '\n';
   std::cout << "Summe "<<p+b<< ", " << b+p<< '\n';
   std::cout << "-------------------\n";
   std::cout << "2DM = "<< b.Wert(Waehrung::EUR) << "Euro\n";
   std::cout << "-------------------\n";
   Preis c(3.0,Waehrung::EUR,100);
   std::cout << "3Euro/100 "<<c<<'\n';
   std::cout << "1DM + 3Euro "<<p+c<< ", " << c+p<< '\n';
   std::cout << "-------------------\n";
   Preis d(4.0,Waehrung::EUR,25);
   std::cout << d << '\n';
   std::cout << "+ 1DM/100 " << p+d << ", " << d+p << '\n';
   std::cout << "--------------------\n";
   Preis e(1.0,Waehrung::DM,7);
   Preis f(.1,Waehrung::EUR,100);
   std::cout << e << " + " << f << " = " << e+f << ", " << f+e << '\n';
   std::cout << "-------------------\n";
   Preis g(5.0,Waehrung::DM,4);
   std::cout << g << " -> 20Stk. = " << g.Gesamtpreis(20,1.0) << '\n';
   std::cout << "-------------------\n";
   Preis g2(7.10,Waehrung::DM,2);
   std::cout << g2 << " -> 80Stk. = " << g2.Gesamtpreis(Waehrung::DM,80,0.0,0.0) << '\n';
   std::cout << "-------------------\n";
   std::cout << "!" << g2 << " = " << !g2 << '\n';
   Preis g3;
   std::cout << "!" << g3 << " = " << !g3 << '\n';
}
