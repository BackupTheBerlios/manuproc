/* $Id: AufEintrag_loops.cc,v 1.23 2004/09/01 08:57:45 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Auftrag/AufEintrag_loops.h> 
#include <Auftrag/AufEintragZu.h>
#include <Misc/TraceNV.h>
#include <typeinfo>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <Artikel/ArtikelStamm.h>
#include <iterator>

// Kinder !!! d.h. nicht nach Priorität
bool distribute_children_cb::operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
{  return AufEintragZu_sort::auftr_0123(a,b);
//a.AEB<b.AEB || (a.AEB==b.AEB && a.Pri<b.Pri);
}

bool distribute_children_twice_cb::operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
{  return distribute_children_cb::operator()(a,b);
}

bool distribute_parents_cb::operator()(AufEintragZu::st_reflist const &a, AufEintragZu::st_reflist const &b) const
{  return AufEintragZu_sort::priority(a,b);
}

AuftragBase::mengen_t distribute_parents_cb::operator()(const AufEintragBase &,
                                AuftragBase::mengen_t) const
{  assert(!"called");
}

static std::string Nametrans(std::string n)
{  static const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();
   static ManuProC::Tracer::Environment trace_channel_e("DEBUG_NAMETRANS",trace_channel);

   ManuProC::Trace _t(trace_channel,__FUNCTION__,n);
   if (n[0]=='N') // g++ 3.0+
   {  const char *x=n.c_str()+1,*end=0;
      long num=strtol(x,const_cast<char**>(&end),10);
      if (end) num+=end-x;
      n=n.substr(num+1);
   }
   if (n[0]=='Q') // g++ 2.95
   {  assert(n[1]=='2');
      const char *x=n.c_str()+2,*end=0;
      long num=strtol(x,const_cast<char**>(&end),10);
      if (end) num+=end-x;
      n=n.substr(num+2);
   }
   if ('0' <= n[0] && n[0] <= '9')
   {  const char *x=n.c_str(),*end=0;
      long num=strtol(x,const_cast<char**>(&end),10);
      if (end) n=n.substr(end-x,num);
   }
   ManuProC::Trace(trace_channel,"=",n);
   return n;
}

static const UniqueValue::value_t trace_channel_menge=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channelme("DEBUG_MENGE",trace_channel_menge);

// passende Menge für distribute_children, distribute_parents
static AufEintragBase::mengen_t
	MinPfeil_or_MinGeliefert(const AufEintragZu::st_reflist &zuloop_var,
				AufEintragBase::mengen_t AE_menge2,
				bool offen=false)
{  if (AE_menge2>=0) 
   {  ManuProC::Trace _t(trace_channel_menge, __FUNCTION__,"min",zuloop_var.Menge,AE_menge2);
      return AuftragBase::min(zuloop_var.Menge,AE_menge2);
   }
   else 
   {  AufEintrag ae(zuloop_var.AEB);
      AuftragBase::mengen_t vergl=offen?ae.getRestStk():ae.getGeliefert();
      ManuProC::Trace _t(trace_channel_menge, __FUNCTION__,"-min",-AE_menge2,vergl,
      		NV("AEB",zuloop_var.AEB),NV("offen",offen));
      if (offen) // Spezialfall: Rückgängig von EinlagernIn
      {  assert(ae.Instanz()->LagerInstanz());
         assert(ae.Id()==AuftragBase::plan_auftrag_id);
      }
      return -AuftragBase::min(-AE_menge2,vergl);
   }
}

// if anybody has a more straight way to do this tell me !
/*  list::sort(comp) seems to try to allocate new objects of the passed type
*/
namespace{ template <typename T=distribute_children_cb> class sort_wrapper
{	const T &c;
public:
	sort_wrapper(const T &dcc) : c(dcc) {}
	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
	{  return c(a,b); }
};}

std::ostream &operator<<(std::ostream &o,const AufEintragZu::st_reflist &r)
{  return o << r.Pri << ':' << r.AEB;
}

