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

static bool Auftrag_Kunde()
{
 Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE); 
 AufEintragBase AEB=auftrag.push_back(16,DATUM,ARTIKEL_TRIO,OPEN,true);
 vergleichen(Check::Menge,"auftrag_anlegen","Kundenauftrag anlegen","AM");

 Auftrag best=Auftrag(Auftrag::Anlegen(EINKAUF),LIEFERANT); 
 AufEintragBase BE=best.push_back(16,DATUM,ARTIKEL_TRIO,OPEN,true);
 vergleichen(Check::Menge,"bestellung_anlegen","Bestellung anlegen","BE");

 Lieferschein we(EINKAUF,cH_Kunde(LIEFERANT));
 AufEintrag BEAE(BE);
 LieferscheinEntryBase lsb(we,we.push_back(BEAE,ARTIKEL_TRIO,20,0,0));
 LieferscheinEntry lsbe(lsb);
 lsbe.lagerid=FertigWarenLager::default_lagerid;
 lsbe.changeStatus(OPEN,true);
 vergleichen(Check::Lieferschein|Check::Menge,"wareneingang_anglegen","Wareneingang","WE");

 Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
 AufEintrag AE(AEB);
 LieferscheinEntryBase LS(liefs,liefs.push_back(AE,ARTIKEL_TRIO,16,0,0));
 LieferscheinEntry LE(LS);
 LE.lagerid=FertigWarenLager::default_lagerid;
 LE.changeStatus(OPEN,true);
 vergleichen(Check::Lieferschein|Check::Menge,"Auslief","Auslieferung","L");
	
 return true;
}

static TestReihe Auftrag_Kunde_(&Auftrag_Kunde,"Kundenauftrag (Mabella)","AM");


static bool Komplex_Artikel()
{
 Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE); 
 AufEintragBase AEB=auftrag.push_back(1,DATUM,ARTIKEL_MUSTER,OPEN,true);
 vergleichen(Check::Menge,"muster_auftrag_anlegen","Kundenmusterauftrag anlegen","MA");

/* Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
 AufEintrag AE(AEB);
 LieferscheinEntryBase LS(liefs,liefs.push_back(AE,ARTIKEL_MUSTER,1,0,0));
 LieferscheinEntry LE(LS);
 LE.lagerid=FertigWarenLager::default_lagerid;
 LE.changeStatus(OPEN,true);
*/
 vergleichen(Check::Lieferschein|Check::Menge,"muster_auftrag_liefern","Auslieferung","L");
	
 return true;
}

static TestReihe Komplex_Artikel_(&Komplex_Artikel,"Zusammengesetzte Artikel","ZuA");


static bool Rollereiplanung()
{  
       Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
       AufEintragBase AEB2=auftrag.push_back(40,DATUM,ARTIKEL_TRIO,OPEN,true);
       vergleichen(Check::Menge,"roll_Ausgangspunkt","Ausgangspunkt","a");
       
       AufEintragBase sourceb(EINKAUF,AuftragBase::ungeplante_id,1);
       AufEintrag source(sourceb);
       assert(source.Artikel()==ARTIKEL_TRIO);
#warning Es gibt keine ppsInstanzID::Rollerei in der Testdatenbank !!!
//       AuftragBase dest(ppsInstanzID::Rollerei,AuftragBase::ungeplante_id);
//       source.Planen(20,dest,source.getLieferdatum());
       vergleichen(Check::Menge,"roll_Plan","H�lfte selbst weben","b");
       
       return true;
}

static TestReihe Rollereiplanung_(&Rollereiplanung,"Rollereiplanung","roll");

#include <Artikel/ArtikelStamm.h>

static bool MindestMenge()
{  FertigWaren fw(ARTIKEL_TRIO,FertigWaren::eManuell,0);
   FertigWarenLager fwl(fw,FertigWarenLager::default_lagerid);
//   fwl.Einlagern(ProductionContext());
   
   ArtikelStamm(make_value(ArtikelBase(ARTIKEL_TRIO))).setMindBest(5);
   vergleichen(Check::Menge,"minmen_Ausgangspunkt","Ausgangspunkt","a");
   
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
   AufEintragBase AEB2=auftrag.push_back(4,DATUM,ARTIKEL_TRIO,OPEN,true);
   vergleichen(Check::Menge,"minmen_Bestellung","Bestellung","b");   
   
   AufEintragBase AEB3=auftrag.push_back(4,DATUM9,ARTIKEL_TRIO,OPEN,true);
   vergleichen(Check::Menge,"minmen_Best2","Bestellung 2","b2");
   
   AufEintrag(AEB2).changeStatus(STORNO);
   vergleichen(Check::Menge,"minmen_storno","Storno","s");   
   
   Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
   AufEintrag AE(AEB3);
   LieferscheinEntryBase lsb(liefs,liefs.push_back(AE,ARTIKEL_TRIO,4,0,0));
   LieferscheinEntry LE(lsb);
   LE.lagerid=fwl.Id();
   LE.changeStatus(OPEN,true);
   vergleichen(Check::Lieferschein|Check::Menge,"minmen_Auslief","Lieferung","l");

   return true;
}

static TestReihe MindestMenge_(&MindestMenge,"Mindestmenge","minMen");


