/* $Id: AufEintrag_loops.cc,v 1.4 2003/07/25 08:00:09 christof Exp $ */
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

static std::string Nametrans(std::string n)
{  if (n[0]=='N')
   {  const char *x=n.c_str()+1,*end=0;
      long num=strtol(x,const_cast<char**>(&end),10);
      if (end) num+=end-x;
      n=n.substr(num+1);
   }
   if ('0' <= n[0] && n[0] <= '9')
   {  const char *x=n.c_str(),*end=0;
      long num=strtol(x,const_cast<char**>(&end),10);
      if (end) n=n.substr(end-x,num);
   }
   return n;
}

// passende Menge für distribute_children
static AufEintragBase::mengen_t
	MinPfeil_or_MinGeliefert(const AufEintragZu::st_reflist &zuloop_var,AufEintragBase::mengen_t AE_menge2)
{  if (AE_menge2>=0) 
      return AuftragBase::min(zuloop_var.Menge,AE_menge2);
   else 
      return -AuftragBase::min(-AE_menge2,AufEintrag(zuloop_var.AEB).getGeliefert());
}

bool distribute_children(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_cb &callee)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,article,Nametrans(typeid(callee).name()));
   AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel(startAEB));
   ArtikelBaum AE_artbaum(article);
   for(AufEintragZu::map_t::const_iterator artloop_var=MapArt.begin();artloop_var!=MapArt.end();++artloop_var)
   {  ArtikelBaum::faktor_t AE_faktor = AE_artbaum.Faktor(artloop_var->first);
      AuftragBase::mengen_t AE_menge2=AE_faktor*menge;
      for(AufEintragZu::list_t::const_iterator zuloop_var=artloop_var->second.begin();
	   		zuloop_var!=artloop_var->second.end();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var
      		=MinPfeil_or_MinGeliefert(*zuloop_var,AE_menge2);
         if (!mengen_var) continue;

         mengen_var=callee(artloop_var->first,zuloop_var->AEB,mengen_var);

         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      // pass the remainder
      if (!!AE_menge2) callee(artloop_var->first,AE_menge2);
   }
   return !MapArt.empty();
}

bool distribute_children_rev(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_cb &callee)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,article,Nametrans(typeid(callee).name()));
   AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel(startAEB));
   ArtikelBaum AE_artbaum(article);
   for(AufEintragZu::map_t::const_iterator artloop_var=MapArt.begin();artloop_var!=MapArt.end();++artloop_var)
   {  ArtikelBaum::faktor_t AE_faktor = AE_artbaum.Faktor(artloop_var->first);
      AuftragBase::mengen_t AE_menge2=AE_faktor*menge;
      for(AufEintragZu::list_t::const_reverse_iterator zuloop_var=artloop_var->second.rbegin();
	   		zuloop_var!=artloop_var->second.rend();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var
      		=MinPfeil_or_MinGeliefert(*zuloop_var,AE_menge2);
         if (!mengen_var) continue;

         mengen_var=callee(artloop_var->first,zuloop_var->AEB,mengen_var);

         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      // pass the remainder
      if (!!AE_menge2) callee(artloop_var->first,AE_menge2);
   }
   return !MapArt.empty();
}

bool distribute_children_twice_rev(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_twice_cb &callee)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,article,Nametrans(typeid(callee).name()));
   AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel(startAEB));
   ArtikelBaum AE_artbaum(article);
   for(AufEintragZu::map_t::iterator artloop_var=MapArt.begin();artloop_var!=MapArt.end();++artloop_var)
   {  ArtikelBaum::faktor_t AE_faktor = AE_artbaum.Faktor(artloop_var->first);
      AuftragBase::mengen_t AE_menge2=AE_faktor*menge;
      for(AufEintragZu::list_t::reverse_iterator zuloop_var=artloop_var->second.rbegin();
	   		zuloop_var!=artloop_var->second.rend();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var
      		=MinPfeil_or_MinGeliefert(*zuloop_var,AE_menge2);
         if (!mengen_var) continue;

         mengen_var=callee(artloop_var->first,zuloop_var->AEB,mengen_var,true);

	 zuloop_var->Menge-=mengen_var; // for the second iteration
         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      for(AufEintragZu::list_t::reverse_iterator zuloop_var=artloop_var->second.rbegin();
	   		zuloop_var!=artloop_var->second.rend();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var
      		=MinPfeil_or_MinGeliefert(*zuloop_var,AE_menge2);
         if (!mengen_var) continue;

         mengen_var=callee(artloop_var->first,zuloop_var->AEB,mengen_var,false);

         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      // pass the remainder
      if (!!AE_menge2) callee(artloop_var->first,AE_menge2);
   }
   return !MapArt.empty();
}

AuftragBase::mengen_t distribute_parents(const AufEintragBase &startAEB, 
	AuftragBase::mengen_t menge,const distribute_parents_cb &callee)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,startAEB,menge,Nametrans(typeid(callee).name()));
   assert(menge>0);
   AufEintragZu::list_t Eltern =
        AufEintragZu::get_Referenz_list(startAEB,AufEintragZu::list_eltern,
                                         AufEintragZu::list_ohneArtikel);
   for (AufEintragZu::list_t::iterator i=Eltern.begin();i!=Eltern.end();++i)
   {  AuftragBase::mengen_t m=AuftragBase::min(i->Menge,menge);
      if (!m) continue;

      m=callee(i->AEB,m);

      menge-=m;
      if (!menge) break;
   }
   return menge;
}

AuftragBase::mengen_t auf_positionen_verteilen(const SQLFullAuftragSelector &selector,
 		AuftragBase::mengen_t menge, const auf_positionen_verteilen_cb &callee)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,menge,Nametrans(typeid(callee).name()));
   SelectedFullAufList auftraglist=SelectedFullAufList(selector);

  AuftragBase::mengen_t m=menge;
  for (SelectedFullAufList::iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t M;
     if (menge>=0) M=AuftragBase::min(i->getRestStk(),m);
     else M=-AuftragBase::min(-m,i->getGeliefert());
     if (!M) continue;

     M=callee(*i,M);

     m-=M;
     if(!m) break;
   }
   return m;
}

AuftragBase::mengen_t auf_positionen_verteilen_rev(const SQLFullAuftragSelector &selector,
 		AuftragBase::mengen_t menge, const auf_positionen_verteilen_cb &callee)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,menge,typeid(callee).name());
   SelectedFullAufList auftraglist=SelectedFullAufList(selector);

  AuftragBase::mengen_t m=menge;
  for (SelectedFullAufList::reverse_iterator i=auftraglist.rbegin();i!=auftraglist.rend();++i)
   {
     AuftragBase::mengen_t M;
     if (menge>=0) M=AuftragBase::min(i->getRestStk(),m);
     else M=-AuftragBase::min(-m,i->getGeliefert());
     if (!M) continue;

     M=callee(*i,M);

     m-=M;
     if(!m) break;
   }
   return m;
}

