// $Id: preis.cc,v 1.1 2001/04/23 08:11:59 christof Exp $
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
   cout << "1DM/100m "<<p<<'\n';
   Preis b(2.0,Waehrung::DM,100);
   cout << "2DM/100m "<<b<<'\n';
   cout << "+ "<<p+b<< ", " << b+p<< '\n';
   cout << "2DM -> "<< b.Wert_i(Waehrung::EUR) << '\n';
   Preis c(3.0,Waehrung::EUR,100);
   cout << "3Euro/100m "<<c<<'\n';
   cout << "1DM+3Euro "<<p+c<< ", " << c+p<< '\n';
   Preis d(4.0,Waehrung::EUR,25);
   cout << d << '\n';
   cout << "+ " << p+d << ", " << d+p << '\n';
   cout << "--------------------\n";
   Preis e(1.0,Waehrung::DM,7);
   Preis f(.1,Waehrung::EUR,100);
   cout << e << '+'<< f<<"=" << e+f<<'\n';
}
