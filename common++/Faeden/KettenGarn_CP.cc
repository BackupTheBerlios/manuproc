/* $Id: KettenGarn_CP.cc,v 1.5 2004/02/26 17:37:51 christof Exp $ */
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

#include <Faeden/KettenGarn.h>
#include <Faeden/Webangaben.hh>
#include <Faeden/Faden.hh>
#include <Ketten/ArtikelGang.h>

#warning diese API unterstützt keine Kombinationen!!!

struct KG_compare
{  bool operator()(const KettenGarn_CP &a,const KettenGarn_CP &b)
   {  return a.index<b.index || (a.index==b.index
   	  && (a.zeile<b.zeile));
   }
};

std::vector<KettenGarn_CP> KettenGarn_CP::Load(ArtikelGang const &ag, unsigned laenge)
{  Fadenliste fdl;
   std::vector<KettenGarn_CP> result;
   try {
   Webangaben wa(ag.art);
   wa.Load();
   fdl.Load(wa);
   }
   catch (SQLerror &e)
   {  return result;
   }

   // ermitteln!
   unsigned new_scheibe=20;
   for (Fadenliste::const_iterator i=fdl.begin(); i!=fdl.end(); ++i)
   {  KettenGarn_CP x;
      if (i->kettscheibe<1) continue;
      x.index=i->kettscheibe;
      x.zeile=i->zeilennummer;
      x.kettenzahl=ag.gaenge;
      x.faeden=i->anzahl;
      x.art=i->material;
      x.laenge=laenge;
      // 2do: wiederholungen 
      x.wiederholungen=1;
      while (i->max_fadenzahl && x.faeden>i->max_fadenzahl)
      {  unsigned tmp=x.faeden-i->max_fadenzahl;
         x.faeden=i->max_fadenzahl;
         x.index=new_scheibe++;
         result.push_back(x);
         x.faeden=tmp;
      }
      x.index=i->kettscheibe;
      result.push_back(x);
   }
   // sortieren?
   std::sort(result.begin(),result.end(),KG_compare());
   // zusammenfassen?
   for (std::vector<KettenGarn_CP>::iterator i=result.begin();i!=result.end();)
   {  std::vector<KettenGarn_CP>::iterator b=i+1;
      if (b==result.end()) break;
      if (i->index==b->index && i->art==b->art && i->kettenzahl==b->kettenzahl
      		&& i->laenge==b->laenge && i->wiederholungen==b->wiederholungen)
      {  i->faeden+=b->faeden;
         i=result.erase(b);
      }
      else ++i;
   }
   return result;
}