static bool MindestMenge_Lief()
{
   FertigWaren fw(ARTIKEL_TRIO,FertigWaren::eManuell,0);
   FertigWarenLager fwl(fw,FertigWarenLager::default_lagerid);
   
   ArtikelStamm(make_value(ArtikelBase(ARTIKEL_TRIO))).setMindBest(5);
   vergleichen(Check::Menge,"minmen_Ausgangspunkt","Ausgangspunkt","a");
   
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
   AufEintragBase AEB2=auftrag.push_back(4,DATUM,ARTIKEL_TRIO,OPEN,true);
   vergleichen(Check::Menge,"minmen_Bestellung","Bestellung","b");   
   
   Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
   AufEintrag AE(AEB2);
   LieferscheinEntryBase lsb(liefs,liefs.push_back(AE,ARTIKEL_TRIO,4,0,0));
   LieferscheinEntry LE(lsb);
   LE.lagerid=fwl.Id();
   LE.changeStatus(OPEN,true);
   vergleichen(Check::Lieferschein|Check::Menge,"minmen_Auslief2","Lieferung","l");
   LE.changeStatus(STORNO,true);
   vergleichen(Check::Lieferschein|Check::Menge,"minmen_Storno","Storno Lief","sl");

   return true;
}

static TestReihe MindestMenge_Lief_(&MindestMenge_Lief,"Mindestmenge LiefStorno","minMen_LS");

static bool MinMenRepair()
{  // FertigWaren fw(ARTIKEL_TRIO,FertigWaren::eManuell,5);
   // FertigWarenLager fwl(fw,FertigWarenLager::default_lagerid);
   // fwl.Einlagern(ProductionContext());

//   ArtikelStamm(make_value(ArtikelBase(ARTIKEL_TRIO))).setMindBest(5);
   vergleichen(Check::Menge,"MinMRep0","Ausgangspunkt","a");

   Check::NeedsRepair();
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
   AufEintragBase AEB2=auftrag.push_back(4,DATUM,ARTIKEL_TRIO,OPEN,true);
   vergleichen(Check::Menge,"MinMRep1","Bestellung","b");   
   
   Query("update auftragentry set bestellt=0 "
         "where (instanz,auftragid,zeilennr)=(?,?,?)")
     << 2 << 2 << 1;
   vergleichen(Check::Menge,"MinMRep2","Inkonsistenz","i");   
   
   return true;
}

static TestReihe MinMenRepair_(&MinMenRepair,"Mindestmenge Reparatur (bugcheck)","minMenRep");

// dieser Test testet ein früheres Problem, wo die nachbestellte Menge von 
// min(2er,0er) abgezogen und M so ohne Not negativ wurde
static bool MinMenRepair2()
{  vergleichen(Check::Menge,"MinMRep0","Ausgangspunkt","a");

   Check::NeedsRepair();
   Auftrag auftrag3=Auftrag(Auftrag::Anlegen(ppsInstanzID::Einkauf),Kunde::default_id);
   AufEintragBase AEB3=auftrag3.push_back(10,DATUM-5,ARTIKEL_TRIO,OPEN,true);
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
   AufEintragBase AEB2=auftrag.push_back(7,DATUM,ARTIKEL_TRIO,OPEN,true);
   vergleichen(Check::Menge,"MinMR2-1","Bestellung","b");   

   (Query("insert into fw_lager_buchung (artikelid,menge,datum,aktion,pid) "
         "values (?,3,now(),'M',?)")
      << ARTIKEL_TRIO << getuid()).Check100();
   Query("update auftragsentryzuordnung set menge=menge-3 "
         "where (altinstanz,altauftragid, neuinstanz,neuauftragid,neuzeilennr)=(2,2, ?,?,?)")
      << AEB3;
   vergleichen(Check::Menge,"MinMR2-2","Inkonsistenz","i");   
   
   return true;
}

static TestReihe MinMenRepair2_(&MinMenRepair2,"Mindestmenge Reparatur 2 (bugcheck)","minMenR2");

// Dieser Test testet einen Datumswechsel im Einkauf
static bool EinkaufDatum()
{  vergleichen(Check::Menge,"DAeA","Ausgangspunkt","a");

   Auftrag auftrag3=Auftrag(Auftrag::Anlegen(ppsInstanzID::Einkauf),LIEFERANT);
   AufEintragBase AEB3=auftrag3.push_back(30,DATUM,ARTIKEL_TRIO,OPEN,true);
   Lieferschein ls(EINKAUF,cH_Kunde(LIEFERANT));
   AufEintrag AE3(AEB3);
   LieferscheinEntryBase lsb(ls,ls.push_back(AE3,ARTIKEL_TRIO,20,0,0));
   LieferscheinEntry le(lsb);
   le.setLagerid(1); // offenbar nicht ppsInstanzID::Fertigwarenlager);
   le.changeStatus(OPEN,true);
   vergleichen(Check::Menge,"DAeA_l","Lieferung","l");   
   AE3.updateLieferdatum(DATUM+5);
   vergleichen(Check::Menge,"DAeA_t","Terminänderung","t");   
   return true;
}

static TestReihe EinfaufDatum_(&EinkaufDatum,"Datumsänderung im Einkauf (bugcheck)","DAeA");

#endif
