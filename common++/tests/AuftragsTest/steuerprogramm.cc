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

#include <ManuProCConfig.h>
#include "DataBase_init.hh"
#include "AuftragsVerwaltung.hh"
#include "Check.hh"
#include "steuerprogramm.hh"
#include <Misc/dbconnect.h>
#include <Misc/exception.h>
#include <Lager/Lager.h>
#include <Auftrag/Auftrag.h>
#include <Misc/Trace.h>
#include <unistd.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>
#include <fstream>
#include <sys/stat.h>
#include <getopt.h>
#include <Misc/inbetween.h>
#include "TestReihe.h"

#ifdef PETIG_EXTENSIONS
#include <Lager/RohwarenLager.h>
#include <Lager/JumboLager.h>
#include <Ketten/KettplanKette.h>
#endif

#if 0
enum e_mode {None,Mengentest,Plantest,Lagertest,Splittest,ZweiAuftraege,
      ZweiterAuftrag_frueheresDatum,Lieferscheintest,LieferscheintestMenge,
      LieferscheintestZusatz,Lieferscheintest_ZweiterAuftrag_frueheresDatum,
      LieferscheinJacek,
      ZweiKundenTest,ZweiKundenMengeFreigebenTest,ManuProCTest,
      JumboLager,Rep_Mabella,Rep_Petig_PhysikalischesLager,
      Rep_Petig_0er_2er_gleichzeitig,Rep_KundenProgramm,Rep_Zuordnungen,
      Rep_Kunden_Zuordnungen, Zusatzinfo, Zusatzinfo2,
      AuftragLager };
#endif      

static std::ostream *testlog;
// for more output ...
static bool verbose=false;
static bool do_not_stop=false;
static std::string header;

TestReihe *TestReihe::first;

static UniqueValue::value_t log_trace = ManuProC::Tracer::channels.get();

static Check C;

void vergleichen(Check::was_checken w, const std::string &zusatz,
        const std::string &name, const std::string &graphname, bool vor_dem_test_reparieren)
{   (*testlog) << int(w) << ' ' << zusatz << ' ' << graphname << ' ' << name << '\n';
    bool erfolgreich=C.teste(w,zusatz,vor_dem_test_reparieren);
    if(!erfolgreich)
    { std::cout << name << "("<<zusatz<<") fehlgeschlagen\n\n"; 
      if (!do_not_stop) { testlog->flush(); exit(1); }
    }
    else if (verbose)
    { std::cout << name << "("<<zusatz<<") ok\n";
    }
}

static void graphheader(const std::string &name)
{   (*testlog) << '#' << ' ' << name << '\n';
    header=name;
}

static void erfolgreich()
{   std::cout << header << " erfolgreich\n\n";
}

static int auftragstests(TestReihe *mode)
{  if (Check::analyse) std::cout << "Mit Analyse\n";
   else if (Check::reparieren) std::cout << "Mit Reparatur\n";

   graphheader(mode->bezeichnung);

   // ANLEGEN eines Auftrags mit Bandlager und Rohwarenlager
   if (mode->aufruf_mit_auftrag)
//   !in(mode,JumboLager,Zusatzinfo,Zusatzinfo2))
   {
      AuftragsVerwaltung  auftrag;
      AufEintragBase AEB=auftrag.anlegen();
      AufEintrag AE=AEB;
      std::cout << "Anlegen eines Auftrags ["<<AEB<<"] beendet\n\n";
      AE.setStatus(OPEN,UID);
      vergleichen(Check::Menge,"mit_lager_open","Öffnen des Auftrags","");
      
      (*(mode->aufruf_mit_auftrag))(AE);
   }
   else (*(mode->aufruf))();

   if (!do_not_stop) erfolgreich();
   return 0;
}

