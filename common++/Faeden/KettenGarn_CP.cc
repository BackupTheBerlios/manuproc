/* $Id: KettenGarn_CP.cc,v 1.22 2004/07/06 14:27:33 christof Exp $ */
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
                  
#if 0
struct KG_compare
{  bool operator()(const KettenGarn_CP &a,const KettenGarn_CP &b)
   {  return a.index<b.index || (a.index==b.index
   	  && (a.zeile<b.zeile));
   }
};

struct intermediate_data : KettenGarn_CP
{  // Fadenliste::const_iterator source; ?
   unsigned min_max_fd;
};

std::vector<KettenGarn_CP> KettenGarn_CP::Load(ArtikelGang const &ag, unsigned laenge)
{  Fadenliste fdl;
   typedef std::vector<intermediate_data> vec_t;
   vec_t result;
   try {
   Webangaben wa(ag.art);
   wa.Load();
   fdl.Load(wa);
   }
   catch (SQLerror &e)
   {  return std::vector<KettenGarn_CP>();
   }

   // ermitteln!
//   unsigned new_scheibe=20;
   for (Fadenliste::const_iterator i=fdl.begin(); i!=fdl.end(); ++i)
   {  intermediate_data x;
      if (i->kettscheibe<1) continue;
      
      const Fd_Kettscheibe &ks=fdl.Kettscheibe(i->kettscheibe);
      unsigned max_fadenzahl=ks.max_fadenzahl;
      if (ag.gaenge==ks.ausn_gaenge && ks.ausn_maxfd) 
         max_fadenzahl=ks.ausn_maxfd;
      else if (ag.gaenge==ks.ausn_gaenge2 && ks.ausn_maxfd2) 
         max_fadenzahl=ks.ausn_maxfd2;
      
      x.index=i->kettscheibe*2;
      x.zeile=i->zeilennummer;
      x.kettenzahl=ag.gaenge;
      x.faeden=i->anzahl;
      if (ag.gaenge==i->ausn_gaenge && i->ausn_faeden) 
      	 x.faeden=i->ausn_faeden;
      x.art=i->material;
      x.laenge=laenge;
      if (ks.verlaengern) 
      {  if (x.laenge<10000) x.laenge+=100; // 3040/35/420 v. 15.1.04
         else if (x.laenge<=12000 || cH_ArtikelBezeichnung(i->material)->Komponente(0).substr(0,10)=="Poly verst")
            x.laenge+=200; // 3040/35/420 v. 15.1.04
         else 
            x.laenge+=500;
      }
      // 2do: wiederholungen 
      x.wiederholungen=1;
      for (Fadenliste::const_repiterator r=fdl.repbegin();r!=fdl.repend();++r)
      {  if (r->start==r->end && r->start==i->zeilennummer)
         {  // Sonderfall: Einzelwiederholung
            x.faeden*=r->anzahl;
         }
         else if (r->start<=i->zeilennummer && i->zeilennummer<=r->end)
         {  x.wiederholungen*=r->anzahl;
         }
      }
      x.min_max_fd=max_fadenzahl;
      // in gleich große Portionen teilen
      if (max_fadenzahl && x.faeden>max_fadenzahl) // && !(x.kettenzahl&1))
      {  unsigned ketten=(x.faeden*x.kettenzahl+max_fadenzahl-1)/max_fadenzahl;
         unsigned klein=x.faeden*x.kettenzahl/ketten;
         unsigned anz_klein=(klein+1)*ketten-x.faeden*x.kettenzahl;
         if (anz_klein!=ketten)
         {  x.faeden=klein+1;
            x.kettenzahl=ketten-anz_klein;
            result.push_back(x);
            x.index++;
         }
         x.faeden=klein;
         x.kettenzahl=anz_klein;
      }
      result.push_back(x);
   }
   // sortieren?
   std::sort(result.begin(),result.end(),KG_compare());
   // zusammenfassen?
   for (vec_t::iterator i=result.begin();i!=result.end();)
   {  vec_t::iterator b=i+1;
      if (b==result.end()) break;
      if (i->index==b->index && i->art==b->art && i->kettenzahl==b->kettenzahl
      		&& i->laenge==b->laenge && i->wiederholungen==b->wiederholungen)
      {  i->faeden+=b->faeden;
         if (b->min_max_fd<i->min_max_fd) i->min_max_fd=b->min_max_fd;
         i=result.erase(b);
      }
      else ++i;
   }
   for (vec_t::iterator i=result.begin();i!=result.end();)
   {  vec_t::iterator ks_end=i;
      unsigned min_max_fd=i->min_max_fd;
      unsigned anz_fd=0;
      while (ks_end!=result.end() && ks_end->index==i->index)
      {  if (ks_end->min_max_fd<min_max_fd) min_max_fd=ks_end->min_max_fd;
         anz_fd+=ks_end->faeden*ks_end->wiederholungen;
         assert(ks_end->kettenzahl==i->kettenzahl);
         ++ks_end;
      }
      ManuProC::Trace(trace_channel,"",NV("min_max_fd",min_max_fd),
      		NV("anz_fd",anz_fd));
      unsigned alte_kettenzahl=i->kettenzahl;
      unsigned neue_kettenzahl=alte_kettenzahl;
      // be a little fuzzy (+1)
      // weg?
      if ((2*anz_fd == min_max_fd+1 || 3*anz_fd == min_max_fd+1) && ks_end-i==1)
      {  neue_kettenzahl=(anz_fd*alte_kettenzahl+min_max_fd-1)/(min_max_fd+1);
      }
      else if (2*anz_fd <= min_max_fd && ks_end-i==1)
      {  neue_kettenzahl=(anz_fd*alte_kettenzahl+min_max_fd-1)/min_max_fd;
         if (!(ag.gaenge&1) && (neue_kettenzahl&1)
         	// Sonderfall: Falls es _genau_ auf eine Kette passt nur eine
         	&& (neue_kettenzahl>1 || min_max_fd!=alte_kettenzahl*anz_fd))
         {  ManuProC::Trace(trace_channel,"",NV("neue_kettenzahl(Zwischenw.)",neue_kettenzahl));
            ++neue_kettenzahl;
         }
      }
      // aus 6 mach 4 (weg?)
      else if (ks_end-i==1 && alte_kettenzahl==6 && anz_fd+anz_fd/2<=min_max_fd)
         neue_kettenzahl=4;
      // aus 8 mach 6 :-O (weg?)
      else if (ks_end-i==1 && alte_kettenzahl==8 && (anz_fd*4/3)==min_max_fd)
         neue_kettenzahl=6;
      ManuProC::Trace(trace_channel,"",NV("neue_kettenzahl",neue_kettenzahl));
      if (neue_kettenzahl!=alte_kettenzahl) // geht es auf?
      {  for (vec_t::iterator j=i;j!=ks_end;++j)
            if ((alte_kettenzahl*j->faeden) % neue_kettenzahl) 
               neue_kettenzahl=alte_kettenzahl;
      }
      if (neue_kettenzahl!=alte_kettenzahl) // dann kombinieren?
      {  for (vec_t::iterator j=i;j!=ks_end;++j)
         {  j->kettenzahl=neue_kettenzahl;
            j->faeden=(alte_kettenzahl*j->faeden)/ neue_kettenzahl;
         }
      }
      i=ks_end;
   }
   std::vector<KettenGarn_CP> real_result;
   real_result.reserve(result.size());
   for (vec_t::const_iterator i=result.begin();i!=result.end();++i)
      real_result.push_back(*i);
   return real_result;
}

