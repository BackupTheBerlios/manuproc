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
#ifdef PETIG_EXTENSIONS
#include "Check.hh"
#include <Aux/FetchIStream.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>
#include <Artikel/ArtikelBezeichnung.h>

void Check::teste(e_check check)
{
  dump(check);  
  vergleich(check);
}


void Check::vergleich(e_check check)
{
  bool errora=false,errorz=false;
  std::string fz1 = "/tmp/auftragsentryzuordnung";
  std::string fa1 = "/tmp/auftragentry_sed";
  std::string fz2="database_tables_test/auftragsentryzuordnung";
  std::string fa2="database_tables_test/auftragentry";
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
     case Split : zusatz="_split"; break;
     case Split_Rohwarenlager_einlagern : zusatz="_split_rohwarenlager_rein"; break;
     case Split_Rohwarenlager_auslagern : zusatz="_split_rohwarenlager_raus"; break;
     case ZweiAuftraege_anlegen : zusatz="_zwei_auftraege_anlegen"; break;
     case ZweiterAuftrag_frueheresDatum : zusatz="_zwei_auftraege_datum"; break;
// Zwei Zeilen für Christof :-)
     case Jumbo_raus : zusatz="_jumbo_raus"; break;
     case Jumbo_rein : zusatz="_jumbo_rein"; break;
   }
  fz2+=zusatz;
  fa2+=zusatz;  
  std::string s="diff --ignore-matching-lines=OID -q "+fz1+" "+fz2;
  int reg=system(s.c_str());
  if(reg==-1) {cout<< "Fehler im diff-Komando (auftragsentryzuordnung)\n"; exit(reg);}
  else if(reg==0) {cout << "auftragsentryzuordnung OK\n";}
  else {errorz=true; }

  s="diff --ignore-matching-lines=OID --ignore-matching-lines=RI_ConstraintTrigger_ -q "+fa1+" "+fa2;
  reg=system(s.c_str());
  if(reg==-1) {cout<< "Fehler im diff-Komando (auftragentry)\n"; exit(reg);}
  else if(reg==0) {cout << "auftragentry OK\n";}
  else { errora=true; }

  if(errora || errorz) 
   {
     if(errora) cout << "\nProbleme, Empfehlung: \n'"<< "mgdiff "+fa1+" "+fa2<<"'\n";
     if(errorz) cout << "Probleme, Empfehlung: \n'"<< "mgdiff "+fz1+" "+fz2<<"'\n";
     exit(1);
   }
}


void Check::dump(e_check check)
{
  system("rm /tmp/auftragsentryzuordnung");
  system("rm /tmp/auftragentry");
  system("rm /tmp/auftragentry_sed");

//if(check == Jumbo_raus || check ==Jumbo_rein) => Christof
  
  std::string s="pg_dump -i -c -O -x -T -t auftragsentryzuordnung testdb > /tmp/auftragsentryzuordnung";
  system(s.c_str());

  s="pg_dump -i -c -O -x -T -d -t auftragentry testdb > /tmp/auftragentry";
  system(s.c_str());

  // Dieser Befehl eliminiert das lasteditdate aus dem sql-dump.
  s="cat /tmp/auftragentry |  sed s/,/@/4 | sed s/,/@/4 | sed s/,/Ü/17 | sed s/,/Ü/17 |"
      "  sed 's/\\(.*\\)\\@\\(.*\\)\\@\\(.*\\)/\\1\\,\\3/' | "
      "  sed 's/\\(.*\\)\\Ü\\(.*\\)\\Ü\\(.*\\)/\\1\\,\\3/' > /tmp/auftragentry_sed ";
  system(s.c_str());
}

#endif