#ifdef PETIG_TEST
static bool Zusatzinfo()
{
        AufEintragBase AEB2=Auftrag(AEB).push_back(4000,DATUM,ARTIKEL_FAERBEREI,OPEN,UID,true);
        AufEintragBase AEB3=Auftrag(AEB).push_back(4000,DATUM+10,ARTIKEL_FAERBEREI,OPEN,UID,true);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI_Ausgangspunkt","Ausgangspunkt","");
        
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       int lznr=liefs.push_back(ARTIKEL_FAERBEREI,9,1000);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI_Auslieferung","Auslieferung","L");
       
       LieferscheinEntryBase lsb(liefs,lznr);
       LieferscheinEntry(lsb).changeMenge(1,1000,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI_Aenderung","weniger","-");

       LieferscheinEntry(lsb).changeMenge(10,1000,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI_Aenderung2","mehr","+");

       LieferscheinEntry le(lsb);
       LieferscheinEntry::deleteEntry(le);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI_Storno","Storno","0");
       return true;
}

static TestReihe Zusatzinfo_(&Zusatzinfo,"Lieferscheinveränderungen","ZI");

static bool Zusatzinfo2()
{
       LagerPlatz LP(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ);
       KettplanKette KK=KettplanKette(Kette(MASCHINE,SCHAERDATUM));
       std::vector<JumboRolle> JR=JumboRolle::create(KK); // 100
       Zeitpunkt_new zp0("2002-3-1 11:00"),zp1("2002-3-1 11:11");
       class JumboLager JL;
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0,true);
       std::vector<JumboRolle> JR2=JumboRolle::create(KK); // 101
       JL.Jumbo_Einlagern(LP,JR2.front(),JumboLager::Einlagern,UID,"TEST",&zp0,true);

       AufEintragBase AEB3=Auftrag(AEB).push_back(170,DATUM,ARTIKEL_BANDLAGER,OPEN,UID,true);
       
       AufEintragBase AEB2=Auftrag(AEB).push_back(17500,DATUM,ARTIKEL_BANDLAGER,OPEN,UID,true);
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       AufEintrag ae(AEB2);
       int lznr=liefs.push_back(ae,ARTIKEL_BANDLAGER,9,1000);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1,true);
       JL.Jumbo_Entnahme(JR2.front(),JumboLager::Auslagern,UID,"TEST",&zp1,true);
       JumboRolle jr(JumboRolle::Pruefziffer_anhaengen(100));
       JL.Jumbo_Entnahme(jr,JumboLager::Auslagern,UID,"TEST",&zp1,true);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI2_Ausgangspunkt","init","");
       
       LieferscheinEntryBase lsb(liefs,lznr);
       LieferscheinEntry(lsb).changeMenge(4,1000,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI2_Problem","Mengenänderung","-");
       return true;
}

static TestReihe Zusatzinfo2_(&Zusatzinfo2,"komplexe Lieferscheinveränderung mit Prod-Selbst-Lager","ZI2");
#endif

#ifdef MANU_PROC_TEST
static bool ManuProCTest(AufEintrag &AE)
{
      {// Planen des Einkaufs (Granulat, grün)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=4;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      AufEintrag PAE(AEP.Planen(UID,200,PA,PLANDATUM5));
      vergleichen(Check::Menge,"planen_granulat","Planen des Einkaufs(Granulat)","P");

      // Spezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(PAE,ARTIKEL_GRANULAT_GRUEN,1,200);
//unspezifisch   liefs.push_back(ARTIKEL_GRANULAT_GRUEN,1,200,0);
      vergleichen(Check::Menge|Check::Lieferschein,"LS_teillieferung","Lieferschein (mit AEB, Granulat) anlegen","L");
      }

      {// Planen des Einkaufs (Metall)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=5;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      AufEintrag PAE(AEP.Planen(UID,10,PA,PLANDATUM5));
      vergleichen(Check::Menge,"planen_weberei_fuer_lager","Planen des Einkaufs(Metall)","P2");


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_METALL,1,5);
      vergleichen(Check::Lieferschein|Check::Menge,"LS_volllieferung","Lieferschein (unbestimmt, Metall) anlegen","L2");
      }

      {// Planen der Gießerei (ARTIKEL_GRIFF_ROT)
      Auftrag PA=Auftrag(Auftrag::Anlegen(GIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
      int znr=1;
      AufEintrag AEP((AufEintragBase(GIESSEREI,AuftragBase::ungeplante_id,znr)));
      AufEintrag PAE(AEP.Planen(UID,500,PA,PLANDATUM5));
      vergleichen(Check::Menge,"planen_faerberei_teil","Planen der Giesserei (Griff rot)","PG");


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(GIESSEREI,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_GRIFF_ROT,500);
      vergleichen(Check::Lieferschein|Check::Menge,"LSZ","Lieferschein (Gießerei) anlegen","LG");
      }

      {// Produktion in der Werkstatt ohne daß vorher etwas geplant wurde
      Lieferschein liefs(WERKSTATT,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_SCHRAUBENZIEHER_GELB,600);
      vergleichen(Check::Lieferschein|Check::Menge,"LSZP","Produktion in der Werkstatt anlegen","LW");
      }

      {// Lieferscheinschreiben für das Schraubenzieherlager => auslagern
      Lieferschein liefs(SCHRAUBENZIEHERLAGER,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_SCHRAUBENZIEHER_ROT,450);
      vergleichen(Check::Lieferschein|Check::Menge,"LSZM","Lieferschein für das Rohwarenlager (auslagern)","LR");
      }

      {// Lieferscheinschreiben für den Kunden
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,450);
      vergleichen(Check::Lieferschein|Check::Menge,"LSZMK","Lieferschein für den Kunden","LK");
      }

      {// Lieferscheinschreiben für den Kunden  (Überlieferung)
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,60);
      vergleichen(Check::Lieferschein|Check::Menge,"LSZA","Lieferschein für den Kunden (Überlieferung)","LKÜ");
      }
   return true;
}

static TestReihe ManuProCTest_(&ManuProCTest,"ManuProC Test","MP");
#endif

#ifdef PETIG_TEST
static bool Mengentest(AufEintrag &AE)
{
      // Menge des Auftrags erhöhen
      auftrag.kunden_bestellmenge_aendern(AE,500);
      vergleichen(Check::Menge,"menge_plus", "Erhöhen der Auftragmenge","");

      // Menge des Auftrags erniedrigen (Rohwarenlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AE,100);
      vergleichen(Check::Menge,"menge_minus","Reduzieren der Auftragmenge unter Rohwarenlagerbestand","");

      AE.updateLieferdatum(NEWDATUM,UID);
      vergleichen(Check::Menge,"datumsaenderung","Datumsänderung","D");

      // Menge des Auftrags weiter erniedrigen (Bandlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AE,10);
      vergleichen(Check::Menge,"menge_minus_bandlager","Reduzieren der Auftragmenge unter Bandlagerbestand","");

      AE.setStatus(CLOSED,UID);
      vergleichen(Check::Menge,"status_closed","Statussänderung (Closed)","C");
       return true;
}

