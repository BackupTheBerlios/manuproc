// $Id: delayed_reclaim.cc,v 1.12 2006/06/26 07:53:01 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski & Christof Petig
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

#include <Auftrag/AufEintrag_delayedreclaim.h>
#include <Misc/TraceNV.h>
#include <Misc/Transaction.h>
#include <Misc/Query.h>
#include <Lager/Lager.h>
#include <Auftrag/VerfuegbareMenge.h>
//#include <Auftrag/selFullAufEntry.h>
#include <Misc/relops.h>
//#include <Auftrag/AufEintragZu.h>
#include <Misc/TemporaryValue.h>
#include <Auftrag/sqlAuftragSelector.h>

bool AufEintrag::delayed_reclaim::active;
bool AufEintrag::delayed_reclaim::reclaiming;
std::list<std::pair<cH_ppsInstanz,ArtikelBase> > 
	AufEintrag::delayed_reclaim::delayed;

// freie Menge nutzen
class MengeNutzen_2er_Lager_cb2 : public distribute_parents_cb
{	AufEintrag &nuller;
	VerfuegbareMenge &vfm;
	
public:
	AuftragBase::mengen_t operator()(const AufEintragBase &elter,AuftragBase::mengen_t m) const
	{  m=vfm.reduce_in_dispo(m,elter);
	   nuller.MengeAendern(-m,true,elter);
	   return m;
	}
	MengeNutzen_2er_Lager_cb2(AufEintrag &_nuller,VerfuegbareMenge &_vfm) 
		: nuller(_nuller), vfm(_vfm) {}
};

class MengeNutzen_2er_Lager_cb : public auf_positionen_verteilen_cb
{	VerfuegbareMenge &vfm;
public:
	MengeNutzen_2er_Lager_cb(VerfuegbareMenge &_vfm) : vfm(_vfm) {}
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t m) const
	{  vfm.setzeDatum(ae.getLieferdatum());
	   distribute_parents(ae,m,MengeNutzen_2er_Lager_cb2(ae,vfm));
	   return m;
	}
};

void AufEintrag::delayed_reclaim::reclaim(AufEintrag &zweier, mengen_t menge)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__, zweier, menge);
   // erstmal nur so gedacht ...
   assert(zweier.Instanz()->LagerInstanz());
   // siehe auch unten
   VerfuegbareMenge vfm(zweier);
   if (!vfm.getMengeDispo()) return;

   SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::
      		sel_Artikel_Planung_id(zweier.Instanz()->Id(),Kunde::eigene_id,
      		            zweier.Artikel(),ungeplante_id)));
   auf_positionen_verteilen(sel,vfm.getMengeDispo(),
               MengeNutzen_2er_Lager_cb(vfm));   
}

void AufEintrag::delayed_reclaim::reclaim()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__);
   
   TemporaryValue<bool>(reclaiming,true);
   Transaction tr;
   Query::Execute("lock table auftragentry in exclusive mode");
   while (!delayed.empty())
   {  std::pair<cH_ppsInstanz,ArtikelBase> act=delayed.front();
      AuftragBase ab(act.first,dispo_id);
      if (act.first->LagerInstanz()) // wegen Lagerdatum sonst Liste
      {  // 0er suchen, dann von verfügbarer Menge abziehen
         VerfuegbareMenge vfm(act.first,act.second,LagerBase::Lagerdatum());
         if (vfm.getDispoAuftraege().empty() || !vfm.getMengeDispo())
           goto continue_loop;
         // kopiert aus MengeVormerken
         SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::
      		sel_Artikel_Planung_id(act.first->Id(),Kunde::eigene_id,act.second,
      				ungeplante_id)));
         auf_positionen_verteilen(sel,vfm.getMengeDispo(),
               MengeNutzen_2er_Lager_cb(vfm));
      }
      else
      {  // alle 2er suchen
         VerfuegbareMenge vfm(act.first,act.second,ManuProC::Datum::Infinity());
         // reduce in dispo?
         for (VerfuegbareMenge::iterator i=vfm.getDispoAuftraege().begin();
         		std_neq(i,vfm.getDispoAuftraege().end());++i)
         {  if (!i->getRestStk()) continue;
            AufEintrag::MengeNutzen_2er(*i);
         }
      }
    continue_loop:
      delayed.pop_front();
   }
   tr.commit();
}

AufEintrag::delayed_reclaim::delayed_reclaim()
  : nested(active)
{  if (!nested) active=true;
}

AufEintrag::delayed_reclaim::~delayed_reclaim()
{  if (!nested) 
   { reclaim(); active=false; }
}

void AufEintrag::delayed_reclaim::delay(cH_ppsInstanz inst,const ArtikelBase &ab)
{  assert(!reclaiming); // you cannot queue while reclaiming
   delayed.push_back(std::pair<cH_ppsInstanz,ArtikelBase>(inst,ab));
}
