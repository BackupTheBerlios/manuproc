// $Id: db_upgrade.cc,v 1.13 2003/07/28 10:07:45 christof Exp $
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

void check_column(const std::string &table, const std::string &column,
		const std::string &type)
{  int oid,attnum;
  try
  {Query("select oid from pg_class where relname=?") 
	<< table
	>> oid;
  } catch (SQLerror &e)
  {  std::cerr << "Table "<< table << " is missing\n";
     return;
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
      }
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
  
  // neue definitive Priorität bei den Zuordnungen
  // allerdings muss der Index auftragsentryzuordnung_altauftr noch
  // geaendert werden (muss prioritaet beinhalten)
  check_column("auftragsentryzuordnung","prioritaet","timestamp with time zone");
  
  ManuProC::dbdisconnect();
  return 0;
}