static TestReihe Mengentest_(&Mengentest,"Mengen Test","M");

static bool Plantest(AufEintrag &AE)
{
       ManuProC::Trace(log_trace,__FILELINE__);
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::_Garn__Einkauf),Kunde::default_id);
       int kupfer_znr=2;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::_Garn__Einkauf,AuftragBase::ungeplante_id,kupfer_znr));

       AEP.Planen(UID,100,PA,PLANDATUM5);
       vergleichen(Check::Menge,"planen_acetat","Planen des Acetateinkaufs","A");
       }
       ManuProC::Trace(log_trace,__FILELINE__);

       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));

       ManuProC::Trace(log_trace,__FILELINE__);
       AEP.Planen(UID,7000,PA,PLANDATUM4);
       vergleichen(Check::Menge,"planen_faerberei_teil","Teil-Planen der Färberei","F");
       }
       ManuProC::Trace(log_trace,__FILELINE__);
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       AEP.Planen(UID,5000,PA,PLANDATUM6);
       vergleichen(Check::Menge,"planen_webereiP","Planen der Weberei","P");
       }
       ManuProC::Trace(log_trace,__FILELINE__);

       Lieferschein liefs(ppsInstanzID::_Garn__Einkauf,cH_Kunde(Kunde::eigene_id));
       liefs.push_back(ARTIKEL_ACETAT,1,66);
       vergleichen(Check::Menge,"planen_einkauf_lieferschein","Lieferschein mit Teillieferung für Einkauf anlegen","L");
       return true;
}

static TestReihe Plantest_(&Plantest,"Planungs Test","P");

static bool Splittest(AufEintrag &AE)
{
      AE.split(UID,300,SPLITDATUM);
      vergleichen(Check::Menge,"split","Splitten einer Auftragszeile","");
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,UID,dummystring,false,true);
std::cout << dummystring<<'\n';
      vergleichen(Check::Menge,"split_rohwarenlager_rein","Rohwarenlager einlagern\n","+");

      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,UID,dummystring,false,false,true);
std::cout << dummystring<<'\n';
      vergleichen(Check::Menge,"split_rohwarenlager_raus","Rohwarenlager auslagern\n","-");
}

static TestReihe Splittest_(&Splittest,"Split Test","S");

static bool Rep_Petig_PhysikalischesLager(AufEintrag &AE)
{     Splittest(AE);
      
      assert(Check::reparieren);
      // Physikalisches Lager ändern
      std::string qB="insert into rl_inhalt (position_,kartons,reste,kg_per_karton,"
         " material,eingelagert,rest_kg)"
         " values ('07D5',10,0,5,211007,'2002-11-28',0)";
      Query::Execute(qB);
      SQLerror::test(__FILELINE__);
      vergleichen(Check::Menge,"split_reparatur","Reparatur-Split-Test (Garnlager)","R",true);

      std::string qJ="insert into rohjumbo (code,maschine,soll_meter,plan_datum,"
         " status,lauf,gang,wiederinslager,artikelid,rest,lagerplatz) "
         " values (101,212,700,'2002-11-28',2,1,1,'2002-01-01 12:00:00+01',"
         " 123755,false,712)";
      Query::Execute(qJ);
      SQLerror::test(__FILELINE__);
      vergleichen(Check::Menge,"split_reparatur_bandlager","Reparatur-Split-Test (Bandlager)","RB",true);

      std::string qJ2="update rohjumbo set soll_meter=200 where "
         "(code,maschine,plan_datum,"
         " status,lauf,gang,wiederinslager,artikelid,rest,lagerplatz) "
         " = (101,212,'2002-11-28',2,1,1,'2002-01-01 12:00:00+01',"
         " 123755,false,712)";
      Query::Execute(qJ2);
      SQLerror::test(__FILELINE__);
      vergleichen(Check::Menge,"split_reparatur_bandlager_minus","Reparatur-Split-Test (BandlagerMinus)","RB-",true);

      std::string qB2="update rl_inhalt set kartons=4 where "
         " (position_,reste,kg_per_karton,"
         " material,eingelagert,rest_kg)"
         " = ('07D5',0,5,211007,'2002-11-28',0)";
      Query::Execute(qB2);
      SQLerror::test(__FILELINE__);
      vergleichen(Check::Menge,"split_reparatur_garnlager_minus","Reparatur-Split-Test (GarnlagerMinus)","RG-",true);
       return true;
}

static TestReihe Rep_Petig_PhysikalischesLager_(&Rep_Petig_PhysikalischesLager,"Reparatur-Test (Petig, Physikalisches Lager)","RP");

