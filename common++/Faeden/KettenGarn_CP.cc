/* $Id: KettenGarn_CP.cc,v 1.10 2004/05/27 11:20:30 christof Exp $ */
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

#warning diese API unterstützt keine Kombinationen!!!

static const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channel_e("DEBUG_KETTEN",trace_channel);
                  
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
   unsigned new_scheibe=20;
   for (Fadenliste::const_iterator i=fdl.begin(); i!=fdl.end(); ++i)
   {  intermediate_data x;
      unsigned max_fadenzahl=i->max_fadenzahl;
      if (ag.gaenge==i->ausn_gaenge && i->ausn_maxfd) 
         max_fadenzahl=i->ausn_maxfd;
      else if (ag.gaenge==i->ausn_gaenge2 && i->ausn_maxfd2) 
         max_fadenzahl=i->ausn_maxfd2;
      
      if (i->kettscheibe<1) continue;
      x.index=i->kettscheibe;
      x.zeile=i->zeilennummer;
      x.kettenzahl=ag.gaenge;
      x.faeden=i->anzahl;
      if (ag.gaenge==i->ausn_gaenge && i->ausn_faeden) 
      	 x.faeden=i->ausn_faeden;
      x.art=i->material;
      x.laenge=laenge;
      if (i->verlaengern) x.laenge+=500;
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
      // vielleicht in gleich große Portionen teilen wenn möglich?
      // schlechte Idee, da Teilmengen wieder kombiniert werden
      x.index=i->kettscheibe;
      while (max_fadenzahl && x.faeden>max_fadenzahl)
      {  unsigned tmp=x.faeden-max_fadenzahl;
         x.faeden=max_fadenzahl;
         result.push_back(x);
         x.faeden=tmp;
         x.index=new_scheibe++;
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
      // aus 6 mach 4
      else if (ks_end-i==1 && alte_kettenzahl==6 && anz_fd+anz_fd/2<=min_max_fd)
         neue_kettenzahl=4;
      // aus 8 mach 6 :-O
      else if (ks_end-i==1 && alte_kettenzahl==8 && (anz_fd*3/4)==min_max_fd)
         neue_kettenzahl=4;
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
