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

#ifdef PETIG_TEST
#include "AuftragsVerwaltung.hh"
#include "Check.hh"
#include "DataBase_init.hh"
#include <Lager/Lager.h>
#include <Auftrag/Auftrag.h>
#include <Misc/Trace.h>
#include <unistd.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>
//#include <Misc/inbetween.h>
#include "TestReihe.h"

#ifdef PETIG_EXTENSIONS
#include <Lager/RohwarenLager.h>
#include <Lager/JumboLager.h>
#include <Ketten/KettplanKette.h>
#endif

static bool Zusatzinfo()
{	
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
        AufEintragBase AEB2=auftrag.push_back(4000,DATUM,ARTIKEL_FAERBEREI,OPEN,true);
        AufEintragBase AEB3=auftrag.push_back(4000,DATUM+10,ARTIKEL_FAERBEREI,OPEN,true);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI_Ausgangspunkt","Ausgangspunkt","");
        
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       int lznr=liefs.push_back(ARTIKEL_FAERBEREI,9,1000);
       LieferscheinEntryBase lsb(liefs,lznr);
       LieferscheinEntry(lsb).changeStatus(OPEN,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI_Auslieferung","Auslieferung","L");
       
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
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0,true);
       std::vector<JumboRolle> JR2=JumboRolle::create(KK); // 101
       JL.Jumbo_Einlagern(LP,JR2.front(),JumboLager::Einlagern,"TEST",&zp0,true);

   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
       AufEintragBase AEB3=auftrag.push_back(170,DATUM,ARTIKEL_BANDLAGER,OPEN,true);
       
       AufEintragBase AEB2=auftrag.push_back(17500,DATUM,ARTIKEL_BANDLAGER,OPEN,true);
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       AufEintrag ae(AEB2);
       int lznr=liefs.push_back(ae,ARTIKEL_BANDLAGER,9,1000);
       LieferscheinEntryBase lsb(liefs,lznr);
       LieferscheinEntry(lsb).changeStatus(OPEN,liefs,false);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1,true);
       JL.Jumbo_Entnahme(JR2.front(),JumboLager::Auslagern,"TEST",&zp1,true);
       JumboRolle jr(JumboRolle::Pruefziffer_anhaengen(100));
       JL.Jumbo_Entnahme(jr,JumboLager::Auslagern,"TEST",&zp1,true);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI2_Ausgangspunkt","init","");
       
       LieferscheinEntry(lsb).changeMenge(4,1000,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"ZI2_Problem","Mengenänderung","-");
       return true;
}

static TestReihe Zusatzinfo2_(&Zusatzinfo2,"komplexe Lieferscheinveränderung mit Prod-Selbst-Lager","ZI2");

static bool Mengentest(AufEintrag &AE)
{  AuftragsVerwaltung auftrag;
      // Menge des Auftrags erhöhen
      auftrag.kunden_bestellmenge_aendern(AE,500);
      vergleichen(Check::Menge,"menge_plus", "Erhöhen der Auftragmenge","");

      // Menge des Auftrags erniedrigen (Rohwarenlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AE,100);
      vergleichen(Check::Menge,"menge_minus","Reduzieren der Auftragmenge unter Rohwarenlagerbestand","");

      AE.updateLieferdatum(NEWDATUM);
      vergleichen(Check::Menge,"datumsaenderung","Datumsänderung","D");

      // Menge des Auftrags weiter erniedrigen (Bandlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AE,10);
      vergleichen(Check::Menge,"menge_minus_bandlager","Reduzieren der Auftragmenge unter Bandlagerbestand","");

      AE.setStatus(CLOSED);
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

       AEP.Planen(100,PA,PLANDATUM5);
       vergleichen(Check::Menge,"planen_acetat","Planen des Acetateinkaufs","A");
       }
       ManuProC::Trace(log_trace,__FILELINE__);

       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));

       ManuProC::Trace(log_trace,__FILELINE__);
       AEP.Planen(7000,PA,PLANDATUM4);
       vergleichen(Check::Menge,"planen_faerberei_teil","Teil-Planen der Färberei","F");
       }
       ManuProC::Trace(log_trace,__FILELINE__);
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       AEP.Planen(5000,PA,PLANDATUM6);
       vergleichen(Check::Menge,"planen_webereiP","Planen der Weberei","P");
       }
       ManuProC::Trace(log_trace,__FILELINE__);

       Lieferschein liefs(ppsInstanzID::_Garn__Einkauf,cH_Kunde(Kunde::eigene_id));
       LieferscheinEntryBase lseb(liefs,liefs.push_back(ARTIKEL_ACETAT,1,66));
       LieferscheinEntry(lseb).changeStatus(OPEN,liefs,false);
       vergleichen(Check::Menge,"planen_einkauf_lieferschein","Lieferschein mit Teillieferung für Einkauf anlegen","L");
       return true;
}