static bool Rep_Zuordnungen(AufEintrag &AE)
{  assert(Check::reparieren);
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,1));
       AEP.Planen(UID,17000,PA,DATUMP);
       vergleichen(Check::Menge,"rep_pf","Reparatur-Planen der Färberei","P");
       }
      {
        Query::Execute("update auftragentry set bestellt=5555 where "
            "(instanz,auftragid)=(2,2)");
        SQLerror::test(__FILELINE__);
        vergleichen(Check::Menge,"rep_pf","Reparatur-Zuordungen (1)","",true);
      }
      {
        Query::Execute("update auftragentry set bestellt=6555 where "
            "(instanz,auftragid)=(2,2)");
        SQLerror::test(__FILELINE__);
        Query::Execute("update auftragsentryzuordnung set menge=6555 where "
            "(altinstanz,altauftragid,neuinstanz)=(2,2,2)");
        SQLerror::test(__FILELINE__);
        vergleichen(Check::Menge,"rep_pf","Reparatur-Zuordungen (2)","",true);
      }
      {
        Query::Execute("update auftragentry set geliefert=16555 where "
            "(instanz,auftragid)=(2,30000)");
        SQLerror::test(__FILELINE__);
        vergleichen(Check::Menge,"rep_pfE","Reparatur-Zuordungen ()","E",true);
      }
       return true;
}

static TestReihe Rep_Zuordnungen_(&Rep_Zuordnungen,"Reparatur-Test (Petig, Zuordnungen)","RZ");

static bool Rep_KundenProgramm(AufEintrag &AE)
{
          assert(Check::reparieren);
      {
       std::string q1="update auftragentry set artikelid="+itos(ARTIKEL_ROLLEREIK.Id())+" where "
                         " (auftragid,zeilennr,instanz) = (0,1,10)";
       Query::Execute(q1);
       SQLerror::test(__FILELINE__);

       vergleichen(Check::Menge,"reparatur_kunde","Reparatur Kundenaufträge (Artikel)","",true);
      }
      {
       std::string q2="update auftragsentryzuordnung set menge=300 where "
                         " (altauftragid) = (30000)";
       Query::Execute(q2);
       SQLerror::test(__FILELINE__);
       AufEintrag AE=AufEintrag(class AufEintragBase(class AuftragBase(ROLLEREI,AuftragBase::ungeplante_id),2));
       AE.MengeAendern(UID,-100,true,AufEintragBase(),ManuProC::Auftrag::r_Anlegen);

       vergleichen(Check::Menge,"reparatur_kunde_menge","Reparatur Kundenaufträge (Menge)","",true);
      }

      {
       std::string q2a="update auftragentry set instanz=3 where "
                         " (auftragid,zeilennr,instanz) = (0,2,10)";
       std::string q2b="update auftragsentryzuordnung set neuinstanz=3 where "
                         " (neuauftragid,neuinstanz) = (0,10)";
       std::string q2c="update auftragsentryzuordnung set altinstanz=3 where "
                         " (altauftragid,altinstanz) = (0,10)";
       Query::Execute(q2a);
       SQLerror::test(__FILELINE__);
       Query::Execute(q2b);
       SQLerror::test(__FILELINE__);
       Query::Execute(q2c);
       SQLerror::test(__FILELINE__);

       vergleichen(Check::Menge,"reparatur_kunde_instanz","Reparatur Kundenaufträge (Instanz)","",true);
      }
      {
       std::string q2="update auftragentry set lieferdate='2009-12-31' where "
                         " (instanz) = (1)";
       Query::Execute(q2);
       SQLerror::test(__FILELINE__);
       vergleichen(Check::Menge,"reparatur_kunde_datum","Reparatur Kundenaufträge (Datum)","D",true);
      }
       return true;
}

static TestReihe Rep_KundenProgramm_(&Rep_KundenProgramm,"Reparatur Kundenaufträge (Artikel, Instanz)","RK");

