/* $Id: KettenGarn_CP.cc,v 1.23 2004/07/06 15:42:03 christof Exp $ */
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
#include <Misc/TraceNV.h>
#include <Misc/algorithm.h>

static const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channel_e("DEBUG_KETTEN",trace_channel);
                  
namespace {
struct map_index_t
{	std::vector<ArtikelGang> agv;
        unsigned scheibe;
        
        map_index_t(const ArtikelGang &a, unsigned s) : scheibe(s) 
        {  agv.push_back(a);
        }
        bool operator==(const map_index_t &b) const
        {  return scheibe==b.scheibe && agv==b.agv; }
        bool operator<(const map_index_t &b) const
        {  if (agv.size()!=b.agv.size()) return agv.size()<b.agv.size();
           if (agv!=b.agv) return agv<b.agv;
           return scheibe<b.scheibe;
        }
        const map_index_t &operator++()
        {  ++scheibe;
           return *this;
        }
};
}

static void GleicheFaedenZusammenfassen(Kettscheibe &x)
{  for (std::vector<KS_Garn>::iterator j=x.faeden.begin();j!=x.faeden.end();)
      {  std::vector<KS_Garn>::iterator b=j+1;
         if (b==x.faeden.end()) break;
         if (j->material==b->material && j->wiederholungen==b->wiederholungen)
         {  j->faeden+=b->faeden;
            j=x.faeden.erase(b);
         }
         else ++j;
      }
}

