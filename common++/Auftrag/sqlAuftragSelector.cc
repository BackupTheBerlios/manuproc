// $Id: sqlAuftragSelector.cc,v 1.11 2002/02/05 17:15:52 christof Exp $
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

#include<Auftrag/sqlAuftragSelector.h>
#include<Aux/string0.h>
#include <Aux/itos.h>
#include<auftrag_status.h>

#define FULL_SELECTIONS "a.instanz, a.auftragid, e.zeilennr, bestellt, " \
	"e.artikelid, e.rohartikelid, " \
	"e.lieferdate, geliefert, " \
	"a.stat, " \
	"a.kundennr, youraufnr, " \
	"coalesce(p.prozessid,0), " \
	"coalesce(p.letztePlanInstanz,0), " \
   	"coalesce(p.maxPlanInstanz,0), " \
	"date(coalesce(p.datum,now())), " \
	"e.preis, int4(e.rabatt*100), " \
	"coalesce(e.preismenge,1), a.waehrung, " \
	"e.status, e.lasteditdate "

#define FULL_FROM "(auftrag a join auftragentry e using (instanz,auftragid))" \
	" left join auftrag_prozess p" \
	" using (instanz,auftragid,zeilennr) "

#define FULL_SELECT_FROM_WHERE "select " FULL_SELECTIONS \
	" from " FULL_FROM " where true "

#define FULL_SELECT_NO_0 " and bestellt!=0 "

// Vorsicht: enthält keine führenden/abschließenden Leerzeichen
std::string SQLFullAuftragSelector::StatusQualifier(AufStatVal stat)
{
 const std::string status(itos(stat));
 static const std::string nicht_erledigt
 	="not in ("+itos(CLOSED)+","+itos(STORNO)+")";
 
 std::string op="=";
// if(nonstatus) op="!=";
// if(nonstatus && UNCOMMITED) assert(!"Nicht implementert");
 switch (stat) {
   case OPEN       : return "a.stat"+op+status+" and e.status"+op+status;
   case UNCOMMITED : return "(a.stat= "+status+" and e.status "+nicht_erledigt+
                        " or (a.stat "+nicht_erledigt+" and e.status="+status+"))";
   case STORNO     : return "(a.stat"+op+status+" or e.status="+op+status+")";
   case CLOSED     : return "(a.stat="+op+status+" or e.status="+op+status+")";
   }
 assert(!"SQLFullAuftragSelector::StatusQualifier komischer Status");
 return "false";
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Status& selstr)
{
 std::string cl=FULL_SELECT_FROM_WHERE FULL_SELECT_NO_0 " and "
    + StatusQualifier(selstr.status)+
       " and a.instanz="+itos(selstr.instanz);

 if(!selstr.geplant) cl +=" and a.auftragid!=0 ";
 setClausel(cl);

/*
 setClausel(FULL_SELECT_FROM_WHERE " and "
   + StatusQualifier(selstr.status,selstr.nonstatus)+
   " and a.instanz="+itos(selstr.instanz));
*/
}




SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Aufid& selstr)
{
    setClausel(FULL_SELECT_FROM_WHERE
	   " and (a.instanz, a.auftragid)="
	   "("+itos(selstr.auftrag.InstanzID())+", "+itos(selstr.auftrag.Id())+")");
}


SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_AufidZnr& selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	   " and (a.instanz, a.auftragid, e.zeilennr)="
	   "("+itos(selstr.auftrag_znr.InstanzID())+", "
	   +itos(selstr.auftrag_znr.Id())+", "+itos(selstr.auftrag_znr.ZNr())+")");
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Jahr_Artikel &selstr)
{
 const std::string jahr(itos(selstr.jahr));
 std::string artids;
 for (std::vector<ArtikelBase::ID>::const_iterator i=selstr.artikelid.begin();
 		i!=selstr.artikelid.end();++i)
 {  if (i!=selstr.artikelid.begin())  artids+=',';
    artids+=itos(*i);
 }
             
 setClausel(FULL_SELECT_FROM_WHERE
	     " and ("
	     " e.lieferdate between date('"+jahr+"-1-1') and date('"+jahr+"-12-31')) "
	     " and artikelid in ("+artids +
	     ") and a.instanz="+itos(selstr.instanz));
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Kunde_Artikel &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	     " and "+StatusQualifier(OPEN)+
	     " and a.instanz="+itos(selstr.instanz) +
	     " and a.kundennr="+itos(selstr.kundennr) +
	     " and artikelid="+itos(selstr.artikel.Id()) +
	     " order by e.lieferdate");
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Artikel_Planung &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	     " and "+StatusQualifier(OPEN)+
	     " and a.instanz="+itos(selstr.instanz) +
	     " and artikelid="+itos(selstr.artikel.Id()) +
	     " and a.auftragid" + (selstr.geplant ? "<>" : "=") + "0"
	     " order by e.lieferdate");
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Kunde_Status &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE FULL_SELECT_NO_0
	     " and "+StatusQualifier(selstr.stat)+
	     " and a.instanz="+itos(selstr.instanz) +
	     " and a.kundennr=" + itos(selstr.kundennr) +
	     " order by e.lieferdate");
}