static TestReihe Plantest_(&Plantest,"Planungs Test","P");

static bool Splittest(AufEintrag &AE)
{
      AE.split(300,SPLITDATUM);
      vergleichen(Check::Menge,"split","Splitten einer Auftragszeile","");
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,dummystring,false,true);
std::cout << dummystring<<'\n';
      vergleichen(Check::Menge,"split_rohwarenlager_rein","Rohwarenlager einlagern\n","+");

      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,dummystring,false,false,true);
std::cout << dummystring<<'\n';
      vergleichen(Check::Menge,"split_rohwarenlager_raus","Rohwarenlager auslagern\n","-");
      return true;
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
       AEP.Planen(17000,PA,DATUMP);
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
       AE.MengeAendern(-100,true,AufEintragBase());

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
          AuftragsVerwaltung auftrag;
       AufEintragBase AEB=auftrag.anlegen2();
       vergleichen(Check::Menge,"rep_zwei_auftraege_anlegen","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"]","");
      {
       Auftrag PA=Auftrag(Auftrag::Anlegen(SPRITZGIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
       AufEintrag AEP((AufEintragBase(SPRITZGIESSEREI,AuftragBase::ungeplante_id,1)));
       AEP.Planen(200,PA,PLANDATUM5);
       vergleichen(Check::Menge,"rep_planen_spritz","Planen der Spritzgießerei (Reparatur)","");
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(SPRITZGIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
       AufEintrag AEP((AufEintragBase(SPRITZGIESSEREI,AuftragBase::ungeplante_id,1)));
       AEP.Planen(5000,PA,PLANDATUM5);
       vergleichen(Check::Menge,"rep_planen_spritz2","Planen der Spritzgießerei (Reparatur)","");
       }
       {
        Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
        LieferscheinEntryBase lseb(liefs,liefs.push_back(ARTIKEL_ROLLEREI,390,1));
        LieferscheinEntry(lseb).changeStatus(OPEN,liefs,false);
        vergleichen(Check::Menge,"rep_lieferschein","Reparatur-Lieferschein anlegen","L");
       }
       return true;
}

static TestReihe Rep_Kunden_Zuordnungen_(&Rep_Kunden_Zuordnungen,"Reparatur Kunden Zuordnungen","RKZ");

static bool Lagertest(AufEintrag &AE)
{
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,dummystring,false,true);
      vergleichen(Check::Menge,"rohwarenlager_rein","Rohwarenlager einlagern\n","+");

      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,dummystring,false,false,true);
std::cout << "D1: "<<dummystring<<'\n';
      RohwarenLager::st_rohlager stRL3(LagerPlatzKupfer,2,10,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL3,dummystring,false,false,true);
std::cout << "D2:" <<dummystring<<'\n';
      vergleichen(Check::Menge,"rohwarenlager_raus","Rohwarenlager auslagern\n","-");

      Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
      int weberei_znr=1;
      AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
      assert(AEP.getStueck()==AEP.getRestStk());
      AEP.Planen(5000,PA,PLANDATUM5);
      vergleichen(Check::Menge,"planen_weberei_fuer_lager","Planen der Weberei zum späteren Test des Bandlagers","W");

      DataBase_init::createJumbo(-10,12000,true);
std::cout << dummystring<<'\n';
      vergleichen(Check::Menge,"bandlager_rein","Bandlager einlagern\n","b");

        AE.ProduziertNG(300,LieferscheinEntryBase());
      vergleichen(Check::Menge,"kunde_teillieferung","Kunde Teillieferung\n","T");

//std::cout << "\n\n120 ABSCHREIBEN\n\n\n";
        AE.ProduziertNG(120,LieferscheinEntryBase());
      vergleichen(Check::Menge,"kunde_ueberlieferung","Kunde Überlieferung\n","Ü");


      // test von force, leer, etc
#if 1
      dummystring="";
      RohwarenLager::st_rohlager stRL10(LagerPlatzKupfer2,6,35,1,7,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL10,dummystring,true,true);
std::cout << "D10: "<<dummystring<<'\n';

      dummystring="";
      RohwarenLager::st_rohlager stRL11(LagerPlatzKupfer,0,0,2,1,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,dummystring,false,false,true);
std::cout << "D11: "<<dummystring<<'\n';
      vergleichen(Check::Menge|Check::RohLager,"force_art","force, falscher Artikel","");

      dummystring="";
      RohwarenLager::st_rohlager stRL12(LagerPlatzKupfer2,10,35,0,0,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,dummystring,false,false,true);
std::cout << "D12: "<<dummystring<<'\n';

      dummystring="";
      RohwarenLager::st_rohlager stRL13(LagerPlatzAcetat,2,7,0,0,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,dummystring,false,true,true);
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
{  AuftragsVerwaltung auftrag;
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(13000,PA,PLANDATUM6);
       vergleichen(Check::Menge,"planen_fuer_zweiten_auftrag","Über-Planen der Färberei","P");
       }
       AufEintragBase AEB=auftrag.anlegen2();
       vergleichen(Check::Menge,"zwei_auftraege_anlegen","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"]","Z");
       return true;
}

