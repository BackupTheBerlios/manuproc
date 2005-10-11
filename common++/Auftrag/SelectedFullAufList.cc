// $Id: SelectedFullAufList.cc,v 1.2 2005/10/11 11:11:07 christof Exp $
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

FetchIStream &operator>>(FetchIStream &is, AufStatVal &v)
{  int i;
   is >> i;
   v=AufStatVal(i);
   return is;
}

FetchIStream &operator>>(FetchIStream &is, cH_PreisListe &v)
{  int i;
   is >> i;
   v=cH_PreisListe(i);
   return is;
}

FetchIStream &operator>>(FetchIStream &is, Preis &v)
{  double p,pm;
   int w;
   int ip,ipm,iw;
   is >> FetchIStream::WithIndicator(p,ip);
   is >> FetchIStream::WithIndicator(pm,ipm);
   is >> FetchIStream::WithIndicator(w,iw);
   if (!ip && !ipm && !iw) v=Preis(p,Waehrung::ID(w),pm);
   else v=Preis();
   return is;
}

FetchIStream &operator>>(FetchIStream &is, AufEintrag &ae)
{  Prozess::ID prozid;

   is >> static_cast<AufEintragBase &>(ae)
   	>> ae.bestellt 
   	>> ae.artikel 
   	>> ae.lieferdatum
   	>> ae.geliefert
	>> ae.auftragstatus 
   	>> ae.kdnr 
   	>> FetchIStream::MapNull(ae.youraufnr,std::string())
   	>> prozid 
   	>> ae.letztePlanInstanz
   	>> ae.maxPlanInstanz 
   	>> ae.prozdate
   	>> ae.preis 
   	>> FetchIStream::MapNull(ae.rabatt,0)
   	>> ae.entrystatus
   	>> ae.lasteditdate_uid
   	>> ae.lasteditdate 
   	>> ae.letzte_lieferung
   	>> ae.preisliste
	>> FetchIStream::MapNull(ae.provsatz,0)
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
   q.FetchArray(aufidliste);
 }
 else
 { Query q(selector.getClausel());
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