static bool Rep_Kunden_Zuordnungen(AufEintrag &AE)
{
          assert(Check::reparieren);
       AufEintragBase AEB=auftrag.anlegen2();
       vergleichen(Check::Menge,"rep_zwei_auftraege_anlegen","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"]","");
      {
       Auftrag PA=Auftrag(Auftrag::Anlegen(SPRITZGIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
       AufEintrag AEP((AufEintragBase(SPRITZGIESSEREI,AuftragBase::ungeplante_id,1)));
       AEP.Planen(UID,200,PA,PLANDATUM5);
       vergleichen(Check::Menge,"rep_planen_spritz","Planen der Spritzgießerei (Reparatur)","");
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(SPRITZGIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
       AufEintrag AEP((AufEintragBase(SPRITZGIESSEREI,AuftragBase::ungeplante_id,1)));
       AEP.Planen(UID,5000,PA,PLANDATUM5);
       vergleichen(Check::Menge,"rep_planen_spritz2","Planen der Spritzgießerei (Reparatur)","");
       }
       {
        Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
        liefs.push_back(ARTIKEL_ROLLEREI,390,1);
        vergleichen(Check::Menge,"rep_lieferschein","Reparatur-Lieferschein anlegen","L");
       }
       return true;
}

static TestReihe Rep_Kunden_Zuordnungen_(&Rep_Kunden_Zuordnungen,"Reparatur Kunden Zuordnungen","RKZ");

static bool Lagertest()
{
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,UID,dummystring,false,true);
      vergleichen(Check::Menge,"rohwarenlager_rein","Rohwarenlager einlagern\n","+");

      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,UID,dummystring,false,false,true);
std::cout << "D1: "<<dummystring<<'\n';
      RohwarenLager::st_rohlager stRL3(LagerPlatzKupfer,2,10,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL3,UID,dummystring,false,false,true);
std::cout << "D2:" <<dummystring<<'\n';
      vergleichen(Check::Menge,"rohwarenlager_raus","Rohwarenlager auslagern\n","-");

      Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
      int weberei_znr=1;
      AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
      assert(AEP.getStueck()==AEP.getRestStk());
      AEP.Planen(UID,5000,PA,PLANDATUM5);
      vergleichen(Check::Menge,"planen_weberei_fuer_lager","Planen der Weberei zum späteren Test des Bandlagers","W");

      DataBase_init::createJumbo(-10,12000,true);
std::cout << dummystring<<'\n';
      vergleichen(Check::Menge,"bandlager_rein","Bandlager einlagern\n","b");

      {AufEintrag AE(AEB);
        AE.Produziert(300,Lieferschein::none_id);
      }
      vergleichen(Check::Menge,"kunde_teillieferung","Kunde Teillieferung\n","T");

//std::cout << "\n\n120 ABSCHREIBEN\n\n\n";
      {AufEintrag AE(AEB);
        AE.Produziert(120,Lieferschein::none_id);
      }
      vergleichen(Check::Menge,"kunde_ueberlieferung","Kunde Überlieferung\n","Ü");


      // test von force, leer, etc
#if 1
      dummystring="";
      RohwarenLager::st_rohlager stRL10(LagerPlatzKupfer2,6,35,1,7,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL10,UID,dummystring,true,true);
std::cout << "D10: "<<dummystring<<'\n';

      dummystring="";
      RohwarenLager::st_rohlager stRL11(LagerPlatzKupfer,0,0,2,1,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,UID,dummystring,false,false,true);
std::cout << "D11: "<<dummystring<<'\n';
      vergleichen(Check::Menge|Check::RohLager,"force_art","force, falscher Artikel","");

      dummystring="";
      RohwarenLager::st_rohlager stRL12(LagerPlatzKupfer2,10,35,0,0,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,UID,dummystring,false,false,true);
std::cout << "D12: "<<dummystring<<'\n';

      dummystring="";
      RohwarenLager::st_rohlager stRL13(LagerPlatzAcetat,2,7,0,0,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,UID,dummystring,false,true,true);
std::cout << "D13: "<<dummystring<<'\n';
      vergleichen(Check::Menge|Check::RohLager,"zuviel","zuviel Entnommen","");
#endif
       return true;
}

static TestReihe Lagertest_(&Lagertest,"Lager","L");

static bool Rep_Petig_0er_2er_gleichzeitig(AufEintrag &AE)
{
        assert(Check::reparieren);

      std::string q1="update auftragentry set bestellt=3000 where "
                     " (auftragid,zeilennr,instanz) = (2,1,8)";
      Query::Execute(q1);
      SQLerror::test(__FILELINE__);
      std::string qJ2="update rohjumbo set soll_meter=5000 where artikelid=123755";
      Query::Execute(qJ2);
      SQLerror::test(__FILELINE__);

      vergleichen(Check::Menge,"Rep0er2ergleichzeitig","Reparatur 0er und 2er gleichzeitig (Lager)","L",true);

      std::string q2="insert into auftragentry (auftragid,zeilennr,bestellt,"
         " geliefert,lieferdate,artikelid,status,instanz)"
         " values (2,5,3000,0,'2009-01-01',123755,1,4)";
      Query::Execute(q2);
      vergleichen(Check::Menge,"Rep0er2ergleichzeitigP","Reparatur 0er und 2er gleichzeitig (Produktionsinstanz)", "P",true);
       return true;
}

static TestReihe Rep_Petig_0er_2er_gleichzeitig_(&Rep_Petig_0er_2er_gleichzeitig,"Reparatur 2er und 0er gleichzeitig","Rg");

static bool ZweiAuftraege(AufEintrag &AE)
{
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,13000,PA,PLANDATUM6);
       vergleichen(Check::Menge,"planen_fuer_zweiten_auftrag","Über-Planen der Färberei","P");
       }
       AufEintragBase AEB=auftrag.anlegen2();
       vergleichen(Check::Menge,"zwei_auftraege_anlegen","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"]","Z");
       return true;
}

static TestReihe ZweiAuftraege_(&ZweiAuftraege,"Zwei Aufträge","Z");

static bool ZweiterAuftrag_frueheresDatum(AufEintrag &AE)
{
       AufEintragBase AEB=auftrag.anlegen3();

       vergleichen(Check::Menge,"zwei_auftraege_datum","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"] mit früherem Liefertermin","D");

       {
          AufEintrag AE(AEB);
        AE.Produziert(200,Lieferschein::none_id);
       }
       vergleichen(Check::Menge,"zwei_auftraege_datum_abschreiben","Teil-Abschreiben des zweiten Auftrags ["+AEB.str()+"]","A");


       AufEintrag(AEB).setStatus(CLOSED,UID);
       vergleichen(Check::Menge,"zwei_auftraege_datum_closed","Statussänderung(2) (Closed)","C");

       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       assert(AEP.getStueck()==AEP.getRestStk());
       AEP.Planen(UID,7000,PA,PLANDATUM5);
       vergleichen(Check::Menge,"zwei_auftraege_weberei_planen","Planen der Weberei","P");

       AE.setStatus(CLOSED,UID);
// OHNE ReparaturProgramm, da CLOSED-Kundedenaufträge noch nicht nach
//  unten korrigiert werden
       vergleichen(Check::Menge,"erster_auftrag_closed","Statussänderung(1) (Closed)","CL",false);
       return true;
}

static TestReihe ZweiterAuftrag_frueheresDatum_(&ZweiterAuftrag_frueheresDatum,"Zwei Aufträge, zweiter Auftrag früheres Datum","D");

