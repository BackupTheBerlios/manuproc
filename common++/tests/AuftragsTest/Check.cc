// $Id: Check.cc,v 1.24 2002/12/04 12:12:09 thoma Exp $
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
#include <Aux/FetchIStream.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <unistd.h>
#include "steuerprogramm.hh"

static std::string tempdir="/tmp/";
#ifdef  MANU_PROC_TEST
static std::string referenzdir="database_tables_test_ManuProC/";
#elif defined  MABELLA_TEST
static std::string referenzdir="database_tables_test_Mabella/";
#elif defined PETIG_TEST
static std::string referenzdir="database_tables_test/";
#endif

bool Check::teste(e_check check,bool mit_reparatur_programm,bool vor_dem_test_reparieren)
{  return teste(WasChecken(check),Zusatz(check),mit_reparatur_programm,vor_dem_test_reparieren);
}

bool Check::teste(was_checken check,const std::string &zusatz, bool mit_reparatur_programm,bool vor_dem_test_reparieren)
{
  dump(check);  
  if(!vor_dem_test_reparieren)
   {
     bool b=vergleich(check,zusatz);
     if(!mit_reparatur_programm || !b) return b;
   }
  
  std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
  for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
   {
     if((*i)->KundenInstanz()) continue;
     if((*i)->LagerInstanz() && (*i)->EigeneLagerKlasseImplementiert())
      {
        std::string com="../../Programme/adjust_store -i "+itos((*i)->Id())+" -a A";
        system(com.c_str());
      }
     std::string com="../../Programme/adjust_store -i "+itos((*i)->Id())+" -a B";
     system(com.c_str());
     std::string com2="../../Programme/adjust_store -i "+itos((*i)->Id())+" -a C";
     system(com2.c_str());

     std::string comD="../../Programme/adjust_store -i "+itos((*i)->Id())+" -a D -y";
     int d=system(comD.c_str());
     std::string comE="../../Programme/adjust_store -i "+itos((*i)->Id())+" -a E -y";
     int e=system(comE.c_str());
     std::string comF="../../Programme/adjust_store -i "+itos((*i)->Id())+" -a F -y";
     int f=system(comF.c_str());
     if(d || e || f) {cout <<*i<<'\t'<< d<<e<<f<<'\n'; return false; }
   }  
  dump(check);  
  return vergleich(check,zusatz);
}

Check::was_checken Check::WasChecken(e_check check)
{
  switch (check)
  {  
   case Jumbo_richtig: case Jumbo_falsch: case Jumbo_doppelt:
	return Jumbo; // |Menge; ??
   case LieferscheinTeil: case LieferscheinZeileLoeschen: case LieferscheinZeileLoeschen_n:
   case LieferscheinVoll: case LieferscheinMengenaenderungPlus:
   case LieferscheinMengenaenderungMinus: case LieferscheinZusatz:
   case LieferscheinZusatzPlus: case LieferscheinZusatzMinus:
   case LieferscheinZusatzMinusKunde: case LieferscheinZweiAufTeil:
   case LieferscheinZweiAufVoll: case LieferscheinJacek0:
   case LieferscheinEinkaufTeillieferung:
        return Lieferschein|Menge;
   default: 
        return Menge;
  }
}

