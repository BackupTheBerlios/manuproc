// $Id: Check.cc,v 1.41 2003/01/08 09:53:49 christof Exp $
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
#include <Misc/FetchIStream.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <unistd.h>
#include "steuerprogramm.hh"

static std::string resultdir="results/";
#ifdef  MANU_PROC_TEST
static std::string referenzdir="database_tables_test_ManuProC/";
#elif defined  MABELLA_TEST
static std::string referenzdir="database_tables_test_Mabella/";
#elif defined PETIG_TEST
static std::string referenzdir="database_tables_test/";
#endif

bool Check::teste(was_checken check,const std::string &zusatz, bool mit_reparatur_programm,bool vor_dem_test_reparieren)
{
  dump(check,zusatz);  
  if(!vor_dem_test_reparieren)
   {
     bool b=vergleich(check,zusatz);
     if(!mit_reparatur_programm || !b) return b;
   }
  
  std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
  Query::Execute("vacuum analyze");
  std::string Com="../../Programme/adjust_store -y -d "+std::string(getenv("PGDATABASE"));
  for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
   {
     if((*i)->KundenInstanz())
      {
        std::string com=Com+" -i "+itos((*i)->Id())+" -a K ";
        system(com.c_str());
      }
     if((*i)->KundenInstanz()) continue;

     if((*i)->LagerInstanz() && (*i)->EigeneLagerKlasseImplementiert())
      {
        std::string com=Com+" -i "+itos((*i)->Id())+" -a A";
        system(com.c_str());
      }
     std::string com=Com+" -i "+itos((*i)->Id())+" -a B";
     system(com.c_str());

     std::string com2=Com+" -i "+itos((*i)->Id())+" -a C";
     system(com2.c_str());
     std::string comD=Com+" -i "+itos((*i)->Id())+" -a D  ";
     int d=system(comD.c_str());
     std::string comE=Com+" -i "+itos((*i)->Id())+" -a E  ";
     int e=system(comE.c_str());
     std::string comF=Com+" -i "+itos((*i)->Id())+" -a F  ";
     int f=system(comF.c_str());
     std::string comG=Com+" -i "+itos((*i)->Id())+" -a G  ";
     int g=system(comG.c_str());
     int h=0;
     if((*i)->LagerInstanz())
      {
        std::string comH=Com+" -i "+itos((*i)->Id())+" -a H  ";
        h=system(comH.c_str());
      }
     std::string comS=Com+" -i "+itos((*i)->Id())+" -a S  ";
     int s=system(comS.c_str());
     std::string comT=Com+" -i "+itos((*i)->Id())+" -a T  ";
     int t=system(comT.c_str());
     if(d||e||f||g||h||s||t) 
         {std::cout <<*i<<'\t'<< d<<e<<f<<g<<h<<s<<t<<'\n'; return false; }
   }  
  dump(check,zusatz);  
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
  {  std::string fz1=resultdir+*i+"_"+zusatz;
     std::string fz2=referenzdir+*i+"_"+zusatz;
      
#ifdef CREATE_TEST_DATABASE
#warning WARNUNG: 
#warning WARNUNG: COMPILATION MIT CREATE_TEST_DATABASE
#warning WARNUNG: 
    system(("mv "+fz1+" "+fz2).c_str());
    std::cout << fz2<<" wurde neu erzeugt\n"; 
#else
     std::string s="diff -q "+fz1+" "+fz2;
//cout<<" DIFF: " << s<<'\n';
     int reg=system(s.c_str());
     if(reg==-1) { std::cout<< "Fehler im diff-Komando ("+*i+")\n"; exit(reg);}
     else if(reg==0) ;//zuviel Output :-( {std::cout << *i << " OK\n";}
     else 
      { std::cout << "Probleme, Empfehlung: \n "<< "mgdiff "+fz1+" "+fz2<<" &\n"; 
        error=true;
      }
#endif
  }
  return !error;
}