static bool Lieferscheintest(AufEintrag &AE)
{
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,150);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_teillieferung","Lieferschein mit Teillieferung anlegen","");

       int lznr=1;
       LieferscheinEntry le((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_zeileloeschen","Lieferscheinzeile löschen","");

       liefs.push_back(ARTIKEL_ROLLEREI,450);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_volllieferung","Lieferschein mit Volllieferung","V");

       LieferscheinEntry le2((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le2);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_zeileloeschen2","Lieferscheinzeile nochmal löschen","");
       return true;
}

static TestReihe Lieferscheintest_(&Lieferscheintest,"Lieferschein","Ls");

static bool LieferscheintestMenge(AufEintrag &AE)
{
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,150);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_teillieferung","Lieferschein mit Teillieferung anlegen","");

       int stueck=140;
       AuftragBase::mengen_t menge=0;
       int lznr=1;
       LieferscheinEntry le(LieferscheinEntryBase(liefs,lznr));
       le.changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_mengenaenderung_minus","Lieferschein Mengenaenderung Minus","");

       stueck=400;
       LieferscheinEntry le2(LieferscheinEntryBase(liefs,lznr));
       le2.changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_mengenaenderung_plus","Lieferschein Mengenaenderung Plus","");
       return true;
}

static TestReihe LieferscheintestMenge_(&LieferscheintestMenge,"Lieferschein Menge","Lm");

static bool Lieferscheintest_ZweiterAuftrag_frueheresDatum(AufEintrag &AE)
{
       AufEintragBase AEB=auftrag.anlegen3();
       vergleichen(Check::Menge,"zwei_auftraege_datum","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"] mit früherem Liefertermin","D");

       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,50);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZA","Lieferschein mit Teillieferung und 2 Aufträgen anlegen","T");

       liefs.push_back(ARTIKEL_ROLLEREI,600);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZAV","Lieferschein mit Volllieferung und 2 Aufträgen anlegen","V");
       return true;
}

static TestReihe Lieferscheintest_ZweiterAuftrag_frueheresDatum_(&Lieferscheintest_ZweiterAuftrag_frueheresDatum,"Zwei Aufträge, zweiter Auftrag früheres Datum","LA");

static bool LieferscheintestZusatz(AufEintrag &AE)
{
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       int lznr=liefs.push_back(ARTIKEL_ROLLEREI,550);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZ","Lieferschein mit Zusatzeintrag anlegen","");

       int stueck=633;
       AuftragBase::mengen_t menge=0;

       LieferscheinEntry le3(LieferscheinEntryBase(liefs,lznr));
       le3.changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZP","Lieferscheinentry mit Zusatzeintrag Plus","+");

       LieferscheinEntry le4(LieferscheinEntryBase(liefs,lznr));
       stueck=450;

       le4.changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZM","Lieferscheinentry mit Zusatzeintrag Minus","-");

       LieferscheinEntry le5(LieferscheinEntryBase(liefs,lznr));
       stueck=350;
       le5.changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZMK","Lieferscheinentry mit Zusatzeintrag Minus Kunde","-K");
       return true;
}

static TestReihe _LieferscheintestZusatz(&LieferscheintestZusatz,"Lieferschein Zusatz","LZ");
#endif

#ifdef MABELLA_TEST
static bool LieferscheinJacek(AufEintrag &AE)
{
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_TRIO,10);
       vergleichen(Check::Lieferschein|Check::Menge,"LS_volllieferung","Lieferschein mit Volllieferung (Mabella) anlegen","V");

       int lznr=1;
       LieferscheinEntry le1(LieferscheinEntryBase(liefs,lznr));
       int stueck=3;
       le1.changeMenge(stueck,0,liefs,true);
       vergleichen(Check::Lieferschein|Check::Menge,"LS_mengenaenderung_minus","Lieferscheinentry: Minus","M");

      {// Überplanen des Einkaufs
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
      int znr=1;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      AEP.Planen(UID,27,PA,PLANDATUM5);
      vergleichen(Check::Menge,"planen_kupfer","Über-Planen des Einkaufs (Mabella)","E");
      }

      {// Einkauf liefert Teilmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
       liefs.push_back(ARTIKEL_TRIO,13);
       vergleichen(Check::Menge|Check::Lieferschein,"LSZP","Lieferschein im Einkauf mit Teillieferung (Mabella)","");
      }

      {// Einkauf liefert Vollmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
       liefs.push_back(ARTIKEL_TRIO,25);
       vergleichen(Check::Menge|Check::Lieferschein,"LSZM","Lieferschein im Einkauf Weberei mit Restlieferung (Mabella)","");
      }


/*      { // Bestellen eines Artikels ohne Kunden-Auftrag
        Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
        ManuProC::st_produziert sp(ARTIKEL_ZWEI,2222,getuid(),Kunde::eigene_id,LieferscheinBase::none_id,PA,SPLITDATUM);
        cH_ppsInstanz I(EINKAUF);
        I->Planen(sp);
        vergleichen(Check::Menge,"zwei_auftraege_anlegen","Einkauf eines nicht-bestelleten Artikel (Mabella)","");
      }
*/
       return true;
}

static TestReihe LieferscheinJacek_(&LieferscheinJacek,"Lieferschein Test für Mabella","LJ");

