/* $Id: Verfuegbarkeit.cc,v 1.16 2004/09/06 13:02:46 christof Exp $ */
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
#include <Misc/relops.h>
#include <Misc/TraceNV.h>

Verfuegbarkeit::mengen_t Verfuegbarkeit::Mengen::summe() const
{  return  ungeplant+geplant+vorraetig+geliefert+error;
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
   // über die Kinder von elter den passenden Eintrag suchen
   // lastoffs beinhaltet den aktuell gültigen offset
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
//      std::cerr << "verf_recurse::operator(): .error " << m << " von " << art << " über\n";
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
   mapindex idx(ae.Instanz(),ae.Artikel(),ae.getKdNr());
   if (!menge) menge=ae.getStueck();
   if (!!ae.getGeliefert())
   {  AuftragBase::mengen_t m=0;
      if (ae.getGeliefert()>offset)
         m=AuftragBase::min(menge,ae.getGeliefert()-offset);
      result[idx].geliefert+=m; // vorraetig+=m;
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
{	// offset wird von oben gerechnet (0=letzte benötigte Menge)
	mutable Verfuegbarkeit::mengen_t offset;
	mutable Verfuegbarkeit::map_t &result;
	const AufEintrag &kind;

	AuftragBase::mengen_t operator()(const AufEintragZu::st_reflist &aeb,AuftragBase::mengen_t m) const;
	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
	{  return AufEintragZu_sort::priority(b,a);
	}

	virtual ~benoe_recurse() {}
	benoe_recurse(const Verfuegbarkeit::mengen_t &o, Verfuegbarkeit::map_t &r,
			const AufEintrag &e) 
		: offset(o), result(r), kind(e) 
	{}
};
}

namespace{ struct ben_recurse_sort
{	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
	{  return AufEintragZu_sort::auftr_1230(b,a); }
};}

AuftragBase::mengen_t benoe_recurse::operator()(const AufEintragZu::st_reflist &rl,AuftragBase::mengen_t m) const
{  // offset ist in Kindereinheit und bezüglich Pfeil
   if (offset>rl.Menge) { offset-=rl.Menge; return 0; }
   if (offset+m>rl.Menge) { m=rl.Menge-offset; }

   AufEintrag ae(rl.AEB);
   AufEintragZu::list_t Kinder= 
   	AufEintragZu::get_Referenz_list(ae,AufEintragZu::list_kinder,
					AufEintragZu::list_Artikel,
					AufEintragZu::list_unsorted);
   Kinder.sort(ben_recurse_sort());
   // in Kindeinheit
   AuftragBase::mengen_t local_offset=0;
   for (AufEintragZu::list_t::iterator i=Kinder.begin();i!=Kinder.end();++i)
   {  if (i->Art!=kind.Artikel()) continue; // ignore
      if (i->AEB==kind && i->Pri==rl.Pri) break;
      local_offset+=i->Menge;
   }
   
   ArtikelBaum ab(ae.Artikel());
   double faktor=ab.Faktor(kind.Artikel()).as_float();
   AuftragBase::mengen_t offset_elter=(offset+local_offset)/faktor;
   AuftragBase::mengen_t m_elter=m/faktor;
   ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,
   		NV("local_offset",local_offset),NV("offset",offset),
   		NV("offset_elter",offset_elter),NV("m_elter",m_elter));
   Verfuegbarkeit::wozu_benoetigt(ae,result,m_elter,offset_elter);
   offset=0; // wenn es für uns schon reichte, dann wird der nächste auch dran sein
   return m; 
}

// offset wird von hinten an gerechnet, das vereinfacht die Sache deutlich
void Verfuegbarkeit::wozu_benoetigt(const AufEintrag &ae, map_t &result, 
	mengen_t menge, mengen_t offset)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("ae",ae),
		NV("menge",menge),NV("offset",offset));
   mapindex idx(ae.Instanz(),ae.Artikel(),ae.getKdNr());
   if (!menge && !offset) 
   {  result[idx].geliefert+=ae.getGeliefert();
      menge=ae.getRestStk();
   }
   // shouldn't we error in verf too ?
   if (offset>=ae.getRestStk()) 
   { result[idx].error+=menge; verf_Trace(result[idx]); return; }
   if (offset+menge>ae.getRestStk()) 
   { result[idx].error+=offset+menge-ae.getRestStk();
     menge=ae.getRestStk()-offset;
   }
   if (!menge) { verf_Trace(result[idx]); return; }
   ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,
   		NV("menge",menge),NV("offset",offset));

   if (ae.Instanz()->LagerInstanz() && ae.Id()==AuftragBase::auffuellen_id)
   {  result[idx].vorraetig+=menge;
      menge=0; // keine Rekursion
   }
   else if (ae.Id()==AuftragBase::dispo_id)
   {  assert(!ae.Instanz()->LagerInstanz());
      result[idx].vorraetig+=menge; // vorraetig statt geplant
      result[idx].geplant-=menge;
      menge=0;
   }
   else if (ae.Id()==AuftragBase::plan_auftrag_id || ae.Id()>=AuftragBase::handplan_auftrag_id)
   {  assert(!ae.Instanz()->LagerInstanz());
      result[idx].geplant+=menge;
      if (ae.Instanz()==ppsInstanzID::Kundenauftraege) menge=0;
   }
   else
   {  assert(ae.Id()==AuftragBase::ungeplante_id);
      result[idx].ungeplant+=menge;
   }
   
   if (!!menge)
   {  // Rekursion (Priority)
      mengen_t rest=
         distribute_parents(ae,menge,benoe_recurse(offset,result,ae));
      if (!!rest) result[idx].error+=rest;
   }
   verf_Trace(result[idx]);
}

