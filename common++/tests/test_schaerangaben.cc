// $Id: test_schaerangaben.cc,v 1.14 2004/07/06 14:59:23 christof Exp $
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

void dump(const std::vector<ArtikelGang> &vag, unsigned laenge)
{  std::vector<Kettscheibe> x=Kettscheibe::Load(vag,laenge);
//   unsigned oldindex=unsigned(-1);
   std::vector<ArtikelGang> oldartikel;
   for (std::vector<Kettscheibe>::const_iterator i=x.begin();i!=x.end();++i)
   {  if (i->artikel!=oldartikel)
      {  if (!oldartikel.empty()) std::cout << "\n";
         for (std::vector<ArtikelGang>::const_iterator j=i->artikel.begin();j!=i->artikel.end();++j)
            std::cout << j->gaenge << "x Artikel " 
               << cH_ArtikelBezeichnung(j->art)->Bezeichnung()
               << ' ';
         std::cout << laenge << "m\n";
         oldartikel=i->artikel;
      }
      std::cout << "   " << i->kettenzahl << " Ketten à " 
            << i->laenge << "m:";
      if (i->faeden.size()!=1) std::cout << "\n";
      for (std::vector<KS_Garn>::const_iterator j=i->faeden.begin();j!=i->faeden.end();++j)
      {  std::cout << "\t" << j->faeden << " Fd. " 
              << cH_ArtikelBezeichnung(j->material)->Bezeichnung();
         if (j->wiederholungen>1) std::cout << " x" << j->wiederholungen;
         std::cout << '\n';
      }
  }
  std::cout << "\n";
}

void dump(const ArtikelBase &ab, unsigned gaenge, unsigned laenge)
{  std::vector<ArtikelGang> vag;
   vag.push_back(ArtikelGang(gaenge,ab));
   dump(vag,laenge);
}

// müssten eigentlich kombiniert werden ...
void dump(const ArtikelBase &ab, const ArtikelBase &ab2, unsigned gaenge, unsigned laenge)
#if 1
{  std::vector<ArtikelGang> vag;
   vag.push_back(ArtikelGang(gaenge,ab));
   vag.push_back(ArtikelGang(gaenge,ab2));
   dump(vag,laenge);
}
#else
{  dump(ab,gaenge,laenge);
   dump(ab2,gaenge,laenge);
}
#endif

