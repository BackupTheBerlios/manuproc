// $Id: sqlAuftragSelector.cc,v 1.6 2001/07/16 09:54:26 christof Exp $
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

#define FULL_SELECTIONS "a.instanz, a.auftragid, e.zeilennr, bestellt," \
	" e.artikelid, e.rohartikelid," \
	" e.lieferdate, geliefert," \
	" a.jahrgang, a.stat," \
	" a.kundennr, youraufnr, coalesce(p.prozessid,0)," \
	" date(coalesce(p.datum,now()))," \
	" e.preis, int4(e.rabatt*100), coalesce(e.preismenge,1), a.waehrung," \
	" e.status, e.lasteditdate "

#define FULL_FROM "(auftrag a join auftragentry e using (instanz,auftragid))" \
	" left join auftrag_prozess p" \
	" on (e.instanz,e.auftragid,e.zeilennr)=(1,p.auftragid,p.zeilennr) "

#define FULL_SELECT_FROM_WHERE "select " FULL_SELECTIONS \
	" from " FULL_FROM  


SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Status& selstr)
{
 const std::string status(itos(selstr.status));
 setClausel(FULL_SELECT_FROM_WHERE
	" where a.stat="+status+
	" and e.status="+status +
	" and a.instanz="+itos(selstr.instanz)
	);	

}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Aufid& selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	" where a.auftragid="+itos(selstr.auftrag.Id())
	+ " and a.instanz="+itos(selstr.auftrag.Instanz()));
}


SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_AufidZnr& selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	" where a.auftragid="+itos(selstr.auftrag_znr.Id())+
	" and a.instanz="+itos(selstr.auftrag_znr.Instanz()) +
	" and e.zeilennr="+itos(selstr.auftrag_znr.ZNr()));
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Jahr_Artikel &selstr)
{
 const std::string jahr(itos(selstr.jahr));

 setClausel(FULL_SELECT_FROM_WHERE
	     " where (a.jahrgang="+jahr+
	     " or e.lieferdate between date('"+jahr+"-1-1') and date('"+jahr+"-12-31')) "
	     " and artikelid="+itos(selstr.artikelid) +
	     " and a.instanz="+itos(selstr.instanz));
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Kunde_Artikel &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	     " where a.stat=" + itos(OPEN) +
	     " and a.instanz="+itos(selstr.instanz) +
	     " and e.status=" + itos(OPEN) +
	     " and a.kundennr=" + itos(selstr.kundennr) +
	     " and artikelid="+itos(selstr.artikelid) +
	     " order by e.lieferdate");
}

// VAR1: and e.instanz=1 and e.auftragid=10033;