void Verfuegbarkeit::benoetigt(const AufEintrag &ae, map_det_t &result, mengen_t menge, mengen_t offset)
{
}

namespace
{
struct benoe_recurse2 : distribute_parents_cb
{	// offset wird von oben gerechnet (0=letzte benötigte Menge)
	mutable Verfuegbarkeit::mengen_t offset;
	mutable Verfuegbarkeit::map_det_t &result;
	const AufEintrag &kind;

	AuftragBase::mengen_t operator()(const AufEintragZu::st_reflist &aeb,AuftragBase::mengen_t m) const;
	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
	{  return AufEintragZu_sort::priority(b,a);
	}

	virtual ~benoe_recurse2() {}
	benoe_recurse2(const Verfuegbarkeit::mengen_t &o, Verfuegbarkeit::map_det_t &r,
			const AufEintrag &e) 
		: offset(o), result(r), kind(e) 
	{}
};
}

AuftragBase::mengen_t benoe_recurse2::operator()(const AufEintragZu::st_reflist &rl,AuftragBase::mengen_t m) const
{  // offset ist in Kindereinheit und bezüglich Pfeil
   if (offset>rl.Menge) { offset-=rl.Menge; return 0; }
   if (offset+m>rl.Menge) { m=rl.Menge-offset; }

   AufEintrag ae(rl.AEB);
   AufEintragZu::list_t Kinder= 
   	AufEintragZu::get_Referenz_list(ae,AufEintragZu::list_kinder,
					AufEintragZu::list_Artikel,
					AufEintragZu::list_unsorted);
   Kinder.sort(ben_recurse_sort());
   // in Kindeinheit
   AuftragBase::mengen_t local_offset=0;
   for (AufEintragZu::list_t::iterator i=Kinder.begin();i!=Kinder.end();++i)
   {  if (i->Art!=kind.Artikel()) continue; // ignore
      if (i->AEB==kind && i->Pri==rl.Pri) break;
      local_offset+=i->Menge;
   }
   
   ArtikelBaum ab(ae.Artikel());
   double faktor=ab.Faktor(kind.Artikel()).as_float();
   AuftragBase::mengen_t offset_elter=(offset+local_offset)/faktor;
   AuftragBase::mengen_t m_elter=m/faktor;
   ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,
   		NV("local_offset",local_offset),NV("offset",offset),
   		NV("offset_elter",offset_elter),NV("m_elter",m_elter));
   Verfuegbarkeit::wozu_benoetigt(ae,result,m_elter,offset_elter);
   offset=0; // wenn es für uns schon reichte, dann wird der nächste auch dran sein
   return m; 
}

void Verfuegbarkeit::wozu_benoetigt(const AufEintrag &ae, map_det_t &result, mengen_t menge, mengen_t offset)
{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("ae",ae),
		NV("menge",menge),NV("offset",offset));
   if (!menge && !offset) 
   {  menge=ae.getRestStk();
   }
   // shouldn't we error in verf too ?
   if (offset>=ae.getRestStk()) return;
   if (offset+menge>ae.getRestStk()) 
   { menge=ae.getRestStk()-offset;
   }
   if (!menge) return;

   ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,
   		NV("menge",menge),NV("offset",offset));

   if (ae.Instanz()->LagerInstanz() && ae.Id()==AuftragBase::auffuellen_id)
   {  result[ae]+=menge;
      menge=0; // keine Rekursion
   }
   else if (ae.Id()==AuftragBase::dispo_id)
   {  assert(!ae.Instanz()->LagerInstanz());
      result[ae]+=menge; // vorraetig statt geplant
      menge=0;
   }
   else if (ae.Id()==AuftragBase::plan_auftrag_id || ae.Id()>=AuftragBase::handplan_auftrag_id)
   {  assert(!ae.Instanz()->LagerInstanz());
      result[ae]+=menge;
      if (ae.Instanz()==ppsInstanzID::Kundenauftraege) menge=0;
   }
   else
   {  assert(ae.Id()==AuftragBase::ungeplante_id);
      result[ae]+=menge;
   }
   
   if (!!menge)
   {  // Rekursion (Priority)
      mengen_t rest=
         distribute_parents(ae,menge,benoe_recurse2(offset,result,ae));
   }
}
