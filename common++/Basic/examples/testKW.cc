// $Id: testKW.cc,v 1.1 2001/04/23 08:11:59 christof Exp $
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


#include <Aux/Datum.h>
#include <iostream>

ostream &operator<<(ostream &o,const Kalenderwoche &kw)
{  return o << kw.Woche() << '\'' << kw.Jahr();
}

int main()
{  Kalenderwoche kw(1,2000);
   Petig::Datum d(kw);
   cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';
   
   kw=Kalenderwoche(52,1999);
   d=Petig::Datum(kw);
   cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(2,1,2000);
   cout << d << " = " << d.KW() << '\n';

   d=Petig::Datum(3,1,2000);
   cout << d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(31,12,2000);
   cout << d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(1,1,2001);
   cout << d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(1,1,1997);
   cout << d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(31,12,1996);
   cout << d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(1,1,1998);
   cout << d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(31,12,1997);
   cout << d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(1,1,1999);
   cout << d << " = " << d.KW() << '\n';
   
   d=Petig::Datum(31,12,1998);
   cout << d << " = " << d.KW() << '\n';
   
   kw=Kalenderwoche(53,1998);
   d=Petig::Datum(kw);
   cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';
   
   kw=Kalenderwoche(13,1970);
   d=Petig::Datum(kw);
   cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';

   return 0;
}