bool distribute_children(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_cb &callee)
{  std::string callee_t=Nametrans(typeid(callee).name());
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,article,callee_t);
   AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel
   	(startAEB,AufEintragZu::list_kinder,AufEintragZu::list_unsorted));
   ArtikelBaum AE_artbaum(article);
   for(AufEintragZu::map_t::iterator artloop_var=MapArt.begin();artloop_var!=MapArt.end();++artloop_var)
   {  ArtikelBaum::faktor_t AE_faktor = AE_artbaum.Faktor(artloop_var->first);
      AuftragBase::mengen_t AE_menge2=AE_faktor*menge;
      artloop_var->second.sort(sort_wrapper<>(callee));
if (ManuProC::Tracer::enabled(AuftragBase::trace_channel))
{   std::ostream_iterator<AufEintragZu::st_reflist> os(std::cout," ");
std::cout << "after sort ";
std::copy(artloop_var->second.begin(),artloop_var->second.end(),os);
std::cout << '\n';
}
      for(AufEintragZu::list_t::const_iterator zuloop_var=artloop_var->second.begin();
	   		zuloop_var!=artloop_var->second.end();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var
      		=MinPfeil_or_MinGeliefert(*zuloop_var,AE_menge2);
         if (!mengen_var) continue;

         {ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,artloop_var->first,zuloop_var->AEB,mengen_var);
          mengen_var=callee(artloop_var->first,*zuloop_var,mengen_var);
         }

         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      // pass the remainder
      if (!!AE_menge2) 
      {  ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,artloop_var->first,AE_menge2);
         callee(artloop_var->first,AE_menge2);
      }
   }
   return !MapArt.empty();
}

bool distribute_children_rev(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_cb &callee)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,article,Nametrans(typeid(callee).name()));
   return distribute_children(startAEB,menge,
		   	article,distribute_children_cb_inverter(callee));
}

void distribute_children_artbaum(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_cb &callee)
{  std::string callee_t=Nametrans(typeid(callee).name());
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,article,callee_t);
   AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel(startAEB));
   ArtikelBaum AE_artbaum(article);
   for(AufEintragZu::map_t::iterator artloop_var=MapArt.begin();artloop_var!=MapArt.end();++artloop_var)
   {  ArtikelBaum::faktor_t AE_faktor = AE_artbaum.Faktor(artloop_var->first);
      AuftragBase::mengen_t AE_menge2=AE_faktor*menge;
      artloop_var->second.sort(sort_wrapper<>(callee));
      for(AufEintragZu::list_t::const_iterator zuloop_var=artloop_var->second.begin();
	   		zuloop_var!=artloop_var->second.end();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var
      		=MinPfeil_or_MinGeliefert(*zuloop_var,AE_menge2);
         if (!mengen_var) continue;

         {ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,artloop_var->first,zuloop_var->AEB,mengen_var);
          mengen_var=callee(artloop_var->first,*zuloop_var,mengen_var);
         }

         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      // pass the remainder
      if (!!AE_menge2) 
      {  ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,artloop_var->first,AE_menge2);
         callee(artloop_var->first,AE_menge2);
      }
   }
   // bei externer Bestellung Baum abschneiden
   if (startAEB.Instanz()->ExterneBestellung()) return;
   
   ppsInstanz::ID next= startAEB.Instanz()->NaechsteInstanz(ArtikelStamm(article));
   if (next!=ppsInstanzID::None)
   {  if (MapArt.find(article)==MapArt.end()) 
      {  ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,article,menge);
         callee(article,menge);
      }
   }
   else for(ArtikelBaum::const_iterator i=AE_artbaum.begin();i!=AE_artbaum.end();++i)
   {  if (MapArt.find(i->rohartikel)==MapArt.end())
      {  ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,i->rohartikel,menge*i->menge);
         callee(i->rohartikel,menge*i->menge);
      }
   }
}

void distribute_children_rev_artbaum(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_cb &callee)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,article,Nametrans(typeid(callee).name()));
   distribute_children_artbaum(startAEB,menge,article,
   			distribute_children_cb_inverter(callee));
}

