/* $Id: Verfuegbarkeit.cc,v 1.9 2004/02/27 16:35:59 christof Exp $ */
/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <Auftrag/AufEintrag.h>
#include <Auftrag/Verfuegbarkeit.h>
//#include <Auftrag/AufEintrag_loops.h>
#include <Misc/relops.h>
#include <Misc/TraceNV.h>

Verfuegbarkeit::mengen_t Verfuegbarkeit::Mengen::summe() const
{  return  ungeplant+geplant+vorraetig+error;
}

namespace
{
struct verf_recurse : distribute_children_cb
{	Verfuegbarkeit::mengen_t offset;
	mutable Verfuegbarkeit::map_t &result;
	mutable ArtikelBaum artbaum;

	mutable ArtikelBase lastart;
	mutable Verfuegbarkeit::mengen_t lastoffs;
	mutable cH_ppsInstanz lastinst;
	
	AufEintragBase elter;

	AuftragBase::mengen_t operator()(const ArtikelBase &, 
	                const AufEintragZu::st_reflist &,AuftragBase::mengen_t) const;
	void operator()(const ArtikelBase &,AuftragBase::mengen_t) const;
	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
	{  return AufEintragZu_sort::auftr_1230(a,b);
	}

	virtual ~verf_recurse() {}
	verf_recurse(const Verfuegbarkeit::mengen_t &o, Verfuegbarkeit::map_t &r,
			const ArtikelBaum &ab, const AufEintragBase &e) 
		: offset(o), result(r), artbaum(ab), elter(e) 
	{}
};
}

namespace{ struct verf_recurse_sort
{	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
	{  return AufEintragZu_sort::priority(b,a); }
};}

AuftragBase::mengen_t verf_recurse::operator()(const ArtikelBase &art, 
	                const AufEintragZu::st_reflist &rl,AuftragBase::mengen_t m) const
{  ManuProC::Trace(AuftragBase::trace_channel, "",
		NV("lastoffs",lastoffs),NV("elter",elter));
   // �ber die Kinder von elter den passenden Eintrag suchen
   // lastoffs beinhaltet den aktuell g�ltigen offset
   if (art!=lastart)
   {  lastart=art;
      lastoffs=offset*artbaum.Faktor(art);
      lastinst=rl.AEB.Instanz();
      ManuProC::Trace(AuftragBase::trace_channel,"[new Article]",
   		NV("offset",offset),NV("lastart",lastart),
   		NV("lastoffs",lastoffs)); // ,NV("lastinst",lastinst));
   }
   AuftragBase::mengen_t offset=lastoffs;
   lastoffs-=rl.Menge;
   if (lastoffs<0) lastoffs=0;
//   if (offset>=m) return 0;
   
   AufEintrag ae(rl.AEB);
   if (m+offset>ae.getStueck()) m=ae.getStueck()-offset;
   if (m<=0) return 0;
   
   AufEintragZu::list_t Eltern= 
   	AufEintragZu::get_Referenz_list(ae,AufEintragZu::list_eltern,
					AufEintragZu::list_ohneArtikel,
					AufEintragZu::list_unsorted);
   Eltern.sort(verf_recurse_sort());
   AuftragBase::mengen_t local_offset=ae.getStueck();
   for (AufEintragZu::list_t::iterator i=Eltern.begin();i!=Eltern.end();++i)
   {  local_offset-=i->Menge;
      if (i->AEB==elter && i->Pri==rl.Pri) break;
   }
   ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,
   		NV("local_offset",local_offset),
   		NV("lastoffs",lastoffs),NV("offset",offset));
   Verfuegbarkeit::verfuegbar(ae,result,m,local_offset+offset);
   return m;
}

void verf_recurse::operator()(const ArtikelBase &art,AuftragBase::mengen_t m) const
{  cH_ppsInstanz bi=ppsInstanz::getBestellInstanz(art);
   if (bi==ppsInstanzID::Kundenauftraege || bi==ppsInstanzID::None) return;
   if (!!lastinst)
   {  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("art",art),
		NV("lastinst",lastinst),NV("m",m));
      result[Verfuegbarkeit::mapindex(lastinst,art)].error+=m;
   }
   else 
   {  result[Verfuegbarkeit::mapindex(elter.Instanz(),art)].error+=m;
//      std::cerr << "verf_recurse::operator(): .error " << m << " von " << art << " �ber\n";
   }
}

