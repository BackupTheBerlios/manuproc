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

static std::string tempdir="/tmp/";
static std::string referenzdir="database_tables_test/";

void Check::teste(e_check check)
{
  dump(check);  
  vergleich(check);
}


void Check::vergleich(e_check check)
{ std::vector<std::string> files;
  bool error=false;

  switch (check)
  {  case Jumbo_richtig: case Jumbo_falsch: case Jumbo_doppelt:
  	files.push_back("rohjumbo");
  	files.push_back("lager_bewegung");
  	break;
     default: 
        files.push_back("auftragsentryzuordnung");
        files.push_back("auftragentry");
        break;
  }

  std::string zusatz;
  switch(check)
   {
     case Open : zusatz="_mit_lager_open"; break;
     case Menge_Plus : zusatz="_menge_plus"; break;
     case Menge_Minus : zusatz="_menge_minus"; break;
     case Menge_MinusMinus : zusatz="_menge_minus_bandlager"; break;
     case Rohwarenlager_einlagern : zusatz="_rohwarenlager_rein"; break;
     case Bandlager_einlagern : zusatz="_bandlager_rein"; break;
     case Rohwarenlager_auslagern : zusatz="_rohwarenlager_raus"; break;
     case Bandlager_auslagern : zusatz="_bandlager_raus"; break;
     case Datumsaenderung : zusatz="_datumsaenderung"; break;
     case StatusClosed : zusatz="_status_closed"; break;
     case Planen_Kupfer : zusatz="_planen_kupfer"; break;
     case Planen_Faerberei_teil : zusatz="_planen_faerberei_teil"; break;
     case Planen_Faerberei : zusatz="_planen_faerberei"; break;
     case Planen_Weberei : zusatz="_planen_weberei"; break;
     case Planen_Weberei1 : zusatz="_planen_weberei1"; break;
     case Split : zusatz="_split"; break;
     case Split_Rohwarenlager_einlagern : zusatz="_split_rohwarenlager_rein"; break;
     case Split_Rohwarenlager_auslagern : zusatz="_split_rohwarenlager_raus"; break;
     case Planen_Faerberei_ueber : zusatz="_planen_fuer_zweiten_auftrag"; break;
     case ZweiAuftraege_anlegen : zusatz="_zwei_auftraege_anlegen"; break;
     case ZweiterAuftrag_frueheresDatum : zusatz="_zwei_auftraege_datum"; break;
     case Jumbo_richtig : zusatz="_richtig"; break;
     case Jumbo_falsch : zusatz="_falsch"; break;
     case Jumbo_doppelt : zusatz="_doppelt"; break;
   }
   
  for (std::vector<std::string>::const_iterator i=files.begin();i!=files.end();++i)
  {  std::string fz1=tempdir+*i;
     std::string fz2=referenzdir+*i+zusatz;
      
  std::string s="diff --ignore-matching-lines=OID --ignore-matching-lines=TRIGGER -q "+fz1+" "+fz2;
  int reg=system(s.c_str());
  if(reg==-1) 
  {cout<< "Fehler im diff-Komando ("+*i+")\n"; exit(reg);}
   else if(reg==0) {cout << *i << " OK\n";}
   else 
   { cout << "Probleme, Empfehlung: \n "<< "mgdiff "+fz1+" "+fz2<<"\n"; 
     error=true;
   }
  }

  if (error) exit(1);
}


void Check::dump(e_check check)
{
  static const std::string psql_cmd="psql testdb -q -X -c";

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
  
  unlink((tempdir+"auftragsentryzuordnung").c_str());
  unlink((tempdir+"auftragentry").c_str());

  std::string s="pg_dump -i -c -O -x -T -t auftragsentryzuordnung testdb > "+tempdir+"auftragsentryzuordnung";
  system(s.c_str());

  s="pg_dump -i -c -O -x -T -d -t auftragentry testdb | "
// Dieser Befehl eliminiert das lasteditdate aus dem sql-dump.
      "  sed s/,/@/4 | sed s/,/@/4 | sed s/,/Ü/17 | sed s/,/Ü/17 |"
      "  sed 's/\\(.*\\)\\@\\(.*\\)\\@\\(.*\\)/\\1\\,\\3/' | "
      "  sed 's/\\(.*\\)\\Ü\\(.*\\)\\Ü\\(.*\\)/\\1\\,\\3/' > "+tempdir+"auftragentry ";
  system(s.c_str());
}