bool KettenGarn::operator==(const KettenGarn_CP &b) const throw()
	{  return Index()==b.Index() && 
	          Artikel()==b.Artikel() &&
	          Faeden()==b.Faeden() &&
	          Kettenzahl()==b.Kettenzahl() &&
	          Laenge()==b.Laenge() ;}
bool KettenGarn::operator<(const KettenGarn_CP &b) const throw()
	{  return (Index()<b.Index()) || 
	          (Index()==b.Index() && Artikel()<b.Artikel() );}
#endif

#if 1

namespace {
struct map_index_t
{	ArtikelGang ag,ag2;
        unsigned scheibe;
        
        map_index_t(const ArtikelGang &a, unsigned s) : ag(a), scheibe(s) {}
        map_index_t(const ArtikelGang &a, const ArtikelGang &b, unsigned s) 
          : ag(a), ag2(b), scheibe(s) {}
        bool operator==(const map_index_t &b) const
        {  return ag==b.ag && ag==b.ag2 && scheibe==b.scheibe; }
        bool operator<(const map_index_t &b) const
        {  return ag<b.ag ||
              (ag==b.ag && ag2<b.ag2) ||
              (ag==b.ag && ag2==b.ag2 && scheibe<b.scheibe); 
        }
        const map_index_t &operator++()
        {  ++scheibe;
           return *this;
        }
};
}

std::vector<Kettscheibe> Kettscheibe::Load(const std::vector<ArtikelGang> &ag, unsigned laenge)
{  typedef std::vector<Kettscheibe> vec_t;
   vec_t result;
   typedef std::map<map_index_t,Kettscheibe> map_t;
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
            }
         }
      }
   }
  }
  
   // gleiche aufeinanderfolgende Fäden zusammenfassen
   for (map_t::iterator i=intermed.begin();i!=intermed.end();++i)
   {  Kettscheibe &x=i->second;
      for (std::vector<KS_Garn>::iterator j=x.faeden.begin();j!=x.faeden.end();)
      {  std::vector<KS_Garn>::iterator b=j+1;
         if (b==x.faeden.end()) break;
         if (j->material==b->material && j->wiederholungen==b->wiederholungen)
         {  j->faeden+=b->faeden;
            j=x.faeden.erase(b);
         }
         else ++j;
      }
   }
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
   
   for (map_t::const_iterator i=intermed.begin();i!=intermed.end();++i)
   {  if (i->second.index) result.push_back(i->second);
   }
   std::vector<Kettscheibe> real_result;
   real_result.reserve(result.size());
   for (vec_t::const_iterator i=result.begin();i!=result.end();++i)
      real_result.push_back(*i);
   return real_result;
  }
  catch (SQLerror &e)
  {  return std::vector<Kettscheibe>();
  }
}
#endif