static TestReihe ZweiAuftraege_(&ZweiAuftraege,"Zwei Aufträge","Z");

static bool ZweiterAuftrag_frueheresDatum(AufEintrag &AE)
{  AuftragsVerwaltung auftrag;
       AufEintragBase AEB=auftrag.anlegen3();

       vergleichen(Check::Menge,"zwei_auftraege_datum","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"] mit früherem Liefertermin","D");

       {
          AufEintrag AE(AEB);
        AE.ProduziertNG(200,LieferscheinEntryBase());
       }
       vergleichen(Check::Menge,"zwei_auftraege_datum_abschreiben","Teil-Abschreiben des zweiten Auftrags ["+AEB.str()+"]","A");


       AufEintrag(AEB).setStatus(CLOSED);
       vergleichen(Check::Menge,"zwei_auftraege_datum_closed","Statussänderung(2) (Closed)","C");

       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       assert(AEP.getStueck()==AEP.getRestStk());
       AEP.Planen(7000,PA,PLANDATUM5);
       vergleichen(Check::Menge,"zwei_auftraege_weberei_planen","Planen der Weberei","P");

       AE.setStatus(CLOSED);
// OHNE ReparaturProgramm, da CLOSED-Kundedenaufträge noch nicht nach
//  unten korrigiert werden
       vergleichen(Check::Menge,"erster_auftrag_closed","Statussänderung(1) (Closed)","CL",false);
       return true;
}

static TestReihe ZweiterAuftrag_frueheresDatum_(&ZweiterAuftrag_frueheresDatum,"Zwei Aufträge, zweiter Auftrag früheres Datum","D");

static bool Lieferscheintest(AufEintrag &AE)
{
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       LieferscheinEntryBase lseb(liefs,liefs.push_back(ARTIKEL_ROLLEREI,150));
       LieferscheinEntry(lseb).changeStatus(OPEN,liefs,false);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_teillieferung","Lieferschein mit Teillieferung anlegen","");

       LieferscheinEntry le(lseb);
       LieferscheinEntry::deleteEntry(le);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_zeileloeschen","Lieferscheinzeile löschen","");

       lseb=LieferscheinEntryBase(liefs,liefs.push_back(ARTIKEL_ROLLEREI,450));
       LieferscheinEntry(lseb).changeStatus(OPEN,liefs,false);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_volllieferung","Lieferschein mit Volllieferung","V");

       LieferscheinEntry le2(lseb);
       LieferscheinEntry::deleteEntry(le2);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_zeileloeschen2","Lieferscheinzeile nochmal löschen","");
       return true;
}