static bool Rep_Mabella(AufEintrag &AE)
{  LieferscheinJacek(AE);
      assert(Check::reparieren);
      graphheader("Reparatur-Test für Mabella");

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

#endif

static bool ZweiKundenTest(AufEintrag &AE)
{      AuftragsVerwaltung  auftrag;
       AufEintragBase AEB2=auftrag.anlegenK();
       vergleichen(Check::Menge,"ZK_anlegen","Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["+AE.str()+"]","");

        AE.Produziert(300,Lieferschein::none_id);
      vergleichen(Check::Menge,"ZK_abschreiben1T","Zwei Kunden Teillieferung 1\n","");

      {AufEintrag AE(AEB2);
        AE.Produziert(180,Lieferschein::none_id);
      }
      vergleichen(Check::Menge,"ZK_abschreiben2T","Zwei Kunden Teillieferung 2\n","");

        AE.Produziert(200,Lieferschein::none_id);
      vergleichen(Check::Menge,"ZK_abschreiben1U","Zwei Kunden Volllieferung 1\n","");
       return true;
}

static TestReihe ZweiKundenTest_(&ZweiKundenTest,"Zwei Kunden","ZK");

#ifdef PETIG_TEST
static bool ZweiKundenMengeFreigebenTest(AufEintrag &AE)
{
       AufEintragBase AEB2=auftrag.anlegenK();
       vergleichen(Check::Menge,"ZK_anlegen","Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["+AEB.str()+"]","");

       {
         cH_ppsInstanz I(ppsInstanzID::Bandlager);
         int znrvon=1;
         AufEintragBase Von(AuftragBase(I,AuftragBase::plan_auftrag_id),znrvon);
         int znrnach=2;
         AufEintrag Fuer((class AufEintragBase(AuftragBase(I,AuftragBase::ungeplante_id),znrnach)));
// hmmm was sollte das tun?         
//#warning Test neu designen
//         AufEintrag(Von).menge_fuer_aeb_freigeben(3000,Fuer,getuid());
//         vergleichen(Check::Menge,"ZKM","Menge freigeben für einen anderen Auftrag","");
       }
       return true;
}

static TestReihe ZweiKundenMengeFreigebenTest_(&ZweiKundenMengeFreigebenTest,"Zwei Kunden-Menge-Freigeben","ZKM");

static bool JumboLager(AufEintrag &AE)
{
#if defined (MANUPROC_DYNAMICENUMS_CREATED)
       LagerPlatz LP(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ);
       LagerPlatz LP2(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ+1);
       KettplanKette KK=KettplanKette(Kette(MASCHINE,SCHAERDATUM));
       std::vector<JumboRolle> JR=JumboRolle::create(KK); // 101
       assert(JR.size()==1);
       class JumboLager JL;
       Zeitpunkt_new zp0("2002-3-1 11:00"),
       		zp1("2002-3-1 11:11"),
       		zp0b("2002-3-1 11:02");
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0,true);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1,true);
       //   101 |      3 | 2002-03-01 11:00:00+01 | 2002-03-01 11:11:00+01
       JR=JumboRolle::create(KK); // 102
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0,true);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1,true);
       vergleichen(Check::Jumbo,"richtig","Jumbo richtig","");
       //  102 |      2 | 2002-03-01 11:11:00+01 | 2002-03-01 11:00:00+01
       JR=JumboRolle::create(KK); // 103
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1,true);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0,true);
       //  103 |      3 | 2002-03-01 11:00:00+01 | 2002-03-01 11:11:00+01
       JR=JumboRolle::create(KK); // 104
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1,true);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0,true);
       vergleichen(Check::Jumbo,"falsch","Jumbo falsche Reihenfolge","");
       //  104 |      2 | 2002-03-01 11:11:00+01 | 2002-03-01 11:00:00+01
       JR=JumboRolle::create(KK); // 105
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0,true);
       try // kein Log Eintrag ist richtig
       {JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0b,true);
        assert(!"Jumbo_Entnahme sollte 100 werfen");
       }catch (SQLerror &e)
       {  assert(e.Code()==100);
       }
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1,true);
       //   105 |      3 |                        | 2002-03-01 11:11:00+01
       JR=JumboRolle::create(KK); // 106
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0,true);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1,true);
       //   106 |      2 | 2002-03-01 11:11:00+01 | 
       vergleichen(Check::Jumbo,"doppelt","Jumbo doppelt Aus-/Einlagern","");
#endif
       return true;
}

static TestReihe JumboLager_(&JumboLager,"Jumbolager","J");

