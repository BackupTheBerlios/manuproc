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

#include "DataBase_init.hh"
#include "AuftragsVerwaltung.hh"
#include "Check.hh"
#include "steuerprogramm.hh"
#include <Aux/dbconnect.h>
#include <Aux/exception.h>
#include <Lager/Lager.h>
#include <Auftrag/Auftrag.h>
#include <Instanzen/ppsInstanzProduziert.h>
#include <Misc/Trace.h>
#include <unistd.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>

#include <fstream>
#include <sys/stat.h>

#ifdef PETIG_EXTENSIONS
#include <Lager/RohwarenLager.h>
#include <Lager/JumboLager.h>
#include <Ketten/KettplanKette.h>
#endif

enum e_mode {None,Mengentest,Plantest,Lagertest,Splittest,ZweiAuftraege,
      ZweiterAuftrag_frueheresDatum,Lieferscheintest,LieferscheintestMenge,
      LieferscheintestZusatz,Lieferscheintest_ZweiterAuftrag_frueheresDatum,
      LieferscheinJacek,
      ZweiKundenTest,ZweiKundenMengeFreigebenTest,ManuProCTest,
      JumboLager,Rep_Mabella,Rep_Petig_PhysikalischesLager,
      Rep_Petig_0er_2er_gleichzeitig,Rep_KundenProgramm,Rep_Zuordnungen,
      Rep_Kunden_Zuordnungen};

static ostream *testlog;
// for more output ...
bool verbose=false;

static void vergleichen(Check &C, Check::was_checken w, const std::string &zusatz,
        const std::string &name, bool mit_reparatur_programm, bool vor_dem_test_reparieren=false)
{   (*testlog) << int(w) << ' ' << zusatz << ' ' << name << '\n';
    bool erfolgreich=C.teste(w,zusatz,mit_reparatur_programm,vor_dem_test_reparieren);
    if(!erfolgreich)
    { cout << name << " fehlgeschlagen\n\n"; exit(1); }
}

int auftragstests(e_mode mode)
{
//  ManuProC::Tracer::Enable(AuftragBase::trace_channel);

   bool mit_reparatur_programm=false;
#ifdef REPARATUR_PROGRAMM_TESTEN
   mit_reparatur_programm=true;
   cout << "Mit ReparaturProgrammTest\n";
#endif

   AuftragsVerwaltung  auftrag;
   AufEintragBase AEB=auftrag.anlegen();
   AufEintrag AE=AEB;
   Check C;
//   bool erfolgreich=false;
   cout << "Anlegen eines Auftrags ["<<AEB<<"] beendet\n\n";

   // ANLEGEN eines Auftrags mit Bandlager und Rohwarenlager
   if (mode!=JumboLager)
   {
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
      AE.setStatus(OPEN,UID);
      vergleichen(C,Check::Menge,"_mit_lager_open","Öffnen des Auftrags",mit_reparatur_programm);
   }

   switch(mode) {
    case ManuProCTest :
     {
      #ifdef MANU_PROC_TEST

      {// Planen des Einkaufs (Granulat, grün)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=4;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,200,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      vergleichen(C,Check::Menge,"_planen_kupfer","Planen des Einkaufs(Granulat)",mit_reparatur_programm);

      // Spezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
      liefs.push_back(PAE,ARTIKEL_GRANULAT_GRUEN,1,200,0);
//unspezifisch   liefs.push_back(ARTIKEL_GRANULAT_GRUEN,1,200,0);
      vergleichen(C,Check::Menge|Check::Lieferschein,"_LS_teillieferung","Lieferschein (mit AEB, Granulat) anlegen",mit_reparatur_programm);
      }

      {// Planen des Einkaufs (Metall)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=5;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,10,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      vergleichen(C,Check::Menge,"_planen_weberei_fuer_lager","Planen des Einkaufs(Metall)",mit_reparatur_programm);


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_METALL,1,5,0);
      vergleichen(C,Check::Lieferschein|Check::Menge,"_LS_volllieferung","Lieferschein (unbestimmt, Metall) anlegen",mit_reparatur_programm);
      }

      {// Planen der Gießerei (ARTIKEL_GRIFF_ROT)
      Auftrag PA=Auftrag(Auftrag::Anlegen(GIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
      int znr=1;
      AufEintrag AEP((AufEintragBase(GIESSEREI,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,500,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      vergleichen(C,Check::Menge,"_planen_faerberei_teil","Planen der Giesserei (Griff rot)",mit_reparatur_programm);


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(GIESSEREI,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_GRIFF_ROT,500,0,0);
      vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZ","Lieferschein (Gießerei) anlegen",mit_reparatur_programm);
      }

      {// Produktion in der Werkstatt ohne daß vorher etwas geplant wurde
//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);
      ManuProC::st_produziert p(ARTIKEL_SCHRAUBENZIEHER_GELB,600,UID,ManuProC::DefaultValues::EigeneKundenId);
      cH_ppsInstanz I(WERKSTATT);
      I->Produziert(p);
      vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZP","Produktion in der Werkstatt anlegen",mit_reparatur_programm);
      }

      {// Lieferscheinschreiben für das Schraubenzieherlager => auslagern
      Lieferschein liefs(SCHRAUBENZIEHERLAGER,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);
      liefs.push_back(ARTIKEL_SCHRAUBENZIEHER_ROT,450,0,0);
      vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZM","Lieferschein für das Rohwarenlager (auslagern)",mit_reparatur_programm);
      }

      {// Lieferscheinschreiben für den Kunden
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,450,0,0);
      vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZMK","Lieferschein für den Kunden",mit_reparatur_programm);
      }

      {// Lieferscheinschreiben für den Kunden  (Überlieferung)
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,60,0,0);
      vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZA","Lieferschein für den Kunden (Überlieferung)",mit_reparatur_programm);
      }

      cout << "ManuProcTest erfolgreich\n";
      #endif
      break;
     }
    case Mengentest :
     {
      // Menge des Auftrags erhöhen
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
      auftrag.kunden_bestellmenge_aendern(AEB,500);
      vergleichen(C,Check::Menge,"_menge_plus", "Erhöhen der Auftragmenge",mit_reparatur_programm);

//ManuProC::Tracer::Enable(AuftragBase::trace_channel);

      // Menge des Auftrags erniedrigen (Rohwarenlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,100);
      vergleichen(C,Check::Menge,"_menge_minus","Reduzieren der Auftragmenge unter Rohwarenlagerbestand",mit_reparatur_programm);

      AE.updateLieferdatum(NEWDATUM,UID);
      vergleichen(C,Check::Menge,"_datumsaenderung","Datumsänderung",mit_reparatur_programm);

//exit(1);

      // Menge des Auftrags weiter erniedrigen (Bandlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,10);
      vergleichen(C,Check::Menge,"_menge_minus_bandlager","Reduzieren der Auftragmenge unter Bandlagerbestand",mit_reparatur_programm);

      AufEintrag(AEB).setStatus(CLOSED,UID);
      vergleichen(C,Check::Menge,"_status_closed","Statussänderung (Closed)",mit_reparatur_programm);

      cout << "Mengen Test erfolgreich\n";

      break;
     }
    case Plantest:
     {
      #ifdef PETIG_TEST
cout << "A\n";
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::_Garn__Einkauf),Kunde::default_id);
       int kupfer_znr=2;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::_Garn__Einkauf,AuftragBase::ungeplante_id,kupfer_znr));

       AEP.Planen(UID,100,PA,PLANDATUM5);
       vergleichen(C,Check::Menge,"_planen_kupfer","Planen des Kupfereinkaufs",mit_reparatur_programm);
       }