static TestReihe Lieferscheintest_(&Lieferscheintest,"Lieferschein","Ls");

static bool LieferscheintestMenge(AufEintrag &AE)
{
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       LieferscheinEntryBase lseb(liefs,liefs.push_back(ARTIKEL_ROLLEREI,150));
       LieferscheinEntry(lseb).changeStatus(OPEN,liefs,false);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_teillieferung","Lieferschein mit Teillieferung anlegen","");

       int stueck=140;
       AuftragBase::mengen_t menge=0;
       LieferscheinEntry(lseb).changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_mengenaenderung_minus","Lieferschein Mengenaenderung Minus","");

       stueck=400;
       LieferscheinEntry(lseb).changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Menge|Check::Lieferschein,"LS_mengenaenderung_plus","Lieferschein Mengenaenderung Plus","");
       return true;
}

static TestReihe LieferscheintestMenge_(&LieferscheintestMenge,"Lieferschein Menge","Lm");

static bool Lieferscheintest_ZweiterAuftrag_frueheresDatum(AufEintrag &AE)
{  AuftragsVerwaltung auftrag;
       AufEintragBase AEB=auftrag.anlegen3();
       vergleichen(Check::Menge,"zwei_auftraege_datum","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"] mit früherem Liefertermin","D");

       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       LieferscheinEntryBase lseb(liefs,liefs.push_back(ARTIKEL_ROLLEREI,50));
       LieferscheinEntry(lseb).changeStatus(OPEN,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZA","Lieferschein mit Teillieferung und 2 Aufträgen anlegen","T");

       lseb=LieferscheinEntryBase(liefs,liefs.push_back(ARTIKEL_ROLLEREI,600));
       LieferscheinEntry(lseb).changeStatus(OPEN,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZAV","Lieferschein mit Volllieferung und 2 Aufträgen anlegen","V");
       return true;
}

static TestReihe Lieferscheintest_ZweiterAuftrag_frueheresDatum_(&Lieferscheintest_ZweiterAuftrag_frueheresDatum,"Zwei Aufträge, zweiter Auftrag früheres Datum","LA");

static bool LieferscheintestZusatz(AufEintrag &AE)
{
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       int lznr=liefs.push_back(ARTIKEL_ROLLEREI,550);
       LieferscheinEntryBase lseb(liefs,lznr);
       LieferscheinEntry(lseb).changeStatus(OPEN,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZ","Lieferschein mit Zusatzeintrag anlegen","");

       int stueck=633;
       AuftragBase::mengen_t menge=0;

       LieferscheinEntry(lseb).changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZP","Lieferscheinentry mit Zusatzeintrag Plus","+");

       stueck=450;
       LieferscheinEntry(lseb).changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZM","Lieferscheinentry mit Zusatzeintrag Minus","-");

       stueck=350;
       LieferscheinEntry(lseb).changeMenge(stueck,menge,liefs,false);
       vergleichen(Check::Lieferschein|Check::Menge,"LSZMK","Lieferscheinentry mit Zusatzeintrag Minus Kunde","-K");
       return true;
}

static TestReihe _LieferscheintestZusatz(&LieferscheintestZusatz,"Lieferschein Zusatz","LZ");

static bool ZweiKundenMengeFreigebenTest(AufEintrag &AE)
{  AuftragsVerwaltung auftrag;
       AufEintragBase AEB2=auftrag.anlegenK();
       vergleichen(Check::Menge,"ZK_anlegen","Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["+AEB2.str()+"]","");

       {
         cH_ppsInstanz I(ppsInstanzID::Bandlager);
         int znrvon=1;
         AufEintragBase Von(AuftragBase(I,AuftragBase::plan_auftrag_id),znrvon);
         int znrnach=2;
         AufEintrag Fuer((class AufEintragBase(AuftragBase(I,AuftragBase::ungeplante_id),znrnach)));
// hmmm was sollte das tun?         
//#warning Test neu designen
//         AufEintrag(Von).menge_fuer_aeb_freigeben(3000,Fuer);
//         vergleichen(Check::Menge,"ZKM","Menge freigeben für einen anderen Auftrag","");
       }
       return true;
}

static TestReihe ZweiKundenMengeFreigebenTest_(&ZweiKundenMengeFreigebenTest,"Zwei Kunden-Menge-Freigeben","ZKM");

static bool JumboLager()
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
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0,true);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1,true);
       //   101 |      3 | 2002-03-01 11:00:00+01 | 2002-03-01 11:11:00+01
       JR=JumboRolle::create(KK); // 102
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0,true);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,"TEST",&zp1,true);
       vergleichen(Check::Jumbo|Check::Menge,"richtig","Jumbo richtig","");
       //  102 |      2 | 2002-03-01 11:11:00+01 | 2002-03-01 11:00:00+01
       JR=JumboRolle::create(KK); // 103
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1,true);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0,true);
       //  103 |      3 | 2002-03-01 11:00:00+01 | 2002-03-01 11:11:00+01
       JR=JumboRolle::create(KK); // 104
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,"TEST",&zp1,true);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0,true);
       vergleichen(Check::Jumbo,"falsch","Jumbo falsche Reihenfolge","");
       //  104 |      2 | 2002-03-01 11:11:00+01 | 2002-03-01 11:00:00+01
       JR=JumboRolle::create(KK); // 105
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0,true);
       try // kein Log Eintrag ist richtig
       {JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0b,true);
        assert(!"Jumbo_Entnahme sollte 100 werfen");
       }catch (SQLerror &e)
       {  assert(e.Code()==100);
       }
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1,true);
       //   105 |      3 |                        | 2002-03-01 11:11:00+01
       JR=JumboRolle::create(KK); // 106
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0,true);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,"TEST",&zp1,true);
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
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0,true);
       std::vector<JumboRolle> JR2=JumboRolle::create(KK); // 102
       JL.Jumbo_Einlagern(LP,JR2.front(),JumboLager::Einlagern,"TEST",&zp0,true);
       vergleichen(Check::Menge,"AL_normal_rein","Einlagern","");

       std::vector<JumboRolle> JR3=JumboRolle::create(KK); // 102
       JL.Jumbo_Einlagern(LP,JR3.front(),JumboLager::Einlagern,"TEST",&zp0,false);
       std::vector<JumboRolle> JR4=JumboRolle::create(KK); // 102
       JL.Jumbo_Einlagern(LP,JR4.front(),JumboLager::Einlagern,"TEST",&zp0,false);
       std::vector<JumboRolle> JR5=JumboRolle::create(KK); // 102
       JL.Jumbo_Einlagern(LP,JR5.front(),JumboLager::Einlagern,"TEST",&zp0,false);
       vergleichen(Check::Menge,"AL_gefunden","Gefunden","");

       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1,true);
       JL.Jumbo_Entnahme(JR2.front(),JumboLager::Auslagern,"TEST",&zp1,true);
       vergleichen(Check::Menge,"AL_normal_raus","Auslagern","");

       JL.Jumbo_Entnahme(JR3.front(),JumboLager::Auslagern,"TEST",&zp1,false);
       JL.Jumbo_Entnahme(JR4.front(),JumboLager::Auslagern,"TEST",&zp1,false);
       JL.Jumbo_Entnahme(JR5.front(),JumboLager::Auslagern,"TEST",&zp1,false);
       vergleichen(Check::Menge,"AL_verschwunden","Verschwunden","");
#endif
       return true;
}

static TestReihe AuftragLager_(&AuftragLager,"Auftrag Lager Interaktion","AL");
#endif