std::vector<Kettscheibe> Kettscheibe::Load(const std::vector<ArtikelGang> &ag, unsigned laenge)
{  typedef std::map<map_index_t,Kettscheibe> map_t;
   map_t intermed;
   try {
   
  for (std::vector<ArtikelGang>::const_iterator gang=ag.begin();gang!=ag.end();++gang)
  {
   Webangaben wa(gang->art);
   wa.Load();
   Fadenliste fdl;
   fdl.Load(wa);

   for (Fadenliste::const_kettiterator i=fdl.kettbegin(); i!=fdl.kettend(); ++i)
   {  if (!i->nr) continue;
      Kettscheibe &x=intermed[map_index_t(*gang,i->nr)];
      x.index=i->nr;
      x.laenge=laenge;
      if (i->verlaengern) 
      {  if (x.laenge<10000) x.laenge+=100; // 3040/35/420 v. 15.1.04
         else if (x.laenge<=12000) x.laenge+=200;
         else x.laenge+=500;
      }
      x.max_fadenzahl=i->max_fadenzahl;
      if (gang->gaenge==i->ausn_gaenge && i->ausn_maxfd) 
         x.max_fadenzahl=i->ausn_maxfd;
      else if (gang->gaenge==i->ausn_gaenge2 && i->ausn_maxfd2) 
         x.max_fadenzahl=i->ausn_maxfd2;
      x.kettenzahl=gang->gaenge;
      x.artikel.push_back(*gang);
   }
   for (Fadenliste::const_iterator i=fdl.begin(); i!=fdl.end(); ++i)
   {  if (i->kettscheibe<1) continue;
      Kettscheibe &y=intermed[map_index_t(*gang,i->kettscheibe)];
      if (!y.index)
      {  y.index=i->kettscheibe;
         y.laenge=laenge;
         y.kettenzahl=gang->gaenge;
         y.artikel.push_back(*gang);
      }
      assert(y.index==i->kettscheibe);
      KS_Garn x;
      x.zeile=i->zeilennummer;
      x.faeden=i->anzahl;
      if (gang->gaenge==i->ausn_gaenge && i->ausn_faeden) 
      	 x.faeden=i->ausn_faeden;
      x.material=i->material;
      // Einzelwiederholungen komprimieren !!!
      // look for single line repeats
      for (Fadenliste::const_repiterator r=fdl.repbegin();r!=fdl.repend();++r)
      {  if (r->start==r->end && r->start==i->zeilennummer)
            x.faeden*=r->anzahl;
      }
      y.faeden.push_back(x);
      y.fadenzahl+=x.faeden;
   }

   // Wiederholungen verteilen und verrechnen
   for (Fadenliste::const_repiterator r=fdl.repbegin();r!=fdl.repend();++r)
   {  if (r->start==r->end) continue;
      for (map_t::iterator i=intermed.begin();i!=intermed.end();++i)
      {  Kettscheibe &x=i->second;
         if (x.artikel.size()!=1 || !(*(x.artikel.begin())==*gang))
            continue;
         for (std::vector<KS_Garn>::iterator j=x.faeden.begin();j!=x.faeden.end();++j)
         {  if (r->start<=j->zeile && j->zeile<=r->end)
            {  x.fadenzahl+=(r->anzahl-1)*j->wiederholungen*j->faeden;
               j->wiederholungen*=r->anzahl;
               x.wiederholungen.push_back(*r);
            }
         }
      }
   }
  }
  
   // gleiche aufeinanderfolgende Fäden zusammenfassen
   for (map_t::iterator i=intermed.begin();i!=intermed.end();++i)
      GleicheFaedenZusammenfassen(i->second);
   // Polyesterketten etwas weniger verlängern
   for (map_t::iterator i=intermed.begin();i!=intermed.end();++i)
   {  Kettscheibe &x=i->second;
      if (x.laenge==laenge+500 && x.faeden.size()==1
          && cH_ArtikelBezeichnung(x.faeden.begin()->material)
              ->Komponente(0).substr(0,10)=="Poly verst")
         x.laenge-=300;
   }
reloop:
   // kombinieren
   for (map_t::iterator i=intermed.begin();i!=intermed.end();++i)
   {  Kettscheibe &x=i->second;
      // Einschränkung auf x.faeden.size()==1 später aufheben
      if (x.max_fadenzahl 
            // es passt mindestens 2x ...
          && (2*x.fadenzahl<=x.max_fadenzahl 
            // ... oder es geht glatt auf
            || (x.fadenzahl<x.max_fadenzahl && !((x.fadenzahl*x.kettenzahl)%x.max_fadenzahl))
            )
          && x.faeden.size()==1)
      {  unsigned neue_kettenzahl=(x.fadenzahl*x.kettenzahl+x.max_fadenzahl-1)
                    /x.max_fadenzahl;
         if (!!(x.kettenzahl&1) && (neue_kettenzahl&1)
         	// Sonderfall: Falls es _genau_ auf eine Kette passt nur eine
         	&& (neue_kettenzahl>1 || x.max_fadenzahl!=x.kettenzahl*x.fadenzahl))
         {  ManuProC::Trace(trace_channel,"",NV("neue_kettenzahl(Zwischenw.)",neue_kettenzahl));
            ++neue_kettenzahl;
         }
         if (neue_kettenzahl!=x.kettenzahl) // geht es auf?
         {  if ((x.kettenzahl*x.faeden.begin()->faeden) % neue_kettenzahl)
               neue_kettenzahl=x.kettenzahl;
            else if ((x.kettenzahl*x.fadenzahl) % neue_kettenzahl)
               neue_kettenzahl=x.kettenzahl;
         }
         if (neue_kettenzahl<x.kettenzahl)
         {  x.faeden.begin()->faeden=(x.kettenzahl*x.faeden.begin()->faeden)
                     /neue_kettenzahl;
            x.fadenzahl=(x.kettenzahl*x.fadenzahl)/neue_kettenzahl;
            x.kettenzahl=neue_kettenzahl;
            goto reloop;
         }
      }
      if (x.max_fadenzahl && 2*x.fadenzahl<=x.max_fadenzahl)
      {  map_t::iterator j=i;
         bool prefer_same_index=false;
         do
         {  prefer_same_index=!prefer_same_index;
           for (++j;j!=intermed.end();++j)
           {  Kettscheibe &y=j->second;
              // gleichartige kombinieren
              // zuerst gleichen index probieren
              if (x.max_fadenzahl==y.max_fadenzahl
                  && (!prefer_same_index || x.index==y.index)
                  && x.fadenzahl+y.fadenzahl<=x.max_fadenzahl
                  && x.laenge==y.laenge && x.kettenzahl==y.kettenzahl
                  && x.wiederholungen.empty() && y.wiederholungen.empty())
              {  map_index_t newidx(i->first);
                 std::copy(y.artikel.begin(),y.artikel.end(),back_inserter(newidx.agv));
                 // hopefully i+j outlive the modifications to intermed
                 Kettscheibe &z=insert_between(intermed,newidx);
                 z=x;
                 z.fadenzahl+=y.fadenzahl;
                 std::copy(y.faeden.begin(),y.faeden.end(),back_inserter(z.faeden));
                 std::copy(y.artikel.begin(),y.artikel.end(),back_inserter(z.artikel));
                 GleicheFaedenZusammenfassen(z);
                 intermed.erase(j);
                 intermed.erase(i);
                 goto reloop;
              }
           }
         } while (prefer_same_index);
      }
   }
   // aufteilen
   for (map_t::iterator i=intermed.begin();i!=intermed.end();++i)
   {  Kettscheibe &x=i->second;
      if (x.max_fadenzahl && x.fadenzahl>x.max_fadenzahl && x.faeden.size()==1)
      {  Kettscheibe y=x;
         unsigned ketten=(x.fadenzahl*x.kettenzahl+x.max_fadenzahl-1)/x.max_fadenzahl;
         unsigned klein=x.fadenzahl*x.kettenzahl/ketten;
         unsigned anz_klein=(klein+1)*ketten-x.fadenzahl*x.kettenzahl;
         x.fadenzahl=klein;
         x.faeden.begin()->faeden=klein;
         x.kettenzahl=anz_klein;
         if (anz_klein!=ketten)
         {  y.fadenzahl=klein+1;
            y.faeden.begin()->faeden=klein+1;
            y.kettenzahl=ketten-anz_klein;
            insert_between(intermed,i->first)=y;
            goto reloop;
         }
      }
   }
   
   std::vector<Kettscheibe> result;
   for (map_t::const_iterator i=intermed.begin();i!=intermed.end();++i)
   {  if (i->second.index) result.push_back(i->second);
   }
   return result;
  }
  catch (SQLerror &e)
  {  return std::vector<Kettscheibe>();
  }
}
