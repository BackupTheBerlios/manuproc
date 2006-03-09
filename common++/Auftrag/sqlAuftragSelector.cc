// $Id: sqlAuftragSelector.cc,v 1.51 2006/03/09 21:15:04 christof Exp $
/*  libcommonc++: ManuProC's main OO library 
 *  Copyright (C) 1998-2005 Adolf Petig GmbH & Co. KG, 
 *  written by Jacek Jakubowski
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

#include <Auftrag/sqlAuftragSelector.h>
#include <Misc/string0.h>
#include <Misc/itos.h>
#include <auftrag_status.h>
#include <BaseObjects/ManuProcEntity.h>
#include <Misc/Waehrung.h>
#include <Misc/Date.h>
#include <DynamicEnums/DynamicEnums.h>



#define FULL_SELECTIONS_BASE0 "a.instanz,a.auftragid,e.zeilennr, " \
        "bestellt, " \
	"e.artikelid, " \
	"e.lieferdate, geliefert, " \
	"a.stat, " \
	"a.kundennr,youraufnr, "
#define PROZESS_BASE "p.prozessid,p.letzteplaninstanz," \
   	"p.maxplaninstanz,p.datum, " 
#define PROZESS_FAKE "null,null,null,null, "
#define FULL_SELECTIONS_BASE1 \
        "e.preis,e.preismenge,a.waehrung,e.rabatt, " \
	"e.status, " \
	"e.lastedit_uid,e.lasteditdate,e.letzte_lieferung, " \
	"e.preisliste, " \
	"e.provsatz "

#define FULL_SELECTIONS_BASE FULL_SELECTIONS_BASE0 \
        PROZESS_BASE \
        FULL_SELECTIONS_BASE1
#define FULL_SELECTIONS_BASE_NOPR FULL_SELECTIONS_BASE0 \
        PROZESS_FAKE \
        FULL_SELECTIONS_BASE1

#define FULL_SELECTIONS FULL_SELECTIONS_BASE",0" 
#define FULL_SELECTIONS_NOPR FULL_SELECTIONS_BASE_NOPR",0" 

#define FULL_FROM "(auftrag a join auftragentry e using (instanz,auftragid))" \
	" left join auftrag_prozess p" \
	" using (instanz,auftragid,zeilennr) "
#define FULL_FROM_NOPR "(auftrag a join auftragentry e using (instanz,auftragid))" 
	
#ifdef MABELLA_EXTENSIONS	
#define FULL_FROM_SORT(s) "(auftrag a join auftragentry e using (instanz,auftragid))" \
	" left join auftrag_prozess p" \
	" using (instanz,auftragid,zeilennr) " \
	" left join "+s+" ab on (artikelid=id) "
	
#define FULL_SELECT_FROM_SORT_WHERE(s) "select " FULL_SELECTIONS \
	" from " FULL_FROM_SORT(s) " where true "	
	
#define SORT_ORDER " order by ab.artikel,atoi(ab.breite),ab.farbe,ab.aufmachung,e.lieferdate"

#endif


#define WARENGRUPPE_JOIN " join artikelstamm ast on (ast.id=e.artikelid) "

#define FULL_SELECT_FROM_WHERE2(s) "select " FULL_SELECTIONS \
	" from " FULL_FROM s " where true "

#define FULL_SELECT_FROM_WHERE "select " FULL_SELECTIONS \
	" from " FULL_FROM " where true "
	
#define FULL_SELECT_FROM_WHERE_NOPR "select " FULL_SELECTIONS_NOPR \
	" from " FULL_FROM_NOPR " where true "


#define FULL_SELECT_NO_0 " and bestellt!=0 "
#define FULL_SELECT_NO_STORNO " and e.status!="+itos(STORNO)+" "

// Vorsicht: enth�t keine fhrenden/abschlie�nden Leerzeichen
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

// 2do: arguments?
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
 std::string cl;

 if(selstr.wg!=ArtikelTyp::none_id)
   cl=FULL_SELECT_FROM_WHERE2(WARENGRUPPE_JOIN);
 else
   cl=FULL_SELECT_FROM_WHERE; 

  cl+=FULL_SELECT_NO_0;

  cl+= IDQualifier(selstr.id) +" and "
    + StatusQualifier(selstr.status)+
   " and a.instanz=?";
  arguments << selstr.instanz;

 if(selstr.wg!=ArtikelTyp::none_id)
 { cl+=" and ast.warengruppe=?";
   arguments << selstr.wg;
 }

// if(!selstr.geplant) cl +=" and a.auftragid!=0 ";
 setClausel(cl);
 many=true;
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Aufid& selstr,
		const std::string artbez_table)
{
#ifdef MABELLA_EXTENSIONS
  std::string s= FULL_SELECT_FROM_SORT_WHERE(artbez_table);
  setOrderClausel(SORT_ORDER);
#else
  std::string s= FULL_SELECT_FROM_WHERE;
#endif	   
  if(!selstr.with_storno) s+=FULL_SELECT_NO_STORNO;
  s+=" and (a.instanz, a.auftragid)=(?,?)";
  arguments << selstr.auftrag.InstanzID() << selstr.auftrag.Id();
  setClausel(s);
}


SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_AufidZnr& selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	   " and (a.instanz, a.auftragid, e.zeilennr)=(?,?,?)");
 arguments << selstr.auftrag_znr;
 prepnum=idx_AufidZnr;
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Jahr_Artikel &selstr)
{
 std::string artids;
 for (std::vector<ArtikelBase::ID>::const_iterator i=selstr.artikelid.begin();
 		i!=selstr.artikelid.end();++i)
 {  if (i!=selstr.artikelid.begin())  artids+=',';
    artids+='?';
    arguments << *i;
 }
             
 setClausel(FULL_SELECT_FROM_WHERE
	     " and artikelid in ("+artids +")"
	     " and (e.lieferdate between ? and ?)"
	     " and a.instanz=?");
  arguments << ManuProC::Date(1,1,selstr.jahr) 
      << ManuProC::Date(31,12,selstr.jahr);
  arguments << selstr.instanz;
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Kunde_Artikel &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE
	     " and "+StatusQualifier(OPEN)+
	     " and a.instanz=? and a.kundennr=? and artikelid=?");
 arguments << selstr.instanz << selstr.kundennr << selstr.artikel;
 prepnum=idx_Kunde_Art;
 setOrderClausel(" order by e.lieferdate");
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Artikel_Planung_id &selstr)
{
  std::string clau=FULL_SELECT_FROM_WHERE;
  if (selstr.noprocess) clau=FULL_SELECT_FROM_WHERE_NOPR;
  clau+=" and "+StatusQualifier(selstr.status)+
	        " and (a.instanz,kundennr,artikelid)=(?,?,?)";
  arguments << selstr.instanz << selstr.kunde << selstr.artikel;
  if(selstr.lieferdatum.valid())
  { clau+= " and lieferdate=?";
    arguments << selstr.lieferdatum;
  }

  if(selstr.status==OPEN)
  {
    if(selstr.auftragid!=AuftragBase::handplan_auftrag_id)
    {
       clau+=" and a.auftragid=?";
       arguments << selstr.auftragid;
    }
    else
    {
       clau+=" and a.auftragid>=?";
       arguments << AuftragBase::handplan_auftrag_id;
    }
    setOrderClausel(" order by e.lieferdate, e.auftragid , e.zeilennr");
  }
  else if(selstr.status==CLOSED)
  { clau+=" and a.auftragid=?";
    arguments << selstr.auftragid;
    setOrderClausel(" order by e.lieferdate desc");
  }
  setClausel(clau);
  // prepare most common case
  if (!selstr.lieferdatum.valid() && selstr.status==OPEN 
      && selstr.auftragid!=AuftragBase::handplan_auftrag_id
      && selstr.noprocess)
    prepnum=idx_Art_Plan_Id;
}

// obseleted sel_InstanzAlle
SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Artikel &selstr)
{ std::string cl=FULL_SELECT_FROM_WHERE;
  if (selstr.instanz!=ppsInstanzID::None)
  { cl+=" and a.instanz=?";
    arguments << selstr.instanz;
  }
  if (!!selstr.artikel)
  { cl+=" and e.artikelid=?";
    arguments << selstr.artikel;
  }
  else many=true;
  setClausel(cl);
}

SQLFullAuftragSelector::SQLFullAuftragSelector(const sel_Kunde_Status &selstr)
{
 setClausel(FULL_SELECT_FROM_WHERE 
	FULL_SELECT_NO_0
	     " and "+StatusQualifier(selstr.stat)+
	     " and a.instanz=? and a.kundennr=?");
 arguments << selstr.instanz << selstr.kundennr;
 setOrderClausel(" order by e.lieferdate");
}


#ifdef MABELLA_EXTENSIONS
SQLFullAuftragSelector::SQLFullAuftragSelector(
			const sel_Kunde_Status_Lager &selstr)
{
// PRE QUERY
 std::string tmptable("aktbestand_");
 tmptable+=itos(getpid())+"_"+itos(getuid());

 std::string query("create temp table ");
 query+=tmptable+" as (select * from "+selstr.lager.ViewTabelle()+")";
 pre_query.push_back(query);
 pre_query.push_back("create unique index temp_uniq on "+tmptable+" (artikelid)");

// MAIN QUERY
 query="select ";
 query+= std::string(FULL_SELECTIONS_BASE)+",best.bestand "+
	" from "+FULL_FROM+" left join "+
	tmptable+" best "
//	selstr.lager.ViewTabelle()+" best "
	" using (artikelid) where true "
        " and bestellt!=0 "
	     " and "+StatusQualifier(selstr.stat)+
	     " and a.instanz=? and a.kundennr=?";
 arguments << selstr.instanz << selstr.kundennr;
 setClausel(query);
 setOrderClausel(" order by e.lieferdate");

// POST QUERY

 query="drop table "+tmptable;
 post_query.push_back(query);
}
#endif
