// $Id: db_upgrade.cc,v 1.23 2003/12/09 13:54:09 christof Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/dbconnect.h>
#include <Misc/exception.h>
#include <Misc/FetchIStream.h>
#include <Misc/inbetween.h>
#include <ecpgerrno.h>

bool check_column(const std::string &table, const std::string &column,
		const std::string &type)
{  int oid,attnum;
  try
  {Query("select oid from pg_class where relname=?") 
	<< table
	>> oid;
  } catch (SQLerror &e)
  {  std::cerr << "Table "<< table << " is missing\n";
     return false;
  }
   try
   { Query("select attnum from pg_attribute where attrelid=? and attname=?")
   	<< oid << column
   	>> attnum;
   }
   catch (SQLerror &e)
   {  //std::cerr << e.Code() << '\n';
      if (in(e.Code(),ECPG_UNKNOWN_DESCRIPTOR,ECPG_NOT_FOUND))
      {  std::string q="ALTER TABLE "+table+" ADD "+column+" "+type;
         std::cout << q << '\n';
         Query x(q);
	 return true;
      }
   }
   return false;
}

bool check_table(const std::string &table, const std::string &definition="")
{ int oid;
  try
  {Query("select oid from pg_class where relname=?") 
	<< table
	>> oid;
  } catch (SQLerror &e)
  {  if (!definition.empty()) 
     {  std::string q="create table "+table+" ("+definition+")";
        std::cout << q << '\n';
        Query x(q);
     }
     return true;
  }
  return false;
}

void Query_nt(const std::string &s)
{  try
   { Query q(s);
     std::cout << s << '\n';
   } 
   catch (SQLerror &e)
   {  std::cerr << s << ": " <<e << '\n';
   }
}

int main(int argc,char *argv[])
{
  ManuProC::PrintUncaughtExceptions();
  ManuProC::dbconnect();

  // ältere Dinge
  check_column("kunden","aktiv","boolean");
  check_column("kunden","preisauto","boolean");
  check_column("kunden","showean","boolean");
  check_column("artikelpreise","mindestmenge","integer");
  check_column("kunden","lieferung_an","integer");
  check_column("extbezschema","textid","integer");
  check_column("extbezschema","preis_signifikant","boolean");
  check_column("kunden","rng_notiz","text");
  check_column("kunden","auf_notiz","text");
  check_column("kunden","lief_notiz","text");
  
  // Vereinheitlichung mit personen
  check_column("kunden","name2","text");
  check_column("kunden","branr","smallint");
  check_column("kunden","gebdatum","date");
  
  // Auftragsbestätigung an andere Adresse
  check_column("kunden","ab_an_rngadresse","bool");
  
  // Provisionsaten in Rechnung
  check_column("rechnung","verknr","numeric(5)");
  check_column("rechnungentry","provsatz","numeric(4,2)");
  check_column("rechnung","an_fibu_am","date");
  if(check_column("rechnung","fibu_buchid","integer"))
    Query_nt("ALTER TABLE rechnung add foreign key (fibu_buchid)"
	" references buchung (buchungid)");
  

  // Provisionsdaten in Auftrag
  check_column("auftrag","verknr","numeric(5)");
  check_column("auftragentry","provsatz","numeric(4,2)");

  // Verkaeufer in Lieferschein
  check_column("lieferschein","verknr","numeric(5)");
  
  // Verkäuferspezifische Daten
	// es gibt noch die alte Spalte verknr mit anderem Nummernkreis,
	// wegfallen soll. Danach vknr -> verknr umbennenen
  check_column("ku_verkaeufer","vknr","numeric(5)");
	// Nur Abschlag pro Abrechnung. 
  check_column("ku_verkaeufer","abschlag","numeric(5)");
	// Abrechnung alle "mon" Monate
  check_column("ku_verkaeufer","abrzyklus_mon","numeric(2)");  
  
  // Preislisten
  if(check_column("ku_preisliste","art","char(1)"))
    {
     Query_nt("update ku_preisliste set art='V'");
     Query_nt("alter table ku_preisliste alter art set not null");
     Query_nt("alter table ku_preisliste alter art set not null");
    }

  // FiBu
  check_column("buchjournal","geschlossen_am","date");
  if(check_column("buchung","buchungid","integer"))
    {
     Query_nt("alter table buchung alter buchung set not null");
     Query_nt("create sequence buchung_id_seq");
     Query_nt("alter table buchung alter buchungid set default nextval('buchung_id_seq')");
     Query_nt("create unique index buchung_uniq on buchung (buchungid)");
    }
     

  // neue definitive Priorität bei den Zuordnungen
  // allerdings muss der Index auftragsentryzuordnung_altauftr noch
  // geaendert werden (muss prioritaet beinhalten)
  check_column("auftragsentryzuordnung","prioritaet","timestamp with time zone");
  
  // Vertriebsabrechnung
  if (check_column("rechnung","kurs","numeric(10,5)"))
  { Query_nt("update rechnung set kurs=(select fkt from waehrung where "
  		"waehrung.wid=rechnung.waehrung)");
  }
  // haben wir uns nicht dagegen entschieden?
  // if (check_column("waehrung","eufkt","numeric(10,5)"))
  //    Query_nt("update  waehrung set eufkt=fkt");
  check_column("rechnungentry","ek_preis","numeric(10,2)");
  
  // schaerangaben nun in webangaben mit drin
  if (!check_table("webangaben"))
  {  check_column("webang_faeden","kettscheibe","integer");
     // für geteilte Ketten (Kantketten)
     check_column("webang_faeden","max_kettlaenge","integer");
     // für kombinierte Ketten
     check_column("webang_faeden","max_fadenzahl","integer"); 

     // references?     
     if (check_table("webang_variante","artikel integer not null,"
     		"altmaterial integer not null,"
     		"neumaterial integer not null"))
        Query_nt("create index webang_variante_art on webang_variante(artikel)");
     check_column("webangaben","variante_von","integer");
  }
  
  // Buchung
  check_column("buchung","waehrung","integer");
  
  ManuProC::dbdisconnect();
  return 0;
}