cout << "B\n";

       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
ManuProC::Tracer::Enable(AuftragBase::trace_channel);
cout << "LOS GEHTS\n";
       AEP.Planen(UID,7000,PA,PLANDATUM4);
       vergleichen(C,Check::Menge,"_planen_faerberei_teil","Teil-Planen der Färberei",mit_reparatur_programm);
       }
cout << "C\n";
exit(0);
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       AEP.Planen(UID,5000,PA,PLANDATUM6);
       vergleichen(C,Check::Menge,"_planen_webereiP","Planen der Weberei",mit_reparatur_programm);
       }

#ifdef PETIG_TEST
       Lieferschein liefs(ppsInstanzID::_Garn__Einkauf,cH_Kunde(Kunde::eigene_id));
//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);
       liefs.push_back(ARTIKEL_ACETAT,1,66,0);
       vergleichen(C,Check::Menge,"_planen_einkauf_lieferschein","Lieferschein mit Teillieferung für Einkauf anlegen",mit_reparatur_programm);
#endif

      cout << "Plan-Test erfolgreich\n";

      break;
      #endif
     }
    case Splittest :
    case Rep_Petig_PhysikalischesLager:
     {
      AE.split(UID,300,SPLITDATUM);
      vergleichen(C,Check::Menge,"_split","Splitten einer Auftragszeile",mit_reparatur_programm);
#ifdef PETIG_TEST
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,UID,dummystring);
std::cout << dummystring<<'\n';
      vergleichen(C,Check::Menge,"_split_rohwarenlager_rein","Rohwarenlager einlagern\n",mit_reparatur_programm);

      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,UID,dummystring);
std::cout << dummystring<<'\n';
      vergleichen(C,Check::Menge,"_split_rohwarenlager_raus","Rohwarenlager auslagern\n",mit_reparatur_programm);
      cout << "Split-Test erfolgreich\n";

      if(mode!=Rep_Petig_PhysikalischesLager) break;
      #ifndef REPARATUR_PROGRAMM_TESTEN
        assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
      #endif
      // Physikalisches Lager ändern