static void verf_Trace(const Verfuegbarkeit::Mengen &m)
{  ManuProC::Trace(AuftragBase::trace_channel,"=",
   		NV("vorr",m.vorraetig),NV("gepl",m.geplant),
   		NV("unge",m.ungeplant),NV("err",m.error));
}

void Verfuegbarkeit::verfuegbar(const AufEintrag &ae, map_t &result, 
	mengen_t menge, mengen_t offset)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("ae",ae),
		NV("menge",menge),NV("offset",offset));
   mapindex idx(ae.Instanz(),ae.Artikel());
   if (!menge) menge=ae.getStueck();
   if (!!ae.getGeliefert())
   {  AuftragBase::mengen_t m=0;
      if (ae.getGeliefert()>offset)
         m=AuftragBase::min(menge,ae.getGeliefert()-offset);
      result[idx].vorraetig+=m;
      menge-=m;
      offset-=ae.getGeliefert();
   }
   if (offset>=ae.getRestStk()) { verf_Trace(result[idx]); return; }
   if (offset+menge>ae.getRestStk()) menge=ae.getRestStk()-offset;
   if (!menge) { verf_Trace(result[idx]); return; }
   ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,
   		NV("menge",menge),NV("offset",offset));
   
   assert(ae.Id()!=AuftragBase::dispo_auftrag_id);
   if (ae.Id()==AuftragBase::plan_auftrag_id || ae.Id()>=AuftragBase::handplan_auftrag_id)
   {  if (ae.Instanz()->LagerInstanz())
      {  result[idx].vorraetig+=AuftragBase::min(ae.getRestStk(),menge);
         verf_Trace(result[idx]);
         return;
      }
      else
         result[idx].geplant+=AuftragBase::min(ae.getRestStk(),menge);
   }
   else
   {  assert(ae.Id()==AuftragBase::ungeplante_id);
      result[idx].ungeplant+=AuftragBase::min(ae.getRestStk(),menge);
   }
   
   // Rekursion (order 1230)
   distribute_children_artbaum(ae,menge,ae.Artikel(),
   		verf_recurse(offset,result,ae.Artikel(),ae));
   verf_Trace(result[idx]);
}

namespace
{
struct benoe_recurse : distribute_parents_cb
{	Verfuegbarkeit::mengen_t offset;
	mutable Verfuegbarkeit::map_t &result;
	mutable ArtikelBaum artbaum;

	mutable ArtikelBase lastart;
	mutable Verfuegbarkeit::mengen_t lastoffs;
	mutable cH_ppsInstanz lastinst;
	
	AufEintragBase elter;

	AuftragBase::mengen_t operator()(const AufEintragBase &aeb,AuftragBase::mengen_t m) const
	{  return m; }
	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
	{  return AufEintragZu_sort::priority(a,b);
	}

	virtual ~benoe_recurse() {}
	benoe_recurse(const Verfuegbarkeit::mengen_t &o, Verfuegbarkeit::map_t &r,
			const ArtikelBaum &ab, const AufEintragBase &e) 
		: offset(o), result(r), artbaum(ab), elter(e) 
	{}
};
}

void Verfuegbarkeit::wozu_benoetigt(const AufEintrag &ae, map_t &result, 
	mengen_t menge, mengen_t offset,const AufEintrag &von)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("ae",ae),
		NV("menge",menge),NV("offset",offset));
   if (offset>=ae.getStueck()) return;
   if (offset+menge>ae.getStueck()) menge=ae.getStueck()-offset;
   if (!menge) return;
   ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,NV("menge",menge));
   
   mapindex idx(ae.Instanz(),ae.Artikel()); // kunde?
   if (ae.Id()==AuftragBase::dispo_id)
   {  result[idx].vorraetig+=menge;
   }
   else if (ae.Id()==AuftragBase::plan_auftrag_id || ae.Id()>=AuftragBase::handplan_auftrag_id)
   {  assert(!ae.Instanz()->LagerInstanz());
      result[idx].geplant+=menge;
   }
   else
   {  assert(ae.Id()==AuftragBase::ungeplante_id);
      result[idx].ungeplant+=menge;
   }
   
   // Rekursion (Priority)
   distribute_parents(ae,menge,benoe_recurse(offset,result,ae.Artikel(),ae));
   verf_Trace(result[idx]);
}
