// $Id: test_schaerangaben.cc,v 1.5 2004/06/18 07:43:51 christof Exp $
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
            << i->laenge << "m:";
         if ((i+1)!=x.end() && (i+1)->index==i->index) std::cout << "\n";
         oldindex=i->index;
      }
      std::cout << "\t" << i->faeden << " Fd. " 
          << cH_ArtikelBezeichnung(i->art)->Bezeichnung();
      if (i->wiederholungen>1) std::cout << " x" << i->wiederholungen;
      std::cout << '\n';
  }
  std::cout << "\n";
}

// müssten eigentlich kombiniert werden ...
void dump(const ArtikelBase &ab, const ArtikelBase &ab2, unsigned gaenge, unsigned laenge)
{  dump(ab,gaenge,laenge);
   dump(ab2,gaenge,laenge);
}

int main()
{  ManuProC::dbconnect();
   /* select distinct '  dump(ArtikelBase('||text(artikelid)||'),'
	||text(gaenge)||','||text(menge)||'); // '
	||artikelbezeichnung(artikelid) from schaertabelle 
	where schaerdatum ='2004-6-18'; */
   // sort -t/ -k2
   dump(ArtikelBase(225518),4,25000); // 1350/25/
   dump(ArtikelBase(225519),4,25000); // 1350/40/
   dump(ArtikelBase(123479),4,30000); // 1810/25/
   dump(ArtikelBase(219567),4,20000); // 1810/40/
   dump(ArtikelBase(219567),6,30000); // 1810/40/
   dump(ArtikelBase(126186),4,30000); // 1810/40/755
   dump(ArtikelBase(127789),6,20000); // 1820/25/
   dump(ArtikelBase(127786),4,20000); // 1820/40/
   dump(ArtikelBase(127786),6,20000); // 1820/40/
   dump(ArtikelBase(219591),ArtikelBase(219602),1,11000); // 2030/40/260+355
   dump(ArtikelBase(224996),2,16500); // 23103/40/9000
   dump(ArtikelBase(225532),4,12750); // 2391/40/
   dump(ArtikelBase(127798),8,4000); // 2501/25/260
   dump(ArtikelBase(209499),ArtikelBase(212353),1,12000); // 2508/25+40/325+215
   dump(ArtikelBase(220465),ArtikelBase(225167),1,6000); // 2727/25/265+420
   dump(ArtikelBase(220445),ArtikelBase(225166),1,6000); // 2727/40/265+420
   dump(ArtikelBase(220096),ArtikelBase(220100),1,6000); // 2730/40/310+360
   dump(ArtikelBase(225186),ArtikelBase(225187),1,10000); // 3038/25/430+435
   dump(ArtikelBase(225193),ArtikelBase(225194),1,10000); // 3038/40/430+435
   dump(ArtikelBase(225168),ArtikelBase(225169),1,10000); // 3042/40/300+310
   dump(ArtikelBase(123618),4,25000); // 375/40/
   dump(ArtikelBase(226121),2,14200); // 4409/40/20
   dump(ArtikelBase(226122),2,16700); // 4409/40/35
   dump(ArtikelBase(226120),4,28000); // 4532/40/05
   dump(ArtikelBase(225661),4,10000); // 4872/15/
   dump(ArtikelBase(225661),6,10000); // 4872/15/
   dump(ArtikelBase(210964),6,30000); // 4972/10/
   dump(ArtikelBase(210964),10,30000); // 4972/3/
   dump(ArtikelBase(210964),6,30000); // 4972/3/
   dump(ArtikelBase(210963),4,25000); // 5419/40/15
   dump(ArtikelBase(210970),4,28000); // 5434/40/15
//?   dump(ArtikelBase(219567),4,30000); // 1810/40/
   return 0;
}
