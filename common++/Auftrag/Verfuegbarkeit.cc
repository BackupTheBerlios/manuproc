/* $Id: Verfuegbarkeit.cc,v 1.1 2003/11/29 12:14:49 christof Exp $ */
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

Verfuegbarkeit::mengen_t Verfuegbarkeit::Mengen::summe() const
{  return  geliefert+ungeplant+geplant+vorraetig+error;
}

namespace
{
struct verf_recurse : distribute_children_artbaum
{	Verfuegbarkeit::mengen_t offset;

	AuftragBase::mengen_t operator()(const ArtikelBase &, 
	                const AufEintragBase &,AuftragBase::mengen_t) const;
	void operator()(const ArtikelBase &,AuftragBase::mengen_t) const;
};
}

AuftragBase::mengen_t verf_recurse::operator()(const ArtikelBase &, 
	                const AufEintragBase &,AuftragBase::mengen_t) const
{
}

void verf_recurse::operator()(const ArtikelBase &,AuftragBase::mengen_t) const
{
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
   {  if (ae.Instanz().LagerInstanz())
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
   distribute_children_artbaum_rev(ae,menge,ae.Artikel(),verf_recurse(offset));
}
