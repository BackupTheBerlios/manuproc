/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG
 *  written by Malte Thoma and Christof Petig
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

#include "steuerprogramm.hh"

#ifdef MABELLA_TEST
#include "AuftragsVerwaltung.hh"
#include "Check.hh"
#include <Lager/Lager.h>
#include <Auftrag/Auftrag.h>
#include <Misc/Trace.h>
#include <unistd.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>
#include "TestReihe.h"
#include <Auftrag/AufEintragBase.h>

static bool Lieferschein_Kunde(AufEintrag &AE)
{
	{
       AufEintragBase auf2=AuftragsVerwaltung().anlegen2();
	AufEintragBase aeb2(KUNDENINSTANZ,auf2.Id(),1);
	 AufEintrag ae2(aeb2);
       vergleichen(Check::Menge,"LS_auftrag2","2. Auftrag anlegen","a2");
	}

	{// Einkauf liefert Vollmenge

//        Auftrag auftrag=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);

//        AufEintragBase AEB2=auftrag.push_back(10,DATUM,ARTIKEL_TRIO,OPEN,false);
        Auftrag auftrag=Auftrag(Auftrag::Anlegen(EINKAUF),LIEFERANT);

#if 0
	AufEintrag AE(AufEintragBase AE(EINKAUF,0,1));
	AE.Planen(10,auftrag,DATUM);


	AufEintragBase AEB(EINKAUF,0,1);
	AufEintrag AE(AEB);
        AE.Planen(8,auftrag,DATUM9);
	AufEintragBase AEB2(EINKAUF,0,2);
	AufEintrag AE2(AEB2);
        AE2.Planen(5,auftrag,DATUM);
       vergleichen(Check::Menge|Check::Lieferschein,"PL_Einkauf","Einkauf Planen","PL");
        AufEintragBase AEB3(EINKAUF,auftrag.Id(),1);


//       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
       Lieferschein liefs(EINKAUF,cH_Kunde(LIEFERANT));

//	AufEintrag ae(AEB2);
//       liefs.push_back(ae,ARTIKEL_TRIO,10);

	AufEintrag ae(AEB3);
       liefs.push_back(ae,ARTIKEL_TRIO,8);
        AufEintragBase AEB4(EINKAUF,auftrag.Id(),2);
	AufEintrag ae2(AEB4);
       liefs.push_back(ae2,ARTIKEL_TRIO,5);

       vergleichen(Check::Menge|Check::Lieferschein,"LS_Einkauf","Lieferschein im Einkauf (Mabella) anlegen","VE");
#endif
	}

	{
       AufEintragBase auf2=AuftragsVerwaltung().anlegen3();
	AufEintragBase aeb2(KUNDENINSTANZ,auf2.Id(),1);
	 AufEintrag ae2(aeb2);
       vergleichen(Check::Menge,"LS_auftrag3","3. Auftrag anlegen","a3");

       Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE2));
       liefs.push_back(ae2,ARTIKEL_TRIO,12);
	}
       vergleichen(Check::Lieferschein|Check::Menge,"LS_volllieferung1","Lieferschein mit Volllieferung (Mabella) anlegen","V1");
	
/*	{
       AufEintragBase AEB(KUNDENINSTANZ,30000,1);
	AufEintrag AE(AEB);
       Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
       liefs.push_back(AE,ARTIKEL_TRIO,2);
	}

       vergleichen(Check::Lieferschein|Check::Menge,"LS_volllieferung2","Lieferschein mit Volllieferung (Mabella) anlegen","V2");
*/
       return true;
}

static TestReihe Lieferschein_Kunde_(&Lieferschein_Kunde,"Lieferschein Test für Mabella","LM");


/*
       int lznr=1;
       LieferscheinEntry le1(LieferscheinEntryBase(liefs,lznr));
       int stueck=3;
       le1.changeMenge(stueck,0,liefs,true);
       vergleichen(Check::Lieferschein|Check::Menge,"LS_mengenaenderung_minus","Lieferscheinentry: Minus","M");

      {// Einkauf liefert Teilmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
       liefs.push_back(ARTIKEL_TRIO,13);
       vergleichen(Check::Menge|Check::Lieferschein,"LSZP","Lieferschein im Einkauf mit Teillieferung (Mabella)","");
      }


      { // Bestellen eines Artikels ohne Kunden-Auftrag
        Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
        ManuProC::st_produziert sp(ARTIKEL_ZWEI,2222,getuid(),Kunde::eigene_id,LieferscheinBase::none_id,PA,SPLITDATUM);
        cH_ppsInstanz I(EINKAUF);
        I->Planen(sp);
        vergleichen(Check::Menge,"zwei_auftraege_anlegen","Einkauf eines nicht-bestelleten Artikel (Mabella)","");
      }

}

*/


/*
static bool Rep_Mabella(AufEintrag &AE)
{  LieferscheinJacek(AE);
      assert(Check::reparieren);
//      graphheader("Reparatur-Test für Mabella");

      // Test 1a: 0er und 2er != OPEN
      // Test 1b: KundenID!=eigene_id
      // Test 1c: 0er und 2er bestellt != 0
      std::string q1="update auftragentry set status="+itos(CLOSED)+" where "
                     "(instanz=2 or instanz=3 or instanz=4) and "
                     "(auftragid=0 or auftragid=2)";
      Query::Execute(q1);
      SQLerror::test(__FILELINE__);
      std::string q1b="update auftrag set stat="+itos(CLOSED)+" where "
                     "(instanz=2 or instanz=3 or instanz=4) and "
                     "(auftragid=0 or auftragid=2)";
      Query::Execute(q1b);
      SQLerror::test(__FILELINE__);
      std::string q2="update auftrag set kundennr="+itos(2)+" where "
                     "(instanz=2 or instanz=3 or instanz=4) and "
                     "(auftragid=0 or auftragid=2 or auftragid=1)";
      Query::Execute(q2);
      SQLerror::test(__FILELINE__);
      std::string q3="update auftragentry set geliefert="+itos(333)+" where "
                     "(instanz=2 or instanz=3 or instanz=4) and "
                     "(auftragid=0 or auftragid=2)";
      Query::Execute(q3);
      SQLerror::test(__FILELINE__);

      vergleichen(Check::Menge,"zwei_auftraege_anlegen","Reparatur (Mabella)", "",true);
       return true;
}

static TestReihe Rep_Mabella_(&Rep_Mabella,"Reparatur-Test für Mabella","RM");
*/


static bool Rollereiplanung()
{  
       Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
       AufEintragBase AEB2=auftrag.push_back(40,DATUM,ArtikelBase(1),OPEN,true);
       vergleichen(Check::Menge,"roll_Ausgangspunkt","Ausgangspunkt","");
       
       AufEintragBase sourceb(ppsInstanz::ID(3),AuftragBase::ungeplante_id,1);
       AufEintrag source(sourceb);
       assert(source.Artikel()==ArtikelBase(1));
       AuftragBase dest(ppsInstanz::ID(4),AuftragBase::ungeplante_id);
       source.Planen(20,dest,source.getLieferdatum());
       vergleichen(Check::Menge,"roll_Plan","Hälfte selbst weben","h");
       
       return true;
}

static TestReihe Rollereiplanung_(&Rollereiplanung,"Rollereiplanung","roll");

#endif