//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);
      std::string qB="insert into rl_inhalt (position_,kartons,reste,kg_per_karton,"
         " material,eingelagert,rest_kg)"
         " values ('07D5',10,0,5,211007,'2002-11-28',0)";
      Query::Execute(qB);
      SQLerror::test(__FILELINE__);
      vergleichen(C,Check::Menge,"_split_reparatur","Reparatur-Split-Test (Garnlager)",mit_reparatur_programm,true);

      std::string qJ="insert into rohjumbo (code,maschine,soll_meter,plan_datum,"
         " status,lauf,gang,wiederinslager,artikelid,rest,lagerplatz) "
         " values (101,212,700,'2002-11-28',2,1,1,'2002-01-01 12:00:00+01',"
         " 123755,false,712)";
      Query::Execute(qJ);
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
      SQLerror::test(__FILELINE__);
      vergleichen(C,Check::Menge,"_split_reparatur_bandlager","Reparatur-Split-Test (Bandlager)",mit_reparatur_programm,true);

      std::string qJ2="update rohjumbo set soll_meter=200 where "
         "(code,maschine,plan_datum,"
         " status,lauf,gang,wiederinslager,artikelid,rest,lagerplatz) "
         " = (101,212,'2002-11-28',2,1,1,'2002-01-01 12:00:00+01',"
         " 123755,false,712)";
      Query::Execute(qJ2);
      SQLerror::test(__FILELINE__);
      vergleichen(C,Check::Menge,"_split_reparatur_bandlager_minus","Reparatur-Split-Test (BandlagerMinus)",mit_reparatur_programm,true);

      std::string qB2="update rl_inhalt set kartons=4 where "
         " (position_,reste,kg_per_karton,"
         " material,eingelagert,rest_kg)"
         " = ('07D5',0,5,211007,'2002-11-28',0)";
      Query::Execute(qB2);
      SQLerror::test(__FILELINE__);
      vergleichen(C,Check::Menge,"_split_reparatur_garnlager_minus","Reparatur-Split-Test (GarnlagerMinus)",mit_reparatur_programm,true);

      cout << "Reparatur-Test (Petig, Physikalisches Lager) erfolgreich\n";