std::string Check::Zusatz(e_check check)
{  std::string zusatz;
  switch(check)
   {
     case Open : zusatz="_mit_lager_open"; break;

     // Mengentest
     case Menge_Plus : zusatz="_menge_plus"; break;
     case Menge_Minus : zusatz="_menge_minus"; break;
     case Datumsaenderung : zusatz="_datumsaenderung"; break;
     case Menge_MinusMinus : zusatz="_menge_minus_bandlager"; break;
     case StatusClosed : zusatz="_status_closed"; break;
     // Plantest
     case Planen_Kupfer : zusatz="_planen_kupfer"; break;
     case Planen_Faerberei_teil : zusatz="_planen_faerberei_teil"; break;
     case Planen_WebereiP : zusatz="_planen_webereiP"; break;
     case LieferscheinEinkaufTeillieferung : zusatz="_planen_einkauf_lieferschein"; break;
     // Splittest
     case Split : zusatz="_split"; break;
     case Split_Rohwarenlager_einlagern : zusatz="_split_rohwarenlager_rein"; break;
     case Split_Rohwarenlager_auslagern : zusatz="_split_rohwarenlager_raus"; break;
     // Lagertest
     case Rohwarenlager_einlagern : zusatz="_rohwarenlager_rein"; break;
     case Rohwarenlager_auslagern : zusatz="_rohwarenlager_raus"; break;
     case Planen_WebereiL : zusatz="_planen_weberei_fuer_lager"; break;
     case Bandlager_einlagern : zusatz="_bandlager_rein"; break;
     case Kunden_Teillieferung : zusatz="_kunde_teillieferung"; break;
     case Kunden_Ueberlieferung : zusatz="_kunde_ueberlieferung"; break;
     // Zwei Aufträge
     case Planen_Faerberei_ueber : zusatz="_planen_fuer_zweiten_auftrag"; break;
     case ZweiAuftraege_anlegen : zusatz="_zwei_auftraege_anlegen"; break;
     // Zweiter Auftrag früheres Datum
     case ZweiterAuftrag_frueheresDatum : zusatz="_zwei_auftraege_datum"; break;
     case ZweiterAuftrag_frueheresDatum_abschreiben : zusatz="_zwei_auftraege_datum_abschreiben"; break;
     case ZweiterAuftrag_frueheresDatum_closed : zusatz="_zwei_auftraege_datum_closed"; break;
     case Planen_WebereiD : zusatz="_zwei_auftraege_weberei_planen"; break;
     case ErsterAuftrag_frueheresDatum_closed :  zusatz="_erster_auftrag_closed"; break;
     // Lieferschein
     case LieferscheinTeil : zusatz="_LS_teillieferung"; break;
     case LieferscheinZeileLoeschen : zusatz="_LS_zeileloeschen"; break;
     case LieferscheinZeileLoeschen_n : zusatz="_LS_zeileloeschen2"; break;
     case LieferscheinVoll: zusatz="_LS_volllieferung"; break;
     case LieferscheinMengenaenderungPlus : zusatz="_LS_mengenaenderung_plus"; break;
     case LieferscheinMengenaenderungMinus : zusatz="_LS_mengenaenderung_minus"; break;
     case LieferscheinZusatz : zusatz="_LSZ"; break;
     case LieferscheinZusatzPlus : zusatz="_LSZP"; break;
     case LieferscheinZusatzMinus: zusatz="_LSZM"; break;
     case LieferscheinZusatzMinusKunde: zusatz="_LSZMK"; break;
     case LieferscheinZweiAufTeil: zusatz="_LSZA"; break;
     case LieferscheinZweiAufVoll: zusatz="_LSZAV"; break;
     case LieferscheinJacek0 : zusatz="_LSJ0"; break;
     case ProduktionsPlanungWeberei : zusatz="_PP"; break;
     case ProduktionsPlanungEinkauf : zusatz="_PPE"; break;
     case ZweiKundenTest_anlegen: zusatz="_ZK_anlegen"; break;
     case ZweiKunden_Teil1: zusatz="_ZK_abschreiben1T"; break;
     case ZweiKunden_Teil2: zusatz="_ZK_abschreiben2T"; break;
     case ZweiKunden_Ueber1:zusatz="_ZK_abschreiben1U"; break;
     case ZweiKundenMengeFuer: zusatz="_ZKM"; break;

     // Jumbo 
     case Jumbo_richtig : zusatz="_richtig"; break;
     case Jumbo_falsch : zusatz="_falsch"; break;
     case Jumbo_doppelt : zusatz="_doppelt"; break;
     
     default: zusatz="_unbekannt"; break;
   }
   return zusatz;
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
  {  std::string fz1=tempdir+*i;
     std::string fz2=referenzdir+*i+zusatz;
      
#ifdef CREATE_TEST_DATABASE
#warning WARNUNG: 
#warning WARNUNG: COMPILATION MIT CREATE_TEST_DATABASE
#warning WARNUNG: 
    system(("mv "+fz1+" "+fz2).c_str());
    cout << fz2<<" wurde neu erzeugt\n"; 
#else
     std::string s="diff -q "+fz1+" "+fz2;
//cout<<" DIFF: " << s<<'\n';
     int reg=system(s.c_str());
     if(reg==-1) { cout<< "Fehler im diff-Komando ("+*i+")\n"; exit(reg);}
     else if(reg==0) ;//zuviel Output :-( {cout << *i << " OK\n";}
     else 
      { cout << "Probleme, Empfehlung: \n "<< "mgdiff "+fz1+" "+fz2<<" &\n"; 
        error=true;
      }
#endif
  }
  return !error;
}


