// $Id: db_upgrade.cc,v 1.41 2005/03/30 10:28:59 christof Exp $
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
        try {
        Query x(q);
        } catch (SQLerror &e)
        {  if (e.Code()!=-600) return false;  // NOTICE
        }
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

  // ‰ltere Dinge
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
  check_column("kunden","mwst","bool");
  
  // Vereinheitlichung mit personen
  check_column("kunden","name2","text");
  check_column("kunden","branr","smallint");
  check_column("kunden","gebdatum","date");
  
  // Auftragsbest‰tigung an andere Adresse
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
  // Lagerid in Lieferscheinentry
  check_column("lieferscheinentry","lagerid","integer");
  // Status im Lieferscheinentry analog zu Auftragenry
  check_column("lieferscheinentry","status","smallint");

  
  // Verk‰uferspezifische Daten
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

  check_column("ku_preisliste","tablename_gruppe","text");

  // FiBu
  check_column("buchjournal","geschlossen_am","date");
  if(check_column("buchung","buchungid","integer"))
    {
     Query_nt("alter table buchung alter buchung set not null");
     Query_nt("create sequence buchung_id_seq");
     Query_nt("alter table buchung alter buchungid set default nextval('buchung_id_seq')");
     Query_nt("create unique index buchung_uniq on buchung (buchungid)");
    }
     

  // neue definitive Priorit‰t bei den Zuordnungen
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
     // f¸r geteilte Ketten (Kantketten)
     check_column("webang_faeden","max_kettlaenge","integer");
     // f¸r kombinierte Ketten
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
  
  // Zahlungsarten
  check_column("rechnung_zahlungsart","frist_monate","integer");
  check_column("rechnung_zahlungsart","textid","integer");  
  
  // genau eine Warengruppe ist default
  if(check_column("artikelgruppen","defaultgrp","bool"))
    {
     Query("create unique index artikelgruppen_defgrp on artikelgruppen (defaultgrp)");
    }
    
  // Darstellung von Instanzen in pps
  check_column("prod_instanz","overview","text");
  // alternatives group number
  check_column("prod_instanz","alt_group_nr","integer");

  if (!check_table("webangaben"))
  {  if (check_table("webang_ketten","artikel integer not null,"
                "kettscheibe integer not null,"
     		"max_kettlaenge integer,"
     		"max_fadenzahl integer,"
     		"verlaengern boolean,"
     		"ausn_gaenge1 integer,"
     		"ausn_maxfd1 integer,"
     		"ausn_gaenge2 integer,"
     		"ausn_maxfd2 integer,"
     		"primary key (artikel,kettscheibe)"))
        Query_nt("insert into webang_ketten select distinct "
            "artikel,kettscheibe,max_kettlaenge,max_fadenzahl,verlaengern,"
            "ausn_gaenge, ausn_maxfd, ausn_gaenge2, ausn_maxfd2 "
            "from webang_faeden where kettscheibe>0");
  }

  // MindestMenge auf lager
  check_column("artikelstamm","mindbestand","integer");
  
  if (check_table("auftrag_label",
          "alid integer primary key,"
          "bezeichnung text,"
          "kurz text not null"))
  { Query_nt("create unique index auftrag_label_bezeichnung_key "
                "on auftrag_label(bezeichnung)");
    Query_nt("create unique index auftrag_label_kurz_key "
                "on auftrag_label(kurz)");
  }
  check_column("auftrag","uid","integer");
  check_column("auftrag","label","integer");

  // free text
  check_column("auftragentry","text","text");
  check_column("lieferscheinentry","text","text");
  
  // artbez_warengruppe, attributes for article types  
  check_column("artbez_warengruppe","statistik","boolean");

// optional
  if (argc>1)
    check_column("lieferscheinentry","reforder_free","text");

  // Einheiten
  check_column("einheiten","textid","integer");
  
  // abh√§ngige Preislisten
  check_column("ku_preisliste","pl_parent","numeric(5,0)");
  check_column("ku_preisliste","fkt_parent","numeric(8,3)");
  check_column("ku_preisliste","add_parent","numeric(8,3)");

  ManuProC::dbdisconnect();
  return 0;
}


