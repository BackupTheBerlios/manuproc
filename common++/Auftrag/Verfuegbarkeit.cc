/* $Id: Verfuegbarkeit.cc,v 1.3 2003/12/02 07:57:45 christof Exp $ */
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

Verfuegbarkeit::mengen_t Verfuegbarkeit::Mengen::summe() const
{  return  geliefert+ungeplant+geplant+vorraetig+error;
}

namespace
{
struct verf_recurse : distribute_children_cb
{	Verfuegbarkeit::mengen_t offset;
	mutable Verfuegbarkeit::map_t result;
	mutable ArtikelBaum artbaum;

	mutable ArtikelBase lastart;
	mutable Verfuegbarkeit::mengen_t lastoffs;
	mutable cH_ppsInstanz lastinst;

	AuftragBase::mengen_t operator()(const ArtikelBase &, 
	                const AufEintragBase &,AuftragBase::mengen_t) const;
	void operator()(const ArtikelBase &,AuftragBase::mengen_t) const;

	virtual ~verf_recurse() {}
	verf_recurse(const Verfuegbarkeit::mengen_t &o, Verfuegbarkeit::map_t &r,
			const ArtikelBaum &ab) 
		: offset(o), result(r), artbaum(ab) 
	{}
};
}

AuftragBase::mengen_t verf_recurse::operator()(const ArtikelBase &art, 
	                const AufEintragBase &aeb,AuftragBase::mengen_t m) const
{  if (art!=lastart)
   {  lastart=art;
      lastoffs=offset*artbaum.Faktor(art);
      lastinst=aeb.Instanz();
   }
   AuftragBase::mengen_t offset=lastoffs;
   
   AufEintragZu::list_t Eltern= 
   	AufEintragZu::get_Referenz_list(aeb,AufEintragZu::list_eltern,
					AufEintragZu::list_ohneArtikel);
   for (AufEintragZu::list_t::iterator i=Eltern.begin();i!=Eltern.end();++i)
   {  if (i->AEB==aeb) break;
      else offset+=i->Menge;
   }
   Verfuegbarkeit::verfuegbar(aeb,result,m,offset);
   return m;
}

void verf_recurse::operator()(const ArtikelBase &art,AuftragBase::mengen_t m) const
{  if (!!lastinst)
   {  result[Verfuegbarkeit::mapindex(lastinst,art)].error+=m;
   }
   else std::cerr << "verf_recurse::operator(): .error " << m << " von " << art << " über\n";
}

void Verfuegbarkeit::verfuegbar(const AufEintrag &ae, map_t &result, 
	mengen_t menge, mengen_t offset)
{  mapindex idx(ae.Instanz(),ae.Artikel());
   if (!menge) menge=ae.getStueck();
   if (!!ae.getGeliefert())
   {  if (ae.getGeliefert()<offset)
      {  mengen_t m=AuftragBase::max(menge,ae.getGeliefert()-offset);
         result[idx].geliefert+=m;
      	 menge-=m;
         if (!menge) return;
      }
      offset-=ae.getGeliefert();
   }
   assert(ae.Id()!=AuftragBase::dispo_auftrag_id);
   if (ae.Id()==AuftragBase::plan_auftrag_id || ae.Id()>=AuftragBase::handplan_auftrag_id)
   {  if (ae.Instanz()->LagerInstanz())
      {  result[idx].vorraetig+=ae.getRestStk();
         return;
      }
      else
         result[idx].geplant+=ae.getRestStk();
   }
   else
   {  assert(ae.Id()==AuftragBase::ungeplante_id);
      result[idx].ungeplant+=ae.getRestStk();
   }
   
   // Rekursion
   distribute_children_rev_artbaum(ae,menge,ae.Artikel(),
   		verf_recurse(offset,result,ae.Artikel()));
}