#endif
      break;
     }
    case Rep_Zuordnungen:
     {
      #ifdef PETIG_TEST
       #ifndef REPARATUR_PROGRAMM_TESTEN
          assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
       #endif
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,1));
       AEP.Planen(UID,17000,PA,DATUMP);
       vergleichen(C,Check::Menge,"_rep_pf","Reparatur-Planen der Färberei",mit_reparatur_programm);
       }
      {
        Query::Execute("update auftragentry set bestellt=5555 where "
            "(instanz,auftragid)=(2,2)");
        SQLerror::test(__FILELINE__);
        vergleichen(C,Check::Menge,"_rep_pf","Reparatur-Zuordungen (1)",mit_reparatur_programm,true);
      }
      {
        Query::Execute("update auftragentry set bestellt=6555 where "
            "(instanz,auftragid)=(2,2)");
        SQLerror::test(__FILELINE__);
        Query::Execute("update auftragsentryzuordnung set menge=6555 where "
            "(altinstanz,altauftragid,neuinstanz)=(2,2,2)");
        SQLerror::test(__FILELINE__);
        vergleichen(C,Check::Menge,"_rep_pf","Reparatur-Zuordungen (2)",mit_reparatur_programm,true);
      }
      {
        Query::Execute("update auftragentry set geliefert=16555 where "
            "(instanz,auftragid)=(2,20000)");
        SQLerror::test(__FILELINE__);
        vergleichen(C,Check::Menge,"_rep_pfE","Reparatur-Zuordungen ()",mit_reparatur_programm,true);
      }
      cout << "Reparatur-Test (Petig, Zuordnungen) erfolgreich\n";
      break;
      #endif
     }
    case Rep_KundenProgramm:
     {
       #ifdef PETIG_TEST
       #ifndef REPARATUR_PROGRAMM_TESTEN
          assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
       #endif
      {
       std::string q1="update auftragentry set artikelid="+itos(ARTIKEL_ROLLEREIK.Id())+" where "
                         " (auftragid,zeilennr,instanz) = (0,1,10)";
       Query::Execute(q1);
       SQLerror::test(__FILELINE__);

       vergleichen(C,Check::Menge,"_reparatur_kunde","Reparatur Kundenaufträge (Artikel)",mit_reparatur_programm,true);
      }
      {
       std::string q2="update auftragsentryzuordnung set menge=300 where "
                         " (altauftragid) = (20000)";
       Query::Execute(q2);
       SQLerror::test(__FILELINE__);
       AufEintrag AE=AufEintrag(class AufEintragBase(class AuftragBase(ROLLEREI,AuftragBase::ungeplante_id),2));
       AE.updateStkDiff__(UID,-100,true,ManuProC::Auftrag::r_Anlegen);

       vergleichen(C,Check::Menge,"_reparatur_kunde","Reparatur Kundenaufträge (Menge)",mit_reparatur_programm,true);
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

       vergleichen(C,Check::Menge,"_reparatur_kunde_instanz","Reparatur Kundenaufträge (Instanz)",mit_reparatur_programm,true);
      }
      {
       std::string q2="update auftragentry set lieferdate='2009-12-31' where "
                         " (instanz) = (1)";
       Query::Execute(q2);
       SQLerror::test(__FILELINE__);
       vergleichen(C,Check::Menge,"_reparatur_kunde_datum","Reparatur Kundenaufträge (Datum)",mit_reparatur_programm,true);
      }
       cout << "Reparatur Kundeaufträge (Artikel, Instanz) erfolgreich\n";

       break;
      #endif
     }
    case Rep_Kunden_Zuordnungen:
     {
      #ifdef PETIG_TEST
       #ifndef REPARATUR_PROGRAMM_TESTEN
          assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
       #endif
       AufEintragBase AEB=auftrag.anlegen2();
       vergleichen(C,Check::Menge,"_rep_zwei_auftraege_anlegen","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"]",mit_reparatur_programm);
      {
       Auftrag PA=Auftrag(Auftrag::Anlegen(SPRITZGIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
       AufEintrag AEP((AufEintragBase(SPRITZGIESSEREI,AuftragBase::ungeplante_id,1)));
       int nznr=AEP.Planen(UID,200,PA,PLANDATUM5);
       vergleichen(C,Check::Menge,"_rep_planen_spritz","Planen der Spritzgießerei (Reparatur)",mit_reparatur_programm);
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(SPRITZGIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
       AufEintrag AEP((AufEintragBase(SPRITZGIESSEREI,AuftragBase::ungeplante_id,1)));
       int nznr=AEP.Planen(UID,5000,PA,PLANDATUM5);
       vergleichen(C,Check::Menge,"_rep_planen_spritz2","Planen der Spritzgießerei (Reparatur)",mit_reparatur_programm);
       }
       {
        Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
        liefs.push_back(ARTIKEL_ROLLEREI,390,1,0);
        vergleichen(C,Check::Menge,"_rep_lieferschein","Reparatur-Lieferschein anlegen",mit_reparatur_programm);
       }

#warning: TODO: 'adjust_store KK'
       cout << "Reparatur Kunden Zuordnungen erfolgreich\n";
       break;
      #endif
     }
    case Lagertest :
     {
#ifdef PETIG_TEST
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,UID,dummystring);
      vergleichen(C,Check::Menge,"_rohwarenlager_rein","Rohwarenlager einlagern\n",mit_reparatur_programm);

      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,UID,dummystring);
std::cout << "D1: "<<dummystring<<'\n';
      RohwarenLager::st_rohlager stRL3(LagerPlatzKupfer,2,10,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL3,UID,dummystring);
std::cout << "D2:" <<dummystring<<'\n';
      vergleichen(C,Check::Menge,"_rohwarenlager_raus","Rohwarenlager auslagern\n",mit_reparatur_programm);

      Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
      int weberei_znr=1;
      AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
      assert(AEP.getStueck()==AEP.getRestStk());
      AEP.Planen(UID,5000,PA,PLANDATUM5);
      vergleichen(C,Check::Menge,"_planen_weberei_fuer_lager","Planen der Weberei zum späteren Test des Bandlagers",mit_reparatur_programm);

//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
      DataBase_init::createJumbo(-10,12000);
std::cout << dummystring<<'\n';
      vergleichen(C,Check::Menge,"_bandlager_rein","Bandlager einlagern\n",mit_reparatur_programm);

      {AufEintrag AE(AEB);
        AE.Produziert(300,Lieferschein::none_id);
      }
      vergleichen(C,Check::Menge,"_kunde_teillieferung","Kunde Teillieferung\n",mit_reparatur_programm);

//std::cout << "\n\n120 ABSCHREIBEN\n\n\n";
      {AufEintrag AE(AEB);
        AE.Produziert(120,Lieferschein::none_id);
      }
      vergleichen(C,Check::Menge,"_kunde_ueberlieferung","Kunde Überlieferung\n",mit_reparatur_programm);


      // test von force, leer, etc
#if 0
      dummystring="";
      RohwarenLager::st_rohlager stRL10(LagerPlatzKupfer2,6,35,1,7,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL10,UID,dummystring,true);
std::cout << "D10: "<<dummystring<<'\n';

      dummystring="";
      RohwarenLager::st_rohlager stRL11(LagerPlatzKupfer,0,0,2,1,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,UID,dummystring);
std::cout << "D11: "<<dummystring<<'\n';
      vergleichen(C,Check::Menge|Check::RohLager,"_force_art","force, falscher Artikel",mit_reparatur_programm);

      dummystring="";
      RohwarenLager::st_rohlager stRL12(LagerPlatzKupfer2,10,35,0,0,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,UID,dummystring);
std::cout << "D12: "<<dummystring<<'\n';

      dummystring="";
      RohwarenLager::st_rohlager stRL13(LagerPlatzAcetat,2,7,0,0,ARTIKEL_ACETAT,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL10,UID,dummystring,false,true);
std::cout << "D13: "<<dummystring<<'\n';
      vergleichen(C,Check::Menge|Check::RohLager,"_zuviel","zuviel Entnommen",mit_reparatur_programm);
#endif

      cout << "Lager-Test erfolgreich\n";
#endif
      break;
     }
    case Rep_Petig_0er_2er_gleichzeitig:
     {
      #ifndef REPARATUR_PROGRAMM_TESTEN
        assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
      #endif

      std::string q1="update auftragentry set bestellt=3000 where "
                     " (auftragid,zeilennr,instanz) = (2,1,8)";
      Query::Execute(q1);
      SQLerror::test(__FILELINE__);
      std::string qJ2="update rohjumbo set soll_meter=5000 where artikelid=123755";
      Query::Execute(qJ2);
      SQLerror::test(__FILELINE__);

      vergleichen(C,Check::Menge,"_Rep0er2ergleichzeitig","Reparatur 0er und 2er gleichzeitig (Lager)",mit_reparatur_programm,true);

      std::string q2="insert into auftragentry (auftragid,zeilennr,bestellt,"
         " geliefert,lieferdate,artikelid,status,instanz)"
         " values (2,5,3000,0,'2009-01-01',123755,1,4)";
      Query::Execute(q2);
      vergleichen(C,Check::Menge,"_Rep0er2ergleichzeitigP","Reparatur 0er und 2er gleichzeitig (Produktionsinstanz)", mit_reparatur_programm,true);

      cout << "Reparatur 2er und 0er gleichzeitig erfolgreich\n";

      break;
     }
    case ZweiAuftraege:
     {
      #ifdef PETIG_TEST
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,13000,PA,PLANDATUM6);
       vergleichen(C,Check::Menge,"_planen_fuer_zweiten_auftrag","Über-Planen der Färberei",mit_reparatur_programm);
       }
       AufEintragBase AEB=auftrag.anlegen2();
       vergleichen(C,Check::Menge,"_zwei_auftraege_anlegen","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"]",mit_reparatur_programm);
      cout << "ZweiAufträge-Test erfolgreich\n";

       break;
      #endif
     }
    case ZweiterAuftrag_frueheresDatum:
     {
      #ifdef PETIG_TEST

       AufEintragBase AEB=auftrag.anlegen3();

       vergleichen(C,Check::Menge,"_zwei_auftraege_datum","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"] mit früherem Liefertermin",mit_reparatur_programm);

       {
          AufEintrag AE(AEB);
        AE.Produziert(200,Lieferschein::none_id);
       }
       vergleichen(C,Check::Menge,"_zwei_auftraege_datum_abschreiben","Teil-Abschreiben des zweiten Auftrags ["+AEB.str()+"]",mit_reparatur_programm);

//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);

       AufEintrag(AEB).setStatus(CLOSED,UID);
       vergleichen(C,Check::Menge,"_zwei_auftraege_datum_closed","Statussänderung(2) (Closed)",mit_reparatur_programm);

       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       assert(AEP.getStueck()==AEP.getRestStk());
       AEP.Planen(UID,7000,PA,PLANDATUM5);
       vergleichen(C,Check::Menge,"_zwei_auftraege_weberei_planen","Planen der Weberei",mit_reparatur_programm);

       AE.setStatus(CLOSED,UID);
// OHNE ReparaturProgramm, da CLOSED-Kundedenaufträge noch nicht nach
//  unten korrigiert werden
       vergleichen(C,Check::Menge,"_erster_auftrag_closed","Statussänderung(1) (Closed)",false);

      cout << "ZweiAufträgeDatum erfolgreich\n";

      break;
      #endif
     }
    case Lieferscheintest:
     {
#ifdef PETIG_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,150,0,0);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LS_teillieferung","Lieferschein mit Teillieferung anlegen",mit_reparatur_programm);

       int lznr=1;
       LieferscheinEntry le((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LS_zeileloeschen","Lieferscheinzeile löschen",mit_reparatur_programm);

       liefs.push_back(ARTIKEL_ROLLEREI,450,0,0);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LS_volllieferung","Lieferschein mit Volllieferung",mit_reparatur_programm);

       LieferscheinEntry le2((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le2);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LS_zeileloeschen2","Lieferscheinzeile nochmal löschen",mit_reparatur_programm);

      cout << "Lieferschein Test erfolgreich\n";
#endif

       break;
     }
    case LieferscheintestMenge:
     {
#ifdef PETIG_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,150,0,0);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LS_teillieferung","Lieferschein mit Teillieferung anlegen",mit_reparatur_programm);

       int stueck=140;
       AuftragBase::mengen_t menge=0;
       int lznr=1;
       LieferscheinEntry le(LieferscheinEntryBase(liefs,lznr));
       le.changeMenge(stueck,menge);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LS_mengenaenderung_minus","Lieferschein Mengenaenderung Minus",mit_reparatur_programm);

       stueck=400;
       LieferscheinEntry le2(LieferscheinEntryBase(liefs,lznr));
       le2.changeMenge(stueck,menge);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LS_mengenaenderung_plus","Lieferschein Mengenaenderung Plus",mit_reparatur_programm);

      cout << "Lieferschein Mengen-Test erfolgreich\n";

#endif


       break;
     }
    case Lieferscheintest_ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();
       vergleichen(C,Check::Menge,"_zwei_auftraege_datum","Anlegen eines zweiten (offenen) Auftrags ["+AEB.str()+"] mit früherem Liefertermin",mit_reparatur_programm);

#ifdef PETIG_TEST

       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,50,0,0);
       vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZA","Lieferschein mit Teillieferung und 2 Aufträgen anlegen",mit_reparatur_programm);

       liefs.push_back(ARTIKEL_ROLLEREI,600,0,0);
       vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZAV","Lieferschein mit Volllieferung und 2 Aufträgen anlegen",mit_reparatur_programm);

      cout << "Lieferschein Datums-Test erfolgreich\n";
#endif

       break;
     }
    case LieferscheintestZusatz:
     {
#ifdef PETIG_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,550,0,0);
       vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZ","Lieferschein mit Zusatzeintrag anlegen",mit_reparatur_programm);

       int stueck=633;
       AuftragBase::mengen_t menge=0;

       int lznr=1;
       LieferscheinEntry le3(LieferscheinEntryBase(liefs,lznr));
       le3.changeMenge(stueck,menge);
       vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZP","Lieferscheinentry mit Zusatzeintrag Plus",mit_reparatur_programm);

       LieferscheinEntry le4(LieferscheinEntryBase(liefs,lznr));
       stueck=450;
//cout << "\nLOS GEHTS";
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);

       le4.changeMenge(stueck,menge);
       vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZM","Lieferscheinentry mit Zusatzeintrag Minus",mit_reparatur_programm);

       LieferscheinEntry le5(LieferscheinEntryBase(liefs,lznr));
       stueck=350;
       le5.changeMenge(stueck,menge);
       vergleichen(C,Check::Lieferschein|Check::Menge,"_LSZMK","Lieferscheinentry mit Zusatzeintrag Minus Kunde",mit_reparatur_programm);

      cout << "Lieferschein Zusatz-Test erfolgreich\n";

#endif
       break;
     }
    case LieferscheinJacek:
    case Rep_Mabella:
     {
#ifdef MABELLA_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_TRIO,10,0,0);
       vergleichen(C,Check::Lieferschein|Check::Menge,"_LS_volllieferung","Lieferschein mit Volllieferung (Mabella) anlegen",mit_reparatur_programm);
//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);

       int lznr=1;
       LieferscheinEntry le1(LieferscheinEntryBase(liefs,lznr));
       int stueck=3;
       le1.changeMenge(stueck,0);
       vergleichen(C,Check::Lieferschein|Check::Menge,"_LS_mengenaenderung_minus","Lieferscheinentry: Minus",mit_reparatur_programm);

       {// Produktionsplaner
        int znr=1;
        AufEintragBase OldAEB((AuftragBase(PRODPLANUNG,AuftragBase::ungeplante_id)),znr);
        {AufEintrag OldAE(OldAEB);
         Auftrag PA=Auftrag(Auftrag::Anlegen(PRODPLANUNG),ManuProC::DefaultValues::EigeneKundenId);
         OldAE.ProduktionsPlanung(UID,2,PA,PLANDATUM6,WEBEREI);
         vergleichen(C,Check::Menge,"_PP","ProduktionsPlanungWeberei",mit_reparatur_programm);
        }

        {AufEintrag OldAE(OldAEB);
         Auftrag PA=Auftrag(Auftrag::Anlegen(PRODPLANUNG),ManuProC::DefaultValues::EigeneKundenId);
         OldAE.ProduktionsPlanung(UID,20,PA,PLANDATUM6,EINKAUF);
         vergleichen(C,Check::Menge,"_PPE","ProduktionsPlanungEinkauf",mit_reparatur_programm);
        }
       }

      {// Überplanen des Einkaufs
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
      int znr=1;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
//      int nznr=
      AEP.Planen(UID,27,PA,PLANDATUM5);
      vergleichen(C,Check::Menge,"_planen_kupfer","Über-Planen des Einkaufs (Mabella)",mit_reparatur_programm);
      }

      {// Weberei liefert mehr als geplant
       Lieferschein liefs(WEBEREI,cH_Kunde(Kunde::eigene_id));
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
       liefs.push_back(ARTIKEL_TRIO,5,0,0);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LSZ","Lieferschein in Weberei mit Überlieferung (Mabella)",mit_reparatur_programm);
      }

      {// Einkauf liefert Teilmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
//ManuProC::Tracer::Enable(AuftragBase::trace_channel);
       liefs.push_back(ARTIKEL_TRIO,13,0,0);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LSZP","Lieferschein im Einkauf mit Teillieferung (Mabella)",mit_reparatur_programm);
      }

      {// Einkauf liefert Vollmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
ManuProC::Tracer::Enable(~AuftragBase::trace_channel);
       liefs.push_back(ARTIKEL_TRIO,25,0,0);
       vergleichen(C,Check::Menge|Check::Lieferschein,"_LSZM","Lieferschein im Einkauf Weberei mit Restlieferung (Mabella)",mit_reparatur_programm);
      }


      { // Bestellen eines Artikels ohne Kunden-Auftrag
        Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
        ManuProC::st_produziert sp(ARTIKEL_ZWEI,2222,getuid(),Kunde::eigene_id,LieferscheinBase::none_id,PA,SPLITDATUM);
        cH_ppsInstanz I(EINKAUF);
//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);
        I->Planen(sp);
        vergleichen(C,Check::Menge,"_zwei_auftraege_anlegen","Einkauf eines nicht-bestelleten Artikel (Mabella)",mit_reparatur_programm);
      }
      cout << "Test für Mabella erfolgreich\n";

      if(mode!=Rep_Mabella) break;
#ifndef REPARATUR_PROGRAMM_TESTEN
      assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
#endif
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

      vergleichen(C,Check::Menge,"_zwei_auftraege_anlegen","Reparatur (Mabella)", mit_reparatur_programm,true);

      cout << "Reparatur-Test für Mabella erfolgreich\n";

#endif
      break;
     }
    case ZweiKundenTest:
     {
       AufEintragBase AEB2=auftrag.anlegenK();
       vergleichen(C,Check::Menge,"_ZK_anlegen","Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["+AEB.str()+"]",mit_reparatur_programm);

      {AufEintrag AE(AEB);
        AE.Produziert(300,Lieferschein::none_id);
      }
      vergleichen(C,Check::Menge,"_ZK_abschreiben1T","Zwei Kunden Teillieferung 1\n",mit_reparatur_programm);

      {AufEintrag AE(AEB2);
        AE.Produziert(180,Lieferschein::none_id);
      }
      vergleichen(C,Check::Menge,"_ZK_abschreiben2T","Zwei Kunden Teillieferung 2\n",mit_reparatur_programm);

      {AufEintrag AE(AEB);
//      ManuProC::Tracer::Enable(~AuftragBase::trace_channel);
        AE.Produziert(200,Lieferschein::none_id);
      }
      vergleichen(C,Check::Menge,"_ZK_abschreiben1U","Zwei Kunden Volllieferung 1\n",mit_reparatur_programm);

      cout << "Zwei Kunden-Test erfolgreich\n";


       break;
     }
    case ZweiKundenMengeFreigebenTest:
     {
#      ifdef PETIG_TEST

       AufEintragBase AEB2=auftrag.anlegenK();
       vergleichen(C,Check::Menge,"_ZK_anlegen","Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["+AEB.str()+"]",mit_reparatur_programm);

       {
         cH_ppsInstanz I(ppsInstanzID::Bandlager);
         int znrvon=1;
         AufEintragBase Von(AuftragBase(I,AuftragBase::plan_auftrag_id),znrvon);
         int znrnach=2;
         AufEintrag Fuer((class AufEintragBase(AuftragBase(I,AuftragBase::ungeplante_id),znrnach)));
         AufEintrag(Von).menge_fuer_aeb_freigeben(3000,Fuer,getuid());
         vergleichen(C,Check::Menge,"_ZKM","Menge freigeben für einen anderen Auftrag",mit_reparatur_programm);
       }

       cout << "Zwei Kunden-Menge-Freigeben-Test erfolgreich\n";

       break;
      #endif
     }
    case JumboLager:
     {
#if defined (PETIG_EXTENSIONS) && defined (MANUPROC_DYNAMICENUMS_CREATED)
       LagerPlatz LP(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ);
       LagerPlatz LP2(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ+1);
       KettplanKette KK=KettplanKette(Kette(MASCHIENE,SCHAERDATUM));
       vector<JumboRolle> JR=JumboRolle::create(KK);
       assert(JR.size()==1);
       class JumboLager JL;
       Zeitpunkt_new zp0("2002-3-1 11:00"),
       		zp1("2002-3-1 11:11"),
       		zp0b("2002-3-1 11:02");
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1);
       JR=JumboRolle::create(KK); // 102
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1);
       vergleichen(C,Check::Jumbo,"_richtig","Jumbo richtig",mit_reparatur_programm);
       JR=JumboRolle::create(KK); // 103
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0);
       JR=JumboRolle::create(KK); // 104
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0);
       vergleichen(C,Check::Jumbo,"_falsch","Jumbo falsche Reihenfolge",mit_reparatur_programm);
       JR=JumboRolle::create(KK); // 105
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0);
       try // kein Log Eintrag ist richtig
       {JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0b);
        assert(!"Jumbo_Entnahme sollte 100 werfen");
       }catch (SQLerror &e)
       {  assert(e.Code()==100);
       }
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1);
       vergleichen(C,Check::Jumbo,"_doppelt","Jumbo doppelt Aus-/Einlagern",mit_reparatur_programm);
       break;