int main()
{  ManuProC::dbconnect();
   /* select distinct '  dump(ArtikelBase('||text(artikelid)||'),'
	||text(gaenge)||','||text(menge)||'); // '
	||artikelbezeichnung(artikelid) from schaertabelle 
	where schaerdatum ='2004-6-18'; */
   // sort -t/ -k2
   dump(ArtikelBase(225879),4,25000); // 1350/15/
   dump(ArtikelBase(225518),4,25000); // 1350/25/
   dump(ArtikelBase(225519),4,25000); // 1350/40/
   dump(ArtikelBase(204806),4,25000); // 1524/40/815
   dump(ArtikelBase(123479),4,30000); // 1810/25/
   dump(ArtikelBase(125649),6,30000); // 1810/25/755
   dump(ArtikelBase(219567),4,20000); // 1810/40/
   dump(ArtikelBase(219567),6,30000); // 1810/40/
   dump(ArtikelBase(126186),4,30000); // 1810/40/755
   dump(ArtikelBase(182851),4,20000); // 1810/60/
   dump(ArtikelBase(127755),6,40000); // 1810/8/
   dump(ArtikelBase(127755),8,40000); // 1810/8/
   dump(ArtikelBase(127754),2,30000); // 1812/25/
   dump(ArtikelBase(127754),4,30000); // 1812/25/
   dump(ArtikelBase(212504),4,30000); // 1812/40/
   dump(ArtikelBase(127789),6,20000); // 1820/25/
   dump(ArtikelBase(127786),4,20000); // 1820/40/
   dump(ArtikelBase(127786),6,20000); // 1820/40/
   dump(ArtikelBase(219591),ArtikelBase(219602),1,11000); // 2030/40/260+355
   // 22528/40/4641+4722 sollte kombinieren!
   dump(ArtikelBase(211974),ArtikelBase(222721),1,17000); // 22528/40/4641+4722
   dump(ArtikelBase(222722),2,17000); // 22528/40/5144
   dump(ArtikelBase(219020),ArtikelBase(225951),2,20000); // 22556/40/1110+2010
   dump(ArtikelBase(224996),2,16500); // 23103/40/9000
   dump(ArtikelBase(225534),2,10000); // 23251/25/
   dump(ArtikelBase(225533),2,25000); // 23251/40/
   dump(ArtikelBase(225543),2,10000); // 23252/40/
   dump(ArtikelBase(225532),4,12750); // 2391/40/
   dump(ArtikelBase(127798),8,4000); // 2501/25/260
   dump(ArtikelBase(209499),ArtikelBase(212353),1,12000); // 2508/25+40/325+215
   dump(ArtikelBase(225032),ArtikelBase(225029),1,4500); // 2522/25/260+430
   dump(ArtikelBase(220465),ArtikelBase(225167),1,6000); // 2727/25/265+420
   dump(ArtikelBase(220445),ArtikelBase(225166),1,6000); // 2727/40/265+420
   dump(ArtikelBase(220096),ArtikelBase(220100),1,6000); // 2730/40/310+360
   dump(ArtikelBase(225041),ArtikelBase(225036),1,10000); // 2736/40/300+515
   dump(ArtikelBase(225017),1,10000); // 2737/40/435
   dump(ArtikelBase(225021),ArtikelBase(225025),1,10000); // 2737/55/305+430
   // 3038/25+40/430+435 sollte kombinieren !!!
   dump(ArtikelBase(225187),2,10000); // 3038/25/435
   dump(ArtikelBase(225117),ArtikelBase(225118),2,8500); // 3038/25/300+360
   dump(ArtikelBase(225186),ArtikelBase(225187),1,10000); // 3038/25/430+435
   dump(ArtikelBase(225193),ArtikelBase(225194),1,10000); // 3038/40/430+435
   dump(ArtikelBase(225975),ArtikelBase(225974),1,10000); // 3039/25/300+420
   dump(ArtikelBase(224768),ArtikelBase(224774),1,6500); // 3040/35/360+430
   dump(ArtikelBase(224969),ArtikelBase(224967),1,6500); // 3040/75/360+420
   dump(ArtikelBase(224479),ArtikelBase(224481),3,25000); // 3041/25+40/
   dump(ArtikelBase(225168),ArtikelBase(225169),1,10000); // 3042/40/300+310
   dump(ArtikelBase(123618),4,25000); // 375/40/
   dump(ArtikelBase(226121),2,14200); // 4409/40/20
   dump(ArtikelBase(226122),2,16700); // 4409/40/35
   dump(ArtikelBase(226120),4,28000); // 4532/40/05
   dump(ArtikelBase(225661),4,10000); // 4872/15/
   dump(ArtikelBase(225661),6,10000); // 4872/15/
   dump(ArtikelBase(210966),6,30000); // 4972/10/
   dump(ArtikelBase(210968),6,22000); // 4972/25/
   dump(ArtikelBase(210964),10,30000); // 4972/3/
   dump(ArtikelBase(210964),6,30000); // 4972/3/
   dump(ArtikelBase(210969),2,20000); // 4972/40/
   dump(ArtikelBase(210969),4,20000); // 4972/40/
   dump(ArtikelBase(210965),10,30000); // 4972/6/
   dump(ArtikelBase(210963),4,25000); // 5419/40/15
   dump(ArtikelBase(210970),4,28000); // 5434/40/15
   dump(ArtikelBase(225881),4,22000); // 7278/25/24
   dump(ArtikelBase(225880),2,23000); // 7278/40/24
   dump(ArtikelBase(225880),4,23000); // 7278/40/24
   return 0;
}
