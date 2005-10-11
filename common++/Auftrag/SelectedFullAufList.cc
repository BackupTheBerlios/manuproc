// $Id: SelectedFullAufList.cc,v 1.3 2005/10/11 15:16:10 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <Auftrag/SelectedFullAufList.h>
#include <Misc/Transaction.h>
#include <Misc/Datum.h>

Query::Row &operator>>(Query::Row &is, AufStatVal &v)
{  int i;
   is >> i;
   v=AufStatVal(i);
   return is;
}

Query::Row &operator>>(Query::Row &is, cH_PreisListe &v)
{  int i;
   is >> i;
   v=cH_PreisListe(i);
   return is;
}

Query::Row &operator>>(Query::Row &is, Preis &v)
{  double p,pm;
   int w;
   int ip; // indicator
   is >> Query::Row::WithIndicator(p,ip);
   is >> Query::Row::MapNull(pm,1);
   // do I really want this default?
   is >> Query::Row::MapNull(w,Waehrung::default_id);
   if (!ip) v=Preis(p,Waehrung::ID(w),pm);
   else v=Preis();
   return is;
}

Query::Row &operator>>(Query::Row &is, AufEintrag &ae)
{  Prozess::ID prozid;

   is >> static_cast<AufEintragBase &>(ae)
   	>> ae.bestellt 
   	>> ae.artikel 
   	>> ae.lieferdatum
   	>> ae.geliefert
	>> ae.auftragstatus 
   	>> ae.kdnr 
   	>> Query::Row::MapNull(ae.youraufnr,std::string())
   	>> Query::Row::MapNull(prozid,ProzessID::None)
   	>> Query::Row::MapNull(ae.letztePlanInstanz,ppsInstanzID::None)
   	>> Query::Row::MapNull(ae.maxPlanInstanz,ppsInstanzID::None)
   	>> Query::Row::MapNull(ae.prozdate)
   	>> ae.preis 
   	>> Query::Row::MapNull(ae.rabatt,0)
   	>> ae.entrystatus
   	>> Query::Row::MapNull(ae.lasteditdate_uid)
   	>> ae.lasteditdate 
   	>> ae.letzte_lieferung
   	>> Query::Row::MapNull(ae.preisliste,ManuProcEntity<>::none_id)
	>> Query::Row::MapNull(ae.provsatz,0)
	>> ae.am_lager;

   ae.prozess=cH_Prozess(prozid ? prozid : cH_Prozess::default_pid);
   if(! prozid) ae.prozdate=ManuProC::Datum();
   return is;
}

SelectedFullAufList::SelectedFullAufList
	(const SQLFullAuftragSelector &selector) throw(SQLerror)
{
 if(!selector.getPreQuery().empty())
   Query(selector.getPreQuery());

 if (selector.many_lines()) 
 { Transaction tr;
   Query q("auftrag",selector.getClausel());
   q << selector.getArguments();
   q.FetchArray(aufidliste);
 }
 else
 { Query q(selector.getClausel());
   q << selector.getArguments();
   q.FetchArray(aufidliste);
 }

 if(!selector.getPostQuery().empty())
   Query(selector.getPostQuery());
}


void SelectedFullAufList::insert(const AufEintragBase& aeb) throw(SQLerror)
{	SQLFullAuftragSelector sel((SQLFullAuftragSelector::sel_AufidZnr(aeb)));
        Query q(sel.getClausel()); 
        q.FetchArray(aufidliste);
}

void SelectedFullAufList::Restrict(const AuftragBase &rest)
{ for (iterator i=aufidliste.begin(); i!=aufidliste.end(); )
  { if (!(static_cast<const AuftragBase &>(*i)==rest))
      i=aufidliste.erase(i);
    else ++i;
  }
}
