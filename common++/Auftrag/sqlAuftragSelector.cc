// $Id: sqlAuftragSelector.cc,v 1.29 2003/06/18 10:17:57 jacek Exp $
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
#include<Misc/string0.h>
#include <Misc/itos.h>
#include<auftrag_status.h>
#include<BaseObjects/ManuProcEntity.h>

#include<DynamicEnums/DynamicEnums.h>



#define FULL_SELECTIONS "a.instanz, a.auftragid, e.zeilennr, bestellt, " \
	"e.artikelid, " \
	"e.lieferdate, geliefert, " \
	"a.stat, " \
	"a.kundennr, youraufnr, " \
	"coalesce(p.prozessid,"+itos(ProzessID::None)+"), " \
	"coalesce(p.letzteplaninstanz,"+itos(ppsInstanzID::None)+"), " \
   	"coalesce(p.maxplaninstanz,"+itos(ppsInstanzID::None)+"), " \
	"coalesce(p.datum,cast(now() as date)), " \
	"e.preis, coalesce(e.preismenge,1), a.waehrung, " \
	"coalesce(e.rabatt,0.0), " \
	"e.status,coalesce(e.lastedit_uid,0),e.lasteditdate," \
	"e.letzte_lieferung," \
	"coalesce(e.preisliste,"+itos(ManuProcEntity<>::none_id)+"), " \
	"e.provsatz "

#define FULL_FROM "(auftrag a join auftragentry e using (instanz,auftragid))" \
	" left join auftrag_prozess p" \
	" using (instanz,auftragid,zeilennr) "
	
#ifdef MABELLA_EXTENSIONS	
#define FULL_FROM_SORT(s) "(auftrag a join auftragentry e using (instanz,auftragid))" \
	" left join auftrag_prozess p" \
	" using (instanz,auftragid,zeilennr) " \
	" left join "+s+" ab on (artikelid=id) "
	
#define FULL_SELECT_FROM_SORT_WHERE(s) "select " FULL_SELECTIONS \
	" from " FULL_FROM_SORT(s) " where true "	
	
#define SORT_ORDER " order by ab.artikel,ab.breite,ab.farbe,ab.aufmachung ";	
#endif


#define FULL_SELECT_FROM_WHERE "select " FULL_SELECTIONS \
	" from " FULL_FROM " where true "

//#define FULL_SELECT_NO_0 " and bestellt!=0 "
#define FULL_SELECT_NO_STORNO " and e.status!="+itos(STORNO)+" "

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
   case OPEN       : return "a.stat=e.status and e.status"+op+status;
   case UNCOMMITED : return "(a.stat= "+status+" and e.status "+nicht_erledigt+
                        " or (a.stat "+nicht_erledigt+" and e.status="+status+"))";
   case STORNO     : return "(a.stat"+op+status+" or e.status"+op+status+")";
   case CLOSED     : return "(a.stat"+op+status+" or e.status"+op+status+")";
   case NOSTAT     : ;
   }
 assert(!"SQLFullAuftragSelector::StatusQualifier komischer Status");
 return "false";
}

std::string SQLFullAuftragSelector::IDQualifier(AuftragBase::ID id)
{
  std::string s;
  if(id==AuftragBase::none_id) return s;
  else if (id==AuftragBase::ungeplante_id || id==AuftragBase::dispo_auftrag_id)
      s+="auftragid="+itos(id);
  else if(id==AuftragBase::plan_auftrag_id) 
      s+="(auftragid!="+itos(AuftragBase::ungeplante_id)+" and"
         " auftragid!="+itos(AuftragBase::dispo_auftrag_id)+")";
  else assert(!"SQLFullAuftragSelector::IDQualifier komische ID");
  return " and "+s+" ";
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Status& selstr)
{
 std::string cl=FULL_SELECT_FROM_WHERE 
    + IDQualifier(selstr.id) +" and "
    + StatusQualifier(selstr.status)+
   " and a.instanz="+itos(selstr.instanz);

// if(!selstr.geplant) cl +=" and a.auftragid!=0 ";
 setClausel(cl);
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Aufid& selstr,
		const std::string artbez_table)
{
//cout << "\n\n\nSTORNO = "<<selstr.with_storno<<"\n\n\n";
#ifdef MABELLA_EXTENSIONS
   std::string s= FULL_SELECT_FROM_SORT_WHERE(artbez_table);
   if(!selstr.with_storno) s+=FULL_SELECT_NO_STORNO;
	s+=" and (a.instanz, a.auftragid)="
	   "("+itos(selstr.auftrag.InstanzID())+", "+itos(selstr.auftrag.Id())+")";
	s+=SORT_ORDER;

    setClausel(s);
#else
   std::string s= FULL_SELECT_FROM_WHERE;
   if(!selstr.with_storno) s+=FULL_SELECT_NO_STORNO;
	 s+=" and (a.instanz, a.auftragid)="
	    "("+itos(selstr.auftrag.InstanzID())+", "+itos(selstr.auftrag.Id())+")";
    setClausel(s);
#endif	   
}


SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_AufidZnr& selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	   " and (a.instanz, a.auftragid, e.zeilennr)="
	   "("+itos(selstr.auftrag_znr.InstanzID())+", "
	   +itos(selstr.auftrag_znr.Id())+", "+itos(selstr.auftrag_znr.ZNr())+")");
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_InstanzAlle& selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	   " and a.instanz="+itos(selstr.instanz));
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

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Artikel_Planung_id &selstr)
{
  std::string clau=FULL_SELECT_FROM_WHERE
           " and "+StatusQualifier(selstr.status)+
	        " and a.instanz="+itos(selstr.instanz) +
           " and kundennr="+itos(selstr.kunde) +
   	     " and artikelid="+itos(selstr.artikel.Id());
  if(selstr.lieferdatum.valid())
      clau+= " and lieferdate="+selstr.lieferdatum.postgres_null_if_invalid();


  if(selstr.status==OPEN)
   {
    if(selstr.auftragid!=AuftragBase::handplan_auftrag_id)
     {
       clau+=" and a.auftragid="+itos(selstr.auftragid) +
             " order by e.lieferdate, e.auftragid , e.zeilennr  ";
     }
    else
     {
       clau+=" and a.auftragid>="+itos(AuftragBase::handplan_auftrag_id) + 
	          " order by e.lieferdate, e.auftragid , e.zeilennr ";
     }
   }
  else if(selstr.status==CLOSED)
   {
       clau+=" and a.auftragid="+itos(selstr.auftragid) + 
	          " order by e.lieferdate desc";
  }
//cout << "CLAUSEL:"<<clau<<"\n\n";
 setClausel(clau);
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Artikel &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE 
	     " and a.instanz="+itos(selstr.instanz) +
	     " and artikelid="+itos(selstr.artikel.Id()));
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Kunde_Status &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE 
        " and bestellt!=0 "
	     " and "+StatusQualifier(selstr.stat)+
	     " and a.instanz="+itos(selstr.instanz) +
	     " and a.kundennr=" + itos(selstr.kundennr) +
	     " order by e.lieferdate");
}
