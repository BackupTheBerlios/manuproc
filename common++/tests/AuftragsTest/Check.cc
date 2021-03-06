// $Id: Check.cc,v 1.68 2006/10/31 16:04:21 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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


#include <ManuProCConfig.h>
#include "Check.hh"
#include <Misc/Query.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <unistd.h>
#include "steuerprogramm.hh"
#include <fstream>

static std::string resultdir="results/";
#ifdef  MANU_PROC_TEST
static std::string referenzdir="database_tables_test_ManuProC/";
#elif defined  MABELLA_TEST
static std::string referenzdir="database_tables_test_Mabella/";
#elif defined PETIG_TEST
static std::string referenzdir="database_tables_test/";
#endif

bool Check::analyse=false;
bool Check::reparieren=false;
bool Check::overwrite=false;
bool Check::resort=false;
bool Check::verbose=false;
bool Check::continue_=false;
bool Check::delete_repair=false;
bool Check::check_links=true;

vergleichstream Check::vergleich_open(const std::string &name) const
{  vergleichstream result(name);
   result.stream=new std::ofstream((resultdir+name).c_str());
   return result;
}
bool Check::vergleich_close(const std::string &name)
{  vergleichstream v(name);
   return vergleich_close(v);
}
bool Check::vergleich_close(vergleichstream &v)
{  if (v.stream) 
   {  v.stream->close();
      delete v.stream;
      v.stream=0;
   }
   bool error=false;
   std::string fz1=resultdir+v.name;
     std::string fz2=referenzdir+v.name;

     if (overwrite)      
     {  system(("cp \""+fz1+"\" \""+fz2+"\"").c_str());
        std::cout << fz2<<" wurde neu erzeugt\n"; 
     }
     else
     {  std::string s="diff -q \""+fz1+"\" \""+fz2+"\" >/dev/null";
        if (resort) s="./resort"+s;
        int reg=system(s.c_str());
        if(reg==-1) { std::cout<< "Fehler im diff-Komando ("+v.name+")\n"; exit(reg);}
        else if(reg==0) ;//zuviel Output :-( {std::cout << v.name << " OK\n";}
        else 
        { std::cout << (resort?"./resort":"") << "mgdiff "+fz1+" "+fz2<<"\n"; 
          error=true;
        }
     }
   return error;
}

bool Check::teste(was_checken check,const std::string &zusatz, bool vor_dem_test_reparieren)
{
  if(!analyse && !reparieren) // sonst unlogisch ... !vor_dem_test_reparieren)
   { dump(check,zusatz);  
     if (!vergleich(check,zusatz)) return false;
     if (!analyse && !reparieren) return true;
   }
  
  std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
  Query::Execute("vacuum analyze");
  std::string cmd="../../Programme/auftrag_repair -I -aAXCM -aN";
  if (analyse && !reparieren) cmd+=" -y";
  else cmd+=" -l";
  if (delete_repair) cmd+=" -aD";
  if (check_links) cmd+=" -aL";
  int c=system(cmd.c_str());
  if(verbose || c) 
  { std::cerr << cmd << " returned " << c<<'\n'; 
  }

  // einmal reicht dann  
//  if (!vor_dem_test_reparieren && !reparieren) return true;
  
  dump(check,zusatz);  
  if (c) return false; 
  return vergleich(check,zusatz);
}

bool Check::vergleich(was_checken was,const std::string &zusatz)
{ std::vector<std::string> files;
  bool error=false;

  if (was & Jumbo) 
  {  files.push_back("rohjumbo");
     files.push_back("lager_bewegung");
  }
  if (was & Lieferschein)
  {  files.push_back("lieferschein");
     files.push_back("lieferscheinentry");
     if (overwrite || !access((referenzdir+"lieferscheinentryzusatz_"+zusatz).c_str(),R_OK))
        files.push_back("lieferscheinentryzusatz");
  }
  if (was & Menge)
  {  files.push_back("auftragsentryzuordnung");
     files.push_back("auftragentry");
  }
  if (was & RohLager) 
  {  files.push_back("rl_inhalt");
     files.push_back("rl_log");
  }

  for (std::vector<std::string>::const_iterator i=files.begin();i!=files.end();++i)
  {  error|=vergleich_close(*i+"_"+zusatz);
  }
  return continue_?true:(!error);
}


