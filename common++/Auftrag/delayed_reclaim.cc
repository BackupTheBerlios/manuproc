// $Id: delayed_reclaim.cc,v 1.5 2004/02/16 10:09:27 christof Exp $
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
#include <Misc/Trace.h>
#include <Misc/Transaction.h>
#include <Misc/FetchIStream.h>
#include <Lager/Lager.h>

bool AufEintrag::delayed_reclaim::active;
std::list<std::pair<cH_ppsInstanz,ArtikelBase> > 
	AufEintrag::delayed_reclaim::delayed;

void AufEintrag::delayed_reclaim::reclaim()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__);
   
   Transaction tr;
   Query::Execute("lock table auftragentry in exclusive mode");
   while (!delayed.empty())
   {  std::pair<cH_ppsInstanz,ArtikelBase> act=delayed.front();
      AuftragBase ab(act.first,dispo_auftrag_id);
      if (act.first->LagerInstanz()) // wegen Lagerdatum sonst Liste
      {  AuftragBase::mengen_t m;
         AufEintragBase neuerAEB(ab,
              ab.existEntry(act.second,LagerBase::Lagerdatum(),OPEN,m));
         // verteilen
         if (neuerAEB.valid())
         {  MengeVormerken(act.first,act.second,m,true,ProductionContext());
            AufEintrag(neuerAEB).MengeAendern(-m,false,AufEintragBase());
         }
      }
      else
      {  // alle 2er suchen
         auf_positionen_verteilen(...)
      }
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
{  delayed.push_back(std::pair<cH_ppsInstanz,ArtikelBase>(inst,ab));
}