#endif
     }
    case None: assert(!"Never get here\n");
   }
 return 0;
}

void usage(const std::string &argv0,const std::string &argv1)
{
  cerr << argv0 <<" muß mit [(M)engentest|(P)lantest|(L)agertest|\n"
                  "\t(S)plittest|(Z)weiAuftraege|"
                  "\tZweiterAuftrag_frueheres(D)atum|\n"
                  "\t(L)iefer(s)cheine|(L)ieferscheine(m)engen|\n"
                  "\t(L)ieferschein(Z)usatzeintrag|(L)ieferscheinZweiter(A)uftrag_frueheresDatum|\n"
                  "\t(L)ieferschein(J)acek\n"
                  "\t(Z)wei(K)unden)\n"
                  "\t(Z)wei(K)unden(M)engeFreigeben\n"
                  "\t(M)anu(P)roCTest\n"
                  "\t(J)umboLager\n"
                  "\t(R)eparatur(P)hysikalischesLager\n"
                  "\t(R)eparatur_0er_2er_(g)leichzeitig\n"
                  "\t(R)eparatur_(K)undenprogramm\n"
                  "\t(R)eparatur_(Z)uordnungen\n"
                  "\t(R)eparatur_(K)unden_(Z)uordnungen\n"
                  "\t(R)eparartur(M)Mabella, =0er+2er OPEN, bestellt=0, Kundenid=1]\n"
                  "\taufgerufen werden\n"
       << " nicht mit '"<<argv1<<"'\n";
  exit(1);
}

