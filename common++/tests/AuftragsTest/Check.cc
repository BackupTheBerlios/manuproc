// $Id: Check.cc,v 1.15 2002/11/07 07:49:16 christof Exp $
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

bool Check::teste(e_check check)
{
  dump(check);  
  return vergleich(check);
}


bool Check::vergleich(e_check check)
{ std::vector<std::string> files;
  bool error=false;

  switch (check)
  {  
   case Jumbo_richtig: case Jumbo_falsch: case Jumbo_doppelt:
  	   files.push_back("rohjumbo");
     	files.push_back("lager_bewegung");
  	   break;
   case LieferscheinTeil: case LieferscheinZeileLoeschen: case LieferscheinZeileLoeschen_n:
   case LieferscheinVoll: case LieferscheinMengenaenderungPlus:
   case LieferscheinMengenaenderungMinus: case LieferscheinZusatz:
   case LieferscheinZusatzPlus: case LieferscheinZusatzMinus:
   case LieferscheinZusatzMinusKunde: case LieferscheinZweiAufTeil:
   case LieferscheinZweiAufVoll: case LieferscheinJacek0:
        files.push_back("lieferschein");
        files.push_back("lieferscheinentry");
        files.push_back("lieferscheinentryzusatz");
   default: 
        files.push_back("auftragsentryzuordnung");
        files.push_back("auftragentry");
        break;
  }

  std::string zusatz;
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
     case ZweiKundenTest_anlegen: zusatz="_ZK_anlegen"; break;
     case ZweiKunden_Teil1: zusatz="_ZK_abschreiben1T"; break;
     case ZweiKunden_Teil2: zusatz="_ZK_abschreiben2T"; break;
     case ZweiKunden_Ueber1:zusatz="_ZK_abschreiben1U"; break;

     // Jumbo 
     case Jumbo_richtig : zusatz="_richtig"; break;
     case Jumbo_falsch : zusatz="_falsch"; break;
     case Jumbo_doppelt : zusatz="_doppelt"; break;
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
      { cout << "Probleme, Empfehlung: \n "<< "mgdiff "+fz1+" "+fz2<<"\n"; 
        error=true;
      }
#endif
  }
  if (error) return false;//exit(1);
  else return true;
}


void Check::dump(e_check check)
{
  static const std::string psql_cmd="psql "+std::string(getenv("PGDATABASE"))+" -q -X -c";

  if(check == Jumbo_richtig || check ==Jumbo_falsch || check==Jumbo_doppelt)
  {  unlink((tempdir+"rohjumbo").c_str());
     unlink((tempdir+"lager_bewegung").c_str());

     system((psql_cmd+" \""+
  	"select code,maschine,lauf,gang,status,wiederinslager,verarb_datum,"
  	  "artikelid,rest,lagerplatz from rohjumbo order by 1;"
  	    +"\" >"+tempdir+"rohjumbo").c_str());
     system((psql_cmd+" \""+
  	"select code,action,name,lagerplatz from lager_bewegung order by code,zeit;"
  	    +"\" >"+tempdir+"lager_bewegung").c_str());
     return;
  }
  else if(check == LieferscheinTeil || check == LieferscheinZeileLoeschen
          || check == LieferscheinZeileLoeschen_n
          || check == LieferscheinVoll || check==LieferscheinMengenaenderungPlus
          || check == LieferscheinMengenaenderungMinus 
          || check == LieferscheinZusatz || check == LieferscheinZusatzPlus
          || check == LieferscheinZusatzMinus 
          || check == LieferscheinZusatzMinusKunde
          || check == LieferscheinZweiAufTeil
          || check == LieferscheinZweiAufVoll)
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