void Check::dump(was_checken check, const std::string &zusatz)
{
  static const std::string psql_cmd="psql "+std::string(getenv("PGDATABASE"))+" -q -X -c";

  if(check & Jumbo)
  {  system((psql_cmd+" \""+
  	"select code,maschine,lauf,gang,status,wiederinslager,verarb_datum,"
  	  "artikelid,rest,lagerplatz from rohjumbo order by code;"
  	    +"\" >"+resultdir+"rohjumbo_"+zusatz).c_str());
     system((psql_cmd+" \""+
  	"select code,action,name,lagerplatz from lager_bewegung order by code,zeit;"
  	    +"\" >"+resultdir+"lager_bewegung_"+zusatz).c_str());
  }
  if(check & Lieferschein)
  {  system((psql_cmd+" \""+
  	"select instanz,lfrsid,kundennr,coalesce(rngid,-1) as rngid,instanz"
  	  " from lieferschein order by instanz,lfrsid;"
  	    +"\" >"+resultdir+"lieferschein_"+zusatz).c_str());
     system((psql_cmd+" \""+
  	"select instanz,lfrsid,zeile,artikelid,refauftragid,stueck,refzeilennr,menge "
  	   " from lieferscheinentry order by instanz,lfrsid,zeile;"
  	    +"\" >"+resultdir+"lieferscheinentry_"+zusatz).c_str());
     if (overwrite || !access((referenzdir+"lieferscheinentryzusatz_"+zusatz).c_str(),R_OK))
       system((psql_cmd+" \""+
  	"select instanz,lfrsid,lfsznr,auftragid,auftragznr,menge "
  	   " from lieferscheinentryzusatz order by instanz,lfrsid,lfsznr,auftragid,auftragznr;"
  	    +"\" >"+resultdir+"lieferscheinentryzusatz_"+zusatz).c_str());
  }
  
 if (check & Menge)
 {
  std::string s1=psql_cmd+" \"select to_char(e.instanz,'99') ||' - ' "
      "|| to_char(e.auftragid,'99999') || ' - ' || text(zeilennr) as "
      "\\\"instanz-id-znr\\\",bestellt,"
      "geliefert,lieferdate,e.status,a.stat,kundennr,artikelid from auftragentry e"
      ", auftrag a where a.auftragid=e.auftragid and a.instanz=e.instanz "
      "order by 1;\" > "+resultdir+"auftragentry_"+zusatz; 
  system(s1.c_str());

  std::string s2=psql_cmd+" \"select to_char(altinstanz,'99') ||' - ' "
      "|| to_char(altauftragid,'99999') || ' - ' || text(altzeilennr) as "
      "\\\"ALT:instanz-id-znr\\\","
      "to_char(neuinstanz,'99') ||' - ' "
      "|| to_char(neuauftragid,'99999') || ' - ' || text(neuzeilennr) as "
      "\\\"NEU:instanz-id-znr\\\",menge from auftragsentryzuordnung "
      "order by 2,prioritaet,altinstanz,altauftragid,altzeilennr;\" > "+resultdir+"auftragsentryzuordnung_"+zusatz; 
  system(s2.c_str());
 }
  if(check & RohLager)
  {  system((psql_cmd+" \""+
  	"select position_ as pos,material,kartons,kg_per_karton as a,"
  		" reste,rest_kg as kg  from rl_inhalt order by 1;"
  	    +"\" >"+resultdir+"rl_inhalt_"+zusatz).c_str());
     system((psql_cmd+" \""+
  	"select position_ as pos,material,typ,kartons,kg_per_karton as a,"
  		" reste,rest_kg as kg,misc from rl_log order by zeit;"
  	    +"\" >"+resultdir+"rl_log_"+zusatz).c_str());
  }
  if(check & FertigwLager)
  {  system((psql_cmd+" \""+
  	"select artikelid,menge,aktion,bestand,lfrsid from fw_lager_buchung "
  	"order by 1,datum\" >"+resultdir+"fw_lager_buchung_"+zusatz).c_str());
  }
}

Check::was_checken operator|(Check::was_checken a, Check::was_checken b)
{  return Check::was_checken(int(a)|int(b)); }
bool operator&(Check::was_checken a, Check::was_checken b)
{  return int(a)&int(b); }

void Check::NeedsRepair()
{ if (!reparieren)
  { std::cerr << "Reparatur zugeschaltet (wird vom Test benötigt)\n";
    reparieren=true;
  }
}
