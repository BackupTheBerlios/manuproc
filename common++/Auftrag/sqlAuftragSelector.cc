// $Id: sqlAuftragSelector.cc,v 1.2 2001/06/06 07:27:39 christof Exp $
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

#define NORMAL_SELECTIONS " a.auftragid, e.zeilennr, a.kundennr "
#define NORMAL_FROM " auftrag a join auftragentry e using (auftragid) "
//#define NORMAL_WHERE " a.auftragid=e.auftragid "
#define NORMAL_SELECT_FROM_WHERE "select " NORMAL_SELECTIONS \
 	     " from " NORMAL_FROM 

SQLAuftragSelector::SQLAuftragSelector()
{
 char tmp[200];
 snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " order by e.auftragid,zeilennr");
 clausel=tmp;
}

SQLAuftragSelector::SQLAuftragSelector(const sel_Aufid& selstr)
{
 char tmp[200];
 snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " where a.auftragid=%d order by zeilennr",selstr.aufid);
 clausel=tmp;
}

SQLAuftragSelector::SQLAuftragSelector(const sel_Jahr& selstr,char *order=0)
{
 char tmp[200];
 if(order)
   snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " where a.jahrgang=%d order by %s,a.auftragid",selstr.jahrgang,order);
 else
   snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " where a.jahrgang=%d order by a.auftragid,zeilennr",selstr.jahrgang);
 clausel=tmp;
}

SQLAuftragSelector::SQLAuftragSelector(const sel_Status& selstr, int aufid=0)
{
 char tmp[500];
 if(aufid)
   snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " where a.status=%d and e.bestellt > e.geliefert"
	     " and lastedit not in ('storno','fertig') and a.auftragid=%d",
		selstr.status, aufid);
 else
   snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " where a.status=%d and e.bestellt > e.geliefert"
	     " and lastedit not in ('storno','fertig') order by e.auftragid",
		selstr.status);
 clausel = tmp;
}

SQLAuftragSelector::SQLAuftragSelector(const sel_Status_Mab& selstr)
{
 char tmp[500];
 snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " where e.bestellt > e.geliefert"
	     " and a.status='%s' order by e.auftragid",
		selstr.status.c_str());
 clausel = tmp;
}

SQLAuftragSelector::SQLAuftragSelector(const sel_Status& selstr, char *order)
{
 char tmp[500];
 if(order)
 snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " order by %s, e.auftragid", order);
 else
 snprintf0(tmp,sizeof tmp,NORMAL_SELECT_FROM_WHERE
	     " order by e.auftragid");
 clausel = tmp;
}

#define FULL_SELECTIONS "a.auftragid, e.zeilennr, bestellt," \
	" e.artikelid, e.rohartikelid," \
	" e.lieferdate, geliefert," \
	" a.jahrgang, a.stat," \
	" a.kundennr, youraufnr, coalesce(p.prozessid,0)," \
	" date(coalesce(p.datum,now()))," \
	" e.preis, int4(e.rabatt*100), coalesce(e.preismenge,1), a.waehrung," \
	" e.status, e.lasteditdate "

#define FULL_FROM "auftrag a join " \
	" (auftragentry e left join auftrag_prozess p using(auftragid,zeilennr))" \
	" using (auftragid) "

//#define FULL_WHERE " a.auftragid=e.auftragid "

#define FULL_SELECT_FROM_WHERE "select " FULL_SELECTIONS \
	" from " FULL_FROM  

// SQLFullAuftragSelector

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Status& selstr)
{
 const string status(itos(selstr.status));
 setClausel(FULL_SELECT_FROM_WHERE
	" where a.stat= "+status+
	" and e.status= "+status
	);	

}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Aufid& selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	" where a.auftragid="+itos(selstr.aufid));
}


SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_AufidZnr& selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	" where a.auftragid="+itos(selstr.aufid)+
	" and e.zeilennr="+itos(selstr.znr));
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Jahr_Artikel &selstr)
{
 const string jahr(itos(selstr.jahr));

 setClausel(FULL_SELECT_FROM_WHERE
	     " where (a.jahrgang="+jahr+
	     " or e.lieferdate between date('"+jahr+"-1-1') and date('"+jahr+"-12-31')) "
	     " and artikelid="+itos(selstr.artikelid));
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Kunde_Artikel &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	     " where a.stat=" + itos(OPEN) +
	     " and e.status=" + itos(OPEN) +
	     " and a.kundennr=" + itos(selstr.kundennr) +
	     " and artikelid="+itos(selstr.artikelid) +
	     " order by e.lieferdate");
}