void Check::dump(was_checken check)
{
  static const std::string psql_cmd="psql "+std::string(getenv("PGDATABASE"))+" -q -X -c";

  if(check & Jumbo)
  {  unlink((tempdir+"rohjumbo").c_str());
     unlink((tempdir+"lager_bewegung").c_str());

     system((psql_cmd+" \""+
  	"select code,maschine,lauf,gang,status,wiederinslager,verarb_datum,"
  	  "artikelid,rest,lagerplatz from rohjumbo order by code;"
  	    +"\" >"+tempdir+"rohjumbo").c_str());
     system((psql_cmd+" \""+
  	"select code,action,name,lagerplatz from lager_bewegung order by code,zeit;"
  	    +"\" >"+tempdir+"lager_bewegung").c_str());
  }
  if(check & Lieferschein)
  {  
     unlink((tempdir+"lieferschein").c_str());
     unlink((tempdir+"lieferscheinentry").c_str());
     unlink((tempdir+"lieferscheinentryzusatz").c_str());

     system((psql_cmd+" \""+
  	"select instanz,lfrsid,kundennr,coalesce(rngid,-1) as rngid,instanz"
  	  " from lieferschein order by instanz,lfrsid;"
  	    +"\" >"+tempdir+"lieferschein").c_str());
     system((psql_cmd+" \""+
  	"select instanz,lfrsid,zeile,artikelid,refauftragid,stueck,refzeilennr,menge "
  	   " from lieferscheinentry order by instanz,lfrsid,zeile;"
  	    +"\" >"+tempdir+"lieferscheinentry").c_str());
     system((psql_cmd+" \""+
  	"select instanz,lfrsid,lfsznr,auftragid,auftragznr,menge "
  	   " from lieferscheinentryzusatz order by instanz,lfrsid,lfsznr,auftragid,auftragznr;"
  	    +"\" >"+tempdir+"lieferscheinentryzusatz").c_str());
  }
  
 if (check & Menge)
 {
  unlink((tempdir+"auftragsentryzuordnung").c_str());
  unlink((tempdir+"auftragentry").c_str());

  std::string s1=psql_cmd+" \"select to_char(e.instanz,'99') ||' - ' "
      "|| to_char(e.auftragid,'99999') || ' - ' || text(zeilennr) as "
      "\\\"instanz-id-znr\\\",bestellt,"
      "geliefert,lieferdate,e.status,a.stat,kundennr,artikelid from auftragentry e"
      ", auftrag a where a.auftragid=e.auftragid and a.instanz=e.instanz "
      "order by 1;\" > "+tempdir+"auftragentry"; 
  system(s1.c_str());

  std::string s2=psql_cmd+" \"select to_char(altinstanz,'99') ||' - ' "
      "|| to_char(altauftragid,'99999') || ' - ' || text(altzeilennr) as "
      "\\\"ALT:instanz-id-znr\\\",
      to_char(neuinstanz,'99') ||' - ' "
      "|| to_char(neuauftragid,'99999') || ' - ' || text(neuzeilennr) as "
      "\\\"NEU:instanz-id-znr\\\",menge from auftragsentryzuordnung "
      "order by 1,2;\" > "+tempdir+"auftragsentryzuordnung"; 
  system(s2.c_str());
 }
  if(check & RohLager)
  {  unlink((tempdir+"rl_inhalt").c_str());
     unlink((tempdir+"rl_log").c_str());

     system((psql_cmd+" \""+
  	"select position_ as pos,material,kartons,kg_per_karton as a,"
  		" reste,rest_kg as kg  from rl_inhalt order by 1;"
  	    +"\" >"+tempdir+"rl_inhalt").c_str());
     system((psql_cmd+" \""+
  	"select position_ as pos,material,typ,kartons,kg_per_karton as a,"
  		" reste,rest_kg as kg,misc from rl_log order by zeit;"
  	    +"\" >"+tempdir+"rl_log").c_str());
  }
}

Check::was_checken operator|(Check::was_checken a, Check::was_checken b)
{  return Check::was_checken(int(a)|int(b)); }
bool operator&(Check::was_checken a, Check::was_checken b)
{  return int(a)&int(b); }
