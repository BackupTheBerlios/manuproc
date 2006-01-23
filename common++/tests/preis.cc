// $Id: preis.cc,v 1.12 2006/01/23 11:25:26 christof Exp $
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
#include <Misc/exception.h>
#include <Misc/dbconnect.h>

int main()
{  ManuProC::PrintUncaughtExceptions();
   ManuProC::dbconnect();
#if 1
   Preis p(1.00,WaehrungID::DM,100);
   std::cout << "1DM/100 "<<p<<'\n';
   Preis b(2.0,WaehrungID::DM,100);
   std::cout << "2DM/100 " << b << '\n';
   std::cout << "Summe "<<p+b<< ", " << b+p<< '\n';
   std::cout << "-------------------\n";
   std::cout << "2DM = "<< b.Wert(WaehrungID::EUR,1) << "Euro\n";
   std::cout << "-------------------\n";
   Preis c(3.0,WaehrungID::EUR,100);
   std::cout << "3Euro/100 "<<c<<'\n';
   std::cout << "1DM + 3Euro "<<p+c<< ", " << c+p<< '\n';
   std::cout << "-------------------\n";
   Preis d(4.0,WaehrungID::EUR,25);
   std::cout << d << '\n';
   std::cout << "+ 1DM/100 " << p+d << ", " << d+p << '\n';
   std::cout << "--------------------\n";
   Preis e(1.0,WaehrungID::DM,7);
   Preis f(.1,WaehrungID::EUR,100);
   std::cout << e << " + " << f << " = " << e+f << ", " << f+e << '\n';
   std::cout << "-------------------\n";
   Preis g(5.0,WaehrungID::DM,4);
   std::cout << g << " -> 20Stk. = " << g.Gesamtpreis(cP_Waehrung(WaehrungID::DM),20,1.0) << '\n';
   std::cout << "-------------------\n";
   Preis g2(7.10,WaehrungID::DM,2);
   std::cout << g2 << " -> 80Stk. = " << g2.Gesamtpreis(WaehrungID::DM,80,0.0,0.0) << '\n';
   std::cout << "-------------------\n";
   std::cout << "!" << g2 << " = " << !g2 << '\n';
   Preis g3;
   std::cout << "!" << g3 << " = " << !g3 << '\n';
#endif   
   std::cout << "-------------------\n";
   Preis g4(2.96,WaehrungID::EUR,100);
   std::cout << g4.Gesamtpreis(cP_Waehrung(WaehrungID::EUR),1000,2276.092) << '\n';
   Preis g5(1.00,cP_Waehrung(Waehrung::default_id),100);
   std::cout << g5.Gesamtpreis(cP_Waehrung(WaehrungID::EUR),1,0.0) << '\n';
}
