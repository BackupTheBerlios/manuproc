// $Id: testKW.cc,v 1.13 2005/12/21 07:18:47 christof Exp $
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


#include <Misc/Datum.h>
#include <iostream>

std::ostream &operator<<(std::ostream &o,const Kalenderwoche &kw)
{  return o << kw.Woche() << '\'' << kw.Jahr();
}

int main()
{  Kalenderwoche kw(1,2000);
   ManuProC::Datum d(kw);
   std::cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';
   
   kw=Kalenderwoche(52,1999);
   d=ManuProC::Datum(kw);
   std::cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(2,5,2004);
   std::cout << d << " = " << d.KW() << '\n';

   d=ManuProC::Datum(3,5,2004);
   std::cout << d << " = " << d.KW() << '\n';

   d=ManuProC::Datum(4,5,2004);
   std::cout << d << " = " << d.KW() << '\n';


   d=ManuProC::Datum(2,1,2000);
   std::cout << d << " = " << d.KW() << '\n';

   d=ManuProC::Datum(2,1,2000);
   std::cout << d << " = " << d.KW() << '\n';

   d=ManuProC::Datum(2,1,2000);
   std::cout << d << " = " << d.KW() << '\n';

   d=ManuProC::Datum(3,1,2000);
   std::cout << d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(31,12,2000);
   std::cout << d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(1,1,2001);
   std::cout << d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(1,1,1997);
   std::cout << d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(31,12,1996);
   std::cout << d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(1,1,1998);
   std::cout << d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(31,12,1997);
   std::cout << d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(1,1,1999);
   std::cout << d << " = " << d.KW() << '\n';
   
   d=ManuProC::Datum(31,12,1998);
   std::cout << d << " = " << d.KW() << '\n';
   
   kw=Kalenderwoche(53,1998);
   d=ManuProC::Datum(kw);
   std::cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';
   
   kw=Kalenderwoche(13,1970);
   d=ManuProC::Datum(kw);
   std::cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';

   d=ManuProC::Datum(1,1,2004);
   std::cout << d << " = " << d.KW() << '\n';

   d=ManuProC::Datum(3,5,2004);
   std::cout << d << " = " << d.KW() << '\n';

   kw=Kalenderwoche(18,2004);
   d=ManuProC::Datum(kw);
   std::cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';

   kw=Kalenderwoche(19,2004);
   d=ManuProC::Datum(kw);
   std::cout << "KW "<<kw << " = "<<d << " = " << d.KW() << '\n';

   d=ManuProC::Datum::today();
   std::cout << "today = "<<d << " = " << d.KW() << '\n';

   d=ManuProC::Datum(20,12,1800);
   try { std::cout << d << " = " << d.KW() << '\n';
   } catch (ManuProC::Datumsfehler &e) {}
   
   return 0;
}