bool distribute_children_twice(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_twice_cb &callee)
{  std::string callee_t=Nametrans(typeid(callee).name());
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,article,callee_t);
   AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel(startAEB));
   ArtikelBaum AE_artbaum(article);
   for(AufEintragZu::map_t::iterator artloop_var=MapArt.begin();artloop_var!=MapArt.end();++artloop_var)
   {  ArtikelBaum::faktor_t AE_faktor = AE_artbaum.Faktor(artloop_var->first);
      AuftragBase::mengen_t AE_menge2=AE_faktor*menge;
      artloop_var->second.sort(sort_wrapper<>(callee));
      for(AufEintragZu::list_t::iterator zuloop_var=artloop_var->second.begin();
	   		zuloop_var!=artloop_var->second.end();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var
      		=MinPfeil_or_MinGeliefert(*zuloop_var,AE_menge2);
         if (!mengen_var) continue;

         {ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,artloop_var->first,zuloop_var->AEB,mengen_var,true);
          mengen_var=callee(artloop_var->first,zuloop_var->AEB,mengen_var,true);
         }

	 zuloop_var->Menge-=mengen_var; // for the second iteration
         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      for(AufEintragZu::list_t::iterator zuloop_var=artloop_var->second.begin();
	   		zuloop_var!=artloop_var->second.end();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var
      		=MinPfeil_or_MinGeliefert(*zuloop_var,AE_menge2);
         if (!mengen_var) continue;

         {ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,artloop_var->first,zuloop_var->AEB,mengen_var,false);
          mengen_var=callee(artloop_var->first,zuloop_var->AEB,mengen_var,false);
         }

         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      // pass the remainder
      if (!!AE_menge2) 
      {  ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,artloop_var->first,AE_menge2);
         callee(artloop_var->first,AE_menge2);
      }
   }
   return !MapArt.empty();
}

AuftragBase::mengen_t distribute_parents(const AufEintragBase &startAEB, 
	AuftragBase::mengen_t menge,const distribute_parents_cb &callee)
{  std::string callee_t=Nametrans(typeid(callee).name());
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,callee_t);
//   assert(menge>0);
   AufEintragZu::list_t Eltern =
        AufEintragZu::get_Referenz_list(startAEB,AufEintragZu::list_eltern,
			AufEintragZu::list_ohneArtikel,AufEintragZu::list_unsorted);
   Eltern.sort(sort_wrapper<distribute_parents_cb>(callee));
   for (AufEintragZu::list_t::iterator i=Eltern.begin();i!=Eltern.end();++i)
   {  AuftragBase::mengen_t m=MinPfeil_or_MinGeliefert(*i,menge,true);
      if (!m) continue;

      {ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,i->AEB,m);
       m=callee(*i,m);
      }

      menge-=m;
      if (!menge) break;
   }
   if (!!menge) ManuProC::Trace(AuftragBase::trace_channel,"=",menge);
   return menge;
}

AuftragBase::mengen_t auf_positionen_verteilen(const SQLFullAuftragSelector &selector,
 		AuftragBase::mengen_t menge, const auf_positionen_verteilen_cb &callee)
{  std::string callee_t=Nametrans(typeid(callee).name());
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,menge,callee_t);
   SelectedFullAufList auftraglist=SelectedFullAufList(selector);

  AuftragBase::mengen_t m=menge;
  for (SelectedFullAufList::iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t M;
     if (menge>=0) M=AuftragBase::min(i->getRestStk(),m);
     else M=-AuftragBase::min(-m,i->getGeliefert());
     if (!M) continue;

     {ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,*i,M);
      M=callee(*i,M);
     }

     m-=M;
     if(!m) break;
   }
   if (!!m) ManuProC::Trace(AuftragBase::trace_channel,"=",m);
   return m;
}

AuftragBase::mengen_t auf_positionen_verteilen_rev(const SQLFullAuftragSelector &selector,
 		AuftragBase::mengen_t menge, const auf_positionen_verteilen_cb &callee)
{  std::string callee_t=Nametrans(typeid(callee).name());
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,menge,callee_t);
   SelectedFullAufList auftraglist=SelectedFullAufList(selector);

  AuftragBase::mengen_t m=menge;
  for (SelectedFullAufList::reverse_iterator i=auftraglist.rbegin();i!=auftraglist.rend();++i)
   {
     AuftragBase::mengen_t M;
     if (menge>=0) M=AuftragBase::min(i->getRestStk(),m);
     else M=-AuftragBase::min(-m,i->getGeliefert());
     if (!M) continue;

     {ManuProC::Trace _t(AuftragBase::trace_channel,callee_t,*i,M);
      M=callee(*i,M);
     }

     m-=M;
     if(!m) break;
   }
   if (!!m) ManuProC::Trace(AuftragBase::trace_channel,"=",m);
   return m;
}

AuftragBase::mengen_t distribute_children_cb::operator()(const ArtikelBase &,
		const AufEintragBase &,AuftragBase::mengen_t) const
{  assert(!"called");
}
