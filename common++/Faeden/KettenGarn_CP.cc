/* $Id: KettenGarn_CP.cc,v 1.3 2004/02/26 10:43:28 christof Exp $ */
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
#include <Faeden/Faden.hh>
#include <Ketten/ArtikelGang.h>

#warning diese API unterstützt keine Kombinationen!!!

std::vector<KettenGarn_CP> KettenGarn_CP::Load(ArtikelGang const &ag, unsigned laenge)
{  Fadenliste fdl;
   std::vector<KettenGarn_CP> result;
   try {
   fdl.Load(ag.art,Bindung::GlobaleListe());
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
   // zusammenfassen?
   return result;
}