void Check::dump(was_checken check, const std::string &zusatz)
{
  static const std::string psql_cmd="psql "+std::string(getenv("PGDATABASE"))+" -q -X -c";

  if(check & Jumbo)
  {  system((psql_cmd+" \""+
  	"select code,maschine,lauf,gang,status,wiederinslager,verarb_datum,"
  	  "artikelid,rest,lagerplatz from rohjumbo order by code;"
  	    +"\" >"+resultdir+"rohjumbo"+"_"+zusatz).c_str());
     system((psql_cmd+" \""+
  	"select code,action,name,lagerplatz from lager_bewegung order by code,zeit;"
  	    +"\" >"+resultdir+"lager_bewegung"+"_"+zusatz).c_str());
  }
  if(check & Lieferschein)
  {  system((psql_cmd+" \""+
  	"select instanz,lfrsid,kundennr,coalesce(rngid,-1) as rngid,instanz"
  	  " from lieferschein order by instanz,lfrsid;"
  	    +"\" >"+resultdir+"lieferschein"+"_"+zusatz).c_str());
     system((psql_cmd+" \""+
  	"select instanz,lfrsid,zeile,artikelid,refauftragid,stueck,refzeilennr,menge "
  	   " from lieferscheinentry order by instanz,lfrsid,zeile;"
  	    +"\" >"+resultdir+"lieferscheinentry"+"_"+zusatz).c_str());
     system((psql_cmd+" \""+
  	"select instanz,lfrsid,lfsznr,auftragid,auftragznr,menge "
  	   " from lieferscheinentryzusatz order by instanz,lfrsid,lfsznr,auftragid,auftragznr;"
  	    +"\" >"+resultdir+"lieferscheinentryzusatz"+"_"+zusatz).c_str());
  }
  
 if (check & Menge)
 {
  std::string s1=psql_cmd+" \"select to_char(e.instanz,'99') ||' - ' "
      "|| to_char(e.auftragid,'99999') || ' - ' || text(zeilennr) as "
      "\\\"instanz-id-znr\\\",bestellt,"
      "geliefert,lieferdate,e.status,a.stat,kundennr,artikelid from auftragentry e"
      ", auftrag a where a.auftragid=e.auftragid and a.instanz=e.instanz "
      "order by 1;\" > "+resultdir+"auftragentry"+"_"+zusatz; 
  system(s1.c_str());

  std::string s2=psql_cmd+" \"select to_char(altinstanz,'99') ||' - ' "
      "|| to_char(altauftragid,'99999') || ' - ' || text(altzeilennr) as "
      "\\\"ALT:instanz-id-znr\\\",
      to_char(neuinstanz,'99') ||' - ' "
      "|| to_char(neuauftragid,'99999') || ' - ' || text(neuzeilennr) as "
      "\\\"NEU:instanz-id-znr\\\",menge from auftragsentryzuordnung "
      "order by 1,2;\" > "+resultdir+"auftragsentryzuordnung"+"_"+zusatz; 
  system(s2.c_str());
 }
  if(check & RohLager)
  {  system((psql_cmd+" \""+
  	"select position_ as pos,material,kartons,kg_per_karton as a,"
  		" reste,rest_kg as kg  from rl_inhalt order by 1;"
  	    +"\" >"+resultdir+"rl_inhalt"+"_"+zusatz).c_str());
     system((psql_cmd+" \""+
  	"select position_ as pos,material,typ,kartons,kg_per_karton as a,"
  		" reste,rest_kg as kg,misc from rl_log order by zeit;"
  	    +"\" >"+resultdir+"rl_log"+"_"+zusatz).c_str());
  }
}

Check::was_checken operator|(Check::was_checken a, Check::was_checken b)
{  return Check::was_checken(int(a)|int(b)); }
bool operator&(Check::was_checken a, Check::was_checken b)
{  return int(a)&int(b); }