int main(int argc,char *argv[])
{

   if(argc==1) { usage(argv[0],""); return 1; }
   e_mode mode=None;

   std::string mode_str=argv[1];
   if(mode_str=="M" || mode_str=="Mengentest") mode=Mengentest;
   else if(mode_str=="P" || mode_str=="Plantest")  mode=Plantest;
   else if(mode_str=="L" || mode_str=="Lagertest")  mode=Lagertest;
   else if(mode_str=="S" || mode_str=="Splittest")  mode=Splittest;
   else if(mode_str=="Z" || mode_str=="ZweiAuftraege")  mode=ZweiAuftraege;
   else if(mode_str=="D" || mode_str=="ZweiterAuftrag_frueheresDatum")  mode=ZweiterAuftrag_frueheresDatum;
   else if(mode_str=="Ls" || mode_str=="Lieferscheintest")  mode=Lieferscheintest;
   else if(mode_str=="Lm" || mode_str=="LieferscheintestMenge")  mode=LieferscheintestMenge;
   else if(mode_str=="LZ" || mode_str=="LieferscheintestZusatz")  mode=LieferscheintestZusatz;
   else if(mode_str=="LA" || mode_str=="Lieferscheintest_ZweiterAuftrag_frueheresDatum")  mode=Lieferscheintest_ZweiterAuftrag_frueheresDatum;
   else if(mode_str=="LA" || mode_str=="Lieferscheintest_ZweiterAuftrag_frueheresDatum")  mode=Lieferscheintest_ZweiterAuftrag_frueheresDatum;
   else if(mode_str=="LJ" || mode_str=="Lieferscheintest_ZweiterAuftrag_frueheresDatum")  mode=LieferscheinJacek;
   else if(mode_str=="ZK" || mode_str=="ZweiKunden")  mode=ZweiKundenTest;
   else if(mode_str=="ZKM"|| mode_str=="ZweiKundenMengeFreigebenTest")  mode=ZweiKundenMengeFreigebenTest;
   else if(mode_str=="MP" || mode_str=="ManuProCTest")  mode=ManuProCTest;
   else if(mode_str=="J" || mode_str=="JumboLager")  mode=JumboLager;
   else if(mode_str=="RM" || mode_str=="ReparaturMabella")  mode=Rep_Mabella;
   else if(mode_str=="RP")  mode=Rep_Petig_PhysikalischesLager;
   else if(mode_str=="Rg")  mode=Rep_Petig_0er_2er_gleichzeitig;
   else if(mode_str=="RK")  mode=Rep_KundenProgramm;
   else if(mode_str=="RZ")  mode=Rep_Zuordnungen;
   else if(mode_str=="RKZ") mode=Rep_Kunden_Zuordnungen;
   if(mode==None) { usage(argv[0],argv[1]); return 1; }

   std::cout << "Initalisierung der Datenbank ...";
   std::cout.flush();

   std::string kill_output=" 2>/dev/null >/dev/null";
   if (verbose) kill_output="";
#ifdef MANU_PROC_TEST
   system((std::string(MANU_DATAPATH)+"/initdb.script "+MANU_DATAPATH+kill_output).c_str());
   putenv("PGDATABASE=anleitungdb");
#elif defined MABELLA_TEST
   system((std::string(MANU_DATAPATH)+"/initdb.script "+MANU_DATAPATH+kill_output).c_str());
   putenv("PGDATABASE=mabella_test_db");
#elif defined PETIG_TEST
   system(("database_tables_init/initdb.script"+kill_output).c_str());
   putenv("PGDATABASE=testdb");
#endif
   cout << "...beendet\n";

   Petig::PrintUncaughtExceptions();
   try{
   mkdir("files.log",0777);
   ofstream logstream(("files.log/"+mode_str).c_str());
   testlog=&logstream;
   Petig::dbconnect();

//ManuProC::Tracer::Enable(~AuftragBase::trace_channel);

   DataBase_init();
   return auftragstests(mode);

   }catch(SQLerror &e){std::cout << e<<'\n';}
  return 0;
}