static bool AuftragLager(AufEintrag &AE)
{
#if defined (MANUPROC_DYNAMICENUMS_CREATED)
       LagerPlatz LP(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ);
       KettplanKette KK=KettplanKette(Kette(MASCHINE,SCHAERDATUM));
       std::vector<JumboRolle> JR=JumboRolle::create(KK); // 101
       assert(JR.size()==1);
       class JumboLager JL;
       Zeitpunkt_new zp0("2002-3-1 11:00"),
       		zp1("2002-3-1 11:11");
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0,true);
       std::vector<JumboRolle> JR2=JumboRolle::create(KK); // 102
       JL.Jumbo_Einlagern(LP,JR2.front(),JumboLager::Einlagern,UID,"TEST",&zp0,true);
       vergleichen(Check::Menge,"AL_normal_rein","Einlagern","");

       std::vector<JumboRolle> JR3=JumboRolle::create(KK); // 102
       JL.Jumbo_Einlagern(LP,JR3.front(),JumboLager::Einlagern,UID,"TEST",&zp0,false);
       std::vector<JumboRolle> JR4=JumboRolle::create(KK); // 102
       JL.Jumbo_Einlagern(LP,JR4.front(),JumboLager::Einlagern,UID,"TEST",&zp0,false);
       std::vector<JumboRolle> JR5=JumboRolle::create(KK); // 102
       JL.Jumbo_Einlagern(LP,JR5.front(),JumboLager::Einlagern,UID,"TEST",&zp0,false);
       vergleichen(Check::Menge,"AL_gefunden","Gefunden","");

       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1,true);
       JL.Jumbo_Entnahme(JR2.front(),JumboLager::Auslagern,UID,"TEST",&zp1,true);
       vergleichen(Check::Menge,"AL_normal_raus","Auslagern","");

       JL.Jumbo_Entnahme(JR3.front(),JumboLager::Auslagern,UID,"TEST",&zp1,false);
       JL.Jumbo_Entnahme(JR4.front(),JumboLager::Auslagern,UID,"TEST",&zp1,false);
       JL.Jumbo_Entnahme(JR5.front(),JumboLager::Auslagern,UID,"TEST",&zp1,false);
       vergleichen(Check::Menge,"AL_verschwunden","Verschwunden","");
#endif
       return true;
}

static TestReihe AuftragLager_(&AuftragLager,"Auftrag Lager Interaktion","AL");
#endif

static void usage(const std::string &argv0,const std::string &argv1)
{
  std::cerr << argv0 <<" [<option> ...] <test>\n";
  std::cerr << "options: --verbose|-v --analyse|-y --repair|-r\n"
  	"\t--continue|-c --trace|-t --reinit|-R --overwrite|-O\n";
  std::cerr << "tests:\n";
  for (const TestReihe *i=TestReihe::first; i; i=i->next)
  {  std::cerr << '\t' << i->kuerzel << '\t' << i->bezeichnung << '\n';
  }
  exit(1);
}

int main(int argc,char *argv[])
{  putenv("LANG=C"); // omit any locale oddities/changes
   bool clean_only=true;

   static struct option long_options[] = {
     { "verbose", no_argument, 0, 'v' },
     { "repair", no_argument, 0, 'r' },
     { "continue", no_argument, 0, 'c' },
     { "trace", no_argument, 0, 't' },
     { "clean-only", no_argument, 0, 'C' },
     { "reinit", no_argument, 0, 'R' },
     { "analyse", no_argument, 0, 'y' },
     { "overwrite", no_argument, 0, 'O' },
     { 0,0,0,0 },
   };
   
   int opt;
   while ((opt=getopt_long(argc,argv,"vrctCRyO",long_options,0))!=-1)
    switch(opt)
   {  case 'v': verbose=true; break;
      case 'r': Check::reparieren=true; break;
      case 'c': do_not_stop=true; break;
      case 'C': clean_only=true; break;
      case 'R': clean_only=false; break;
      case 'y': Check::analyse=true; break;
      case 't': ManuProC::Tracer::Enable(AuftragBase::trace_channel);
      	ManuProC::Tracer::Enable(log_trace);
      	break;
      case 'O': Check::overwrite=true; break;
      default: usage(argv[0],""); return 1;
   }

   if(argc-optind!=1) { usage(argv[0],""); return 1; }
   std::string mode_str=argv[optind];

#ifdef MANU_PROC_TEST
   putenv("PGDATABASE=anleitungdb");
#elif defined MABELLA_TEST
   putenv("PGDATABASE=mabella_testdb");
#elif defined PETIG_TEST
   putenv("PGDATABASE=testdb");
#endif

  TestReihe *mode=0;
  for (mode=TestReihe::first; mode; mode=mode->next)
  {  if (mode_str==mode->kuerzel) break; 
  }
   if(!mode) { usage(argv[0],mode_str); return 1; }

   std::cout << "Initalisierung der Datenbank ...";
   std::cout.flush();

   if (clean_only) // test for existance
   {  try
      {  ManuProC::dbconnect();
         Kunde k(1);
         ManuProC::dbdisconnect();
      }
      catch (SQLerror &e)
      {  clean_only=false;
      }
   }

   std::string kill_output=" 2>/dev/null >/dev/null";
   if (verbose) kill_output="";

   if (clean_only)
   {  system((std::string("./clean_db ")+getenv("PGDATABASE")+kill_output).c_str());
      if (!access((std::string(MANU_DATAPATH)+"/clean_db2").c_str(),X_OK))
         system((std::string("cd ")+MANU_DATAPATH+"; ./clean_db2 "+getenv("PGDATABASE")+kill_output).c_str());
   }
   else
      system((std::string(MANU_DATAPATH)+"/initdb.script "+MANU_DATAPATH+kill_output).c_str());
   std::cout << "...beendet\n";

   Petig::PrintUncaughtExceptions();
   try{
   mkdir("files.log",0777);
   mkdir("results",0777);
   std::ofstream logstream(("files.log/"+mode_str).c_str());
   testlog=&logstream;
   ManuProC::dbconnect();

   DataBase_init();
   return auftragstests(mode);

   }catch(SQLerror &e){std::cout << e<<'\n';}
  return 1;
}

