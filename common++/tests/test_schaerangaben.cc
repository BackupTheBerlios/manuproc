// $Id: test_schaerangaben.cc,v 1.1 2004/06/14 15:44:11 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
#include <Faeden/KettenGarn.h>
#include <Ketten/ArtikelGang.h>
#include <Misc/dbconnect.h>

void dump(const ArtikelBase &ab, unsigned gaenge, unsigned laenge)
{  std::vector<KettenGarn> x=KettenGarn::Load(ArtikelGang(gaenge,ab),laenge);
   unsigned oldindex=unsigned(-1);
   std::cout << gaenge << "x Artikel " << cH_ArtikelBezeichnung(ab)->Bezeichnung()
       << ' ' << laenge << "m\n";
   for (std::vector<KettenGarn>::const_iterator i=x.begin();i!=x.end();++i)
   {  if (i->index!=oldindex)
      {  std::cout << "   " << i->kettenzahl << " Ketten à " 
            << i->laenge << "m:\n";
         oldindex=i->index;
      }
      std::cout << "\t" << i->faeden << " Fd. " 
          << cH_ArtikelBezeichnung(i->art)->Bezeichnung()
          << " x" << i->wiederholungen << '\n';
  }
  std::cout << "\n";
}

int main()
{  ManuProC::dbconnect();
   dump(ArtikelBase(219567),4,20000);
   return 0;
}
