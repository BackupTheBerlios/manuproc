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
      Rep_Petig_0er_2er_gleichzeitig,Rep_KundenProgramm,Rep_Zuordnungen};

static int fehler()
{
  cerr << "ABBRUCH \n";
  return 1;
}


static int kein_fehler()
{
  return 0;
}


int auftragstests(e_mode mode)
{
//  ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);

   bool mit_reparatur_programm=false;
#ifdef REPARATUR_PROGRAMM_TESTEN
   mit_reparatur_programm=true;
   cout << "Mit ReparaturProgrammTest\n";
#endif   

   AuftragsVerwaltung  auftrag; 
   AufEintragBase AEB=auftrag.anlegen();
   AufEintrag AE=AEB;
   Check C;
   bool erfolgreich=false;
   cout << "Anlegen eines Auftrags ["<<AEB<<"] beendet\n\n";

   // ANLEGEN eines Auftrags mit Bandlager und Rohwarenlager
   if (mode!=JumboLager)
   {  
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
      AE.setStatus(OPEN,UID);
      erfolgreich=C.teste(Check::Menge,"_mit_lager_open",mit_reparatur_programm);
      if(!erfolgreich) 
         { cout << "Öffnen des Auftrags fehlgeschlagen\n"; return fehler();}
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
      erfolgreich=C.teste(Check::Menge,"_planen_kupfer",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Planen des Einkaufs(Granulat) \n\n"; return fehler();}       

      // Spezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(PAE,ARTIKEL_GRANULAT_GRUEN,1,200,0);
//unspezifisch   liefs.push_back(ARTIKEL_GRANULAT_GRUEN,1,200,0);
      erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LS_teillieferung",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein (mit AEB, Granulat) anlegen\n\n"; return  fehler();}
      }


      {// Planen des Einkaufs (Metall)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=5;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,10,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      erfolgreich=C.teste(Check::Menge,"_planen_weberei_fuer_lager",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Planen des Einkaufs(Metall) \n\n"; return fehler();}       


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_METALL,1,5,0);
      erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LS_volllieferung",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein (unbestimmt, Metall) anlegen\n\n"; return  fehler();}
      }


      {// Planen der Gießerei (ARTIKEL_GRIFF_ROT)
      Auftrag PA=Auftrag(Auftrag::Anlegen(GIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
      int znr=1;
      AufEintrag AEP((AufEintragBase(GIESSEREI,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,500,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      erfolgreich=C.teste(Check::Menge,"_planen_faerberei_teil",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Planen der Giesserei (Griff rot) \n\n"; return fehler();}       


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(GIESSEREI,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_GRIFF_ROT,500,0,0);
      erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZ",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein (Gießerei) anlegen\n\n"; return  fehler();}
      }

      {// Produktion in der Werkstatt ohne daß vorher etwas geplant wurde
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
      ManuProC::st_produziert p(ARTIKEL_SCHRAUBENZIEHER_GELB,600,UID,ManuProC::DefaultValues::EigeneKundenId);
      cH_ppsInstanz I(WERKSTATT); 
      I->Produziert(p);
      erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZP",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Produktion in der Werkstatt anlegen\n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für das Schraubenzieherlager => auslagern 
      Lieferschein liefs(SCHRAUBENZIEHERLAGER,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
      liefs.push_back(ARTIKEL_SCHRAUBENZIEHER_ROT,450,0,0);
      erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZM",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein für das Rohwarenlager (auslagern)\n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für den Kunden 
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,450,0,0);
      erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZMK",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein für den Kunden \n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für den Kunden  (Überlieferung)
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,60,0,0);
      erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZA",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein für den Kunden (Überlieferung)\n\n"; return  fehler();}
      }

      cout << "ManuProcTest erfolgreich\n";
      #endif
      break;
     }
    case Mengentest :
     {    

      // Menge des Auftrags erhöhen
      auftrag.kunden_bestellmenge_aendern(AEB,500);
      erfolgreich=C.teste(Check::Menge,"_menge_plus",mit_reparatur_programm);
      if(!erfolgreich) {cout << "Erhöhen der Auftragmenge \n\n";
                        return fehler();}

      // Menge des Auftrags erniedrigen (Rohwarenlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,100);
      erfolgreich=C.teste(Check::Menge,"_menge_minus",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Reduzieren der Auftragmenge unter Rohwarenlagerbestand \n\n";
               return fehler();}


      AE.updateLieferdatum(NEWDATUM,UID);
      erfolgreich=C.teste(Check::Menge,"_datumsaenderung",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Datumsänderung \n\n";
               return fehler();}


      // Menge des Auftrags weiter erniedrigen (Bandlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,10);
      erfolgreich=C.teste(Check::Menge,"_menge_minus_bandlager",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Reduzieren der Auftragmenge unter Bandlagerbestand \n\n";
               return fehler();}

      AufEintrag(AEB).setStatus(CLOSED,UID);
      erfolgreich=C.teste(Check::Menge,"_status_closed",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Statussänderung (Closed) \n\n";
               return fehler();}

      cout << "Mengen Test erfolgreich\n";
             
      break;
     }
    case Plantest:
     {
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::_Garn__Einkauf),Kunde::default_id);
       int kupfer_znr=2;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::_Garn__Einkauf,AuftragBase::ungeplante_id,kupfer_znr));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
       AEP.Planen(UID,100,PA,PLANDATUM5);
       erfolgreich=C.teste(Check::Menge,"_planen_kupfer",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Planen des Kupfereinkaufs \n\n";
               return fehler();}       
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,7000,PA,PLANDATUM4);
       erfolgreich=C.teste(Check::Menge,"_planen_faerberei_teil",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Teil-Planen der Färberei \n\n";
               return fehler();}
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       AEP.Planen(UID,5000,PA,PLANDATUM6);
       erfolgreich=C.teste(Check::Menge,"_planen_webereiP",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Planen der Weberei \n\n";
               return fehler();}
       }

#ifdef PETIG_TEST
       Lieferschein liefs(ppsInstanzID::_Garn__Einkauf,cH_Kunde(Kunde::eigene_id));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
       liefs.push_back(ARTIKEL_ACETAT,1,66,0);
       erfolgreich=C.teste(Check::Menge,"_planen_einkauf_lieferschein",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung für Einkauf anlegen\n\n"; return fehler();}
#endif

      cout << "Plan-Test erfolgreich\n";

      break;
     }
    case Splittest :
    case Rep_Petig_PhysikalischesLager:
     {
      AE.split(UID,300,SPLITDATUM);
      erfolgreich=C.teste(Check::Menge,"_split",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Splitten einer Auftragszeile \n\n";
               return fehler();}
#ifdef PETIG_TEST
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,UID,dummystring);
std::cout << dummystring<<'\n';
      erfolgreich=C.teste(Check::Menge,"_split_rohwarenlager_rein",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Rohwarenlager einlagern\n";
               return fehler();}

      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,UID,dummystring);
std::cout << dummystring<<'\n';
      erfolgreich=C.teste(Check::Menge,"_split_rohwarenlager_raus",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Rohwarenlager auslagern\n";
               return fehler();}
      cout << "Split-Test erfolgreich\n";

      if(mode!=Rep_Petig_PhysikalischesLager) break;
      #ifndef REPARATUR_PROGRAMM_TESTEN
        assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
      #endif
      // Physikalisches Lager ändern 
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
      std::string qB="insert into rl_inhalt (position_,kartons,reste,kg_per_karton,"
         " material,eingelagert,rest_kg)"
         " values ('07D5',10,0,5,211007,'2002-11-28',0)";
      Query::Execute(qB);
      SQLerror::test(__FILELINE__);
      erfolgreich=C.teste(Check::Menge,"_split_reparatur",mit_reparatur_programm,true);
      if(!erfolgreich) { cout << "Reparatur-Split-Test (Garnlager) \n";
               return fehler();}

      std::string qJ="insert into rohjumbo (code,maschine,soll_meter,plan_datum,"
         " status,lauf,gang,wiederinslager,artikelid,rest,lagerplatz) "
         " values (101,212,700,'2002-11-28',2,1,1,'2002-01-01 12:00:00+01',"
         " 123755,false,712)";
      Query::Execute(qJ);
      SQLerror::test(__FILELINE__);
      erfolgreich=C.teste(Check::Menge,"_split_reparatur_bandlager",mit_reparatur_programm,true);
      if(!erfolgreich) { cout << "Reparatur-Split-Test (Bandlager) \n";
               return fehler();}


      std::string qJ2="update rohjumbo set soll_meter=200 where "
         "(code,maschine,plan_datum,"
         " status,lauf,gang,wiederinslager,artikelid,rest,lagerplatz) "
         " = (101,212,'2002-11-28',2,1,1,'2002-01-01 12:00:00+01',"
         " 123755,false,712)";
      Query::Execute(qJ2);
      SQLerror::test(__FILELINE__);
      erfolgreich=C.teste(Check::Menge,"_split_reparatur_bandlager_minus",mit_reparatur_programm,true);
     if(!erfolgreich) { cout << "Reparatur-Split-Test (BandlagerMinus) \n";
               return fehler();}

      std::string qB2="update rl_inhalt set kartons=4 where "
         " (position_,reste,kg_per_karton,"
         " material,eingelagert,rest_kg)"
         " = ('07D5',0,5,211007,'2002-11-28',0)";
      Query::Execute(qB2);
      SQLerror::test(__FILELINE__);
      erfolgreich=C.teste(Check::Menge,"_split_reparatur_garnlager_minus",mit_reparatur_programm,true);
      if(!erfolgreich) { cout << "Reparatur-Split-Test (GarnalgerMinus) \n";
               return fehler();}

      cout << "Reparatur-Test (Petig, Physikalisches Lager) erfolgreich\n";

#endif
      break;
     }
    case Rep_Zuordnungen:
     {
       #ifndef REPARATUR_PROGRAMM_TESTEN
          assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
       #endif
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,1));
       AEP.Planen(UID,17000,PA,DATUMP);
       erfolgreich=C.teste(Check::Menge,"_rep_pf",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Reparatur-Planen der Färberei \n\n";
               return fehler();}
       }
      {
        Query::Execute("update auftragentry set bestellt=5555 where "
            "(instanz,auftragid)=(2,2)");
        SQLerror::test(__FILELINE__);
        erfolgreich=C.teste(Check::Menge,"_rep_pf",mit_reparatur_programm,true);
        if(!erfolgreich) { cout << "Reparatur-Zuordungen () \n";
               return fehler();}
      }
      {
        Query::Execute("update auftragentry set bestellt=6555 where "
            "(instanz,auftragid)=(2,2)");
        SQLerror::test(__FILELINE__);
        Query::Execute("update auftragsentryzuordnung set menge=6555 where "
            "(altinstanz,altauftragid,neuinstanz)=(2,2,2)");
        SQLerror::test(__FILELINE__);
        erfolgreich=C.teste(Check::Menge,"_rep_pf",mit_reparatur_programm,true);
        if(!erfolgreich) { cout << "Reparatur-Zuordungen () \n";
               return fehler();}
      }
      {
        Query::Execute("update auftragentry set geliefert=16555 where "
            "(instanz,auftragid)=(2,20000)");
        SQLerror::test(__FILELINE__);
        erfolgreich=C.teste(Check::Menge,"_rep_pfE",mit_reparatur_programm,true);
        if(!erfolgreich) { cout << "Reparatur-Zuordungen () \n";
               return fehler();}
      }
      cout << "Reparatur-Test (Petig, Zuordnungen) erfolgreich\n";
      break;      
     }
    case Rep_KundenProgramm:
     {
       #ifndef REPARATUR_PROGRAMM_TESTEN
          assert(!"FEHLER: MIT REPARATURPROGRAMM KOMPILIEREN\n");
       #endif
      {
       std::string q1="update auftragentry set artikelid="+itos(ARTIKEL_ROLLEREIK.Id())+" where "
                         " (auftragid,zeilennr,instanz) = (0,1,10)";                            
       Query::Execute(q1);
       SQLerror::test(__FILELINE__);

       erfolgreich=C.teste(Check::Menge,"_reparatur_kunde",mit_reparatur_programm,true);
       if(!erfolgreich) { cout << "Reparatur Kundenaufträge (Artikel)\n";
                           return fehler();} 
      }
      {
       std::string q2="update auftragsentryzuordnung set menge=300 where "
                         " (altauftragid) = (20000)";                            
       Query::Execute(q2);
       SQLerror::test(__FILELINE__);
       AufEintrag AE=AufEintrag(class AufEintragBase(class AuftragBase(ROLLEREI,AuftragBase::ungeplante_id),2));
       AE.updateStkDiff__(UID,-100,true,ManuProC::Auftrag::r_Anlegen);

       erfolgreich=C.teste(Check::Menge,"_reparatur_kunde",mit_reparatur_programm,true);
       if(!erfolgreich) { cout << "Reparatur Kundenaufträge (Menge)\n";
                           return fehler();} 
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

       erfolgreich=C.teste(Check::Menge,"_reparatur_kunde_instanz",mit_reparatur_programm,true);
       if(!erfolgreich) { cout << "Reparatur Kundenaufträge (Instanz)\n";
                           return fehler();} 
      }
      {
       std::string q2="update auftragentry set lieferdate='2009-12-31' where "
                         " (instanz) = (1)";                            
       Query::Execute(q2);
       SQLerror::test(__FILELINE__);
       erfolgreich=C.teste(Check::Menge,"_reparatur_kunde_datum",mit_reparatur_programm,true);
       if(!erfolgreich) { cout << "Reparatur Kundenaufträge (Datum)\n";
                           return fehler();} 
      }
       cout << "Reparatur Kundeaufträge (Artikel, Instanz) erfolgreich\n";
                               
       break;
     } 
    case Lagertest :
     {    
#ifdef PETIG_TEST
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,UID,dummystring);
      erfolgreich=C.teste(Check::Menge,"_rohwarenlager_rein",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Rohwarenlager einlagern\n";
               return fehler();}

      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,UID,dummystring);
std::cout << "D1: "<<dummystring<<'\n';
      RohwarenLager::st_rohlager stRL3(LagerPlatzKupfer,2,10,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL3,UID,dummystring);
std::cout << "D2:" <<dummystring<<'\n';
      erfolgreich=C.teste(Check::Menge,"_rohwarenlager_raus",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Rohwarenlager auslagern\n";
               return fehler();}

      Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
      int weberei_znr=1;
      AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
      assert(AEP.getStueck()==AEP.getRestStk());
      AEP.Planen(UID,5000,PA,PLANDATUM5);
      erfolgreich=C.teste(Check::Menge,"_planen_weberei_fuer_lager",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Planen der Weberei zum späteren Test des Bandlagers \n\n";
               return fehler();}

//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
      DataBase_init::createJumbo(-10,12000);
std::cout << dummystring<<'\n';
      erfolgreich=C.teste(Check::Menge,"_bandlager_rein",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Bandlager einlagern\n";
               return fehler();}

      {AufEintrag AE(AEB);
        AE.Produziert(300,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::Menge,"_kunde_teillieferung",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Kunde Teillieferung\n";
               return fehler();}

//std::cout << "\n\n120 ABSCHREIBEN\n\n\n";
      {AufEintrag AE(AEB);
        AE.Produziert(120,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::Menge,"_kunde_ueberlieferung",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Kunde Überlieferung\n";
               return fehler();} 


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
                                                

      erfolgreich=C.teste(Check::Menge,"_Rep0er2ergleichzeitig",mit_reparatur_programm,true);
      if(!erfolgreich) { cout << "Reparatur 0er und 2er gleichzeitig (Lager)\n";
                         return fehler();} 
      erfolgreich=C.teste(Check::Menge,"_Rep0er2ergleichzeitig",mit_reparatur_programm,true);
      if(!erfolgreich) { cout << "Reparatur 0er und 2er gleichzeitig (Lager, 2ter Durchlauf)\n";
                         return fehler();} 

      std::string q2="insert into auftragentry (auftragid,zeilennr,bestellt,"
         " geliefert,lieferdate,artikelid,status,instanz)"
         " values (2,5,3000,0,'2009-01-01',123755,1,4)";
      Query::Execute(q2);
      erfolgreich=C.teste(Check::Menge,"_Rep0er2ergleichzeitigP",mit_reparatur_programm,true);
      if(!erfolgreich) { cout << "Reparatur 0er und 2er gleichzeitig (Produktionsinstanz)\n";
                         return fehler();} 

      cout << "Reparatur 2er und 0er gleichzeitig erfolgreich\n";
                              
      break;
     } 
    case ZweiAuftraege:
     {
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,13000,PA,PLANDATUM6);
       erfolgreich=C.teste(Check::Menge,"_planen_fuer_zweiten_auftrag",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Über-Planen der Färberei \n\n";
               return fehler();}
       }
       AufEintragBase AEB=auftrag.anlegen2();
       erfolgreich=C.teste(Check::Menge,"_zwei_auftraege_anlegen",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] \n\n";
               return fehler();}
      cout << "ZweiAufträge-Test erfolgreich\n";
 
       break;
     }    
    case ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();

       erfolgreich=C.teste(Check::Menge,"_zwei_auftraege_datum",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin \n\n";
               return fehler();}

       {
          AufEintrag AE(AEB);
        AE.Produziert(200,Lieferschein::none_id);
       }
       erfolgreich=C.teste(Check::Menge,"_zwei_auftraege_datum_abschreiben",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Teil-Abschreiben des zweiten Auftrags ["<<AEB<<"] \n\n";
               return fehler();}

//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
 
       AufEintrag(AEB).setStatus(CLOSED,UID);
       erfolgreich=C.teste(Check::Menge,"_zwei_auftraege_datum_closed",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Statussänderung(2) (Closed) \n\n";
               return fehler();}

       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       assert(AEP.getStueck()==AEP.getRestStk());
       AEP.Planen(UID,7000,PA,PLANDATUM5);
       erfolgreich=C.teste(Check::Menge,"_zwei_auftraege_weberei_planen",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Planen der Weberei\n\n";
               return fehler();}
 
       AE.setStatus(CLOSED,UID);
#warning OHNE ReparaturProgramm, da CLOSED-Kundedenaufträge noch nicht nach 
#warning unten korrigiert werden
//       erfolgreich=C.teste(Check::Menge,"_erster_auftrag_closed",mit_reparatur_programm);
       erfolgreich=C.teste(Check::Menge,"_erster_auftrag_closed",false);
       if(!erfolgreich) { cout << "Statussänderung(1) (Closed)\n\n";
               return fehler();}       

      cout << "ZweiAufträgeDatum erfolgreich\n";

      break;
     }    
    case Lieferscheintest:
     {
#ifdef PETIG_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,150,0,0);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LS_teillieferung",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung anlegen\n\n"; return fehler();}

       int lznr=1;
       LieferscheinEntry le((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LS_zeileloeschen",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinzeile löschen\n\n";return fehler();}              

       liefs.push_back(ARTIKEL_ROLLEREI,450,0,0);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LS_volllieferung",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Volllieferung \n\n"; return fehler();}

       LieferscheinEntry le2((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le2);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LS_zeileloeschen2",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinzeile nochmal löschen\n\n";return fehler();}              
      
      cout << "Lieferschein Test erfolgreich\n";
#endif

       break;
     }
    case LieferscheintestMenge:
     {
#ifdef PETIG_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,150,0,0);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LS_teillieferung",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung anlegen\n\n"; return fehler();}

       int stueck=140;
       AuftragBase::mengen_t menge=0;
       int lznr=1;
       LieferscheinEntry le(LieferscheinEntryBase(liefs,lznr));
       le.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LS_mengenaenderung_minus",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein Mengenaenderung Minus \n\n"; return fehler();}

       stueck=400;
       LieferscheinEntry le2(LieferscheinEntryBase(liefs,lznr));
       le2.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LS_mengenaenderung_plus",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein Mengenaenderung Plus \n\n"; return fehler();}

      cout << "Lieferschein Mengen-Test erfolgreich\n";

#endif


       break;
     }
    case Lieferscheintest_ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();
       erfolgreich=C.teste(Check::Menge,"_zwei_auftraege_datum",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin \n\n";
                          return fehler();}

#ifdef PETIG_TEST

       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,50,0,0);
       erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZA",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung und 2 Aufträgen anlegen\n\n"; return fehler();}

       liefs.push_back(ARTIKEL_ROLLEREI,600,0,0);
       erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZAV",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Volllieferung und 2 Aufträgen anlegen\n\n"; return fehler();}

      cout << "Lieferschein Datums-Test erfolgreich\n";
#endif

       break;
     }
    case LieferscheintestZusatz:
     {
#ifdef PETIG_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,550,0,0);
       erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZ",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Zusatzeintrag anlegen\n\n"; return fehler();}

       int stueck=633;
       AuftragBase::mengen_t menge=0;

       int lznr=1;
       LieferscheinEntry le3(LieferscheinEntryBase(liefs,lznr));
       le3.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZP",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Plus \n\n"; return fehler();}

       LieferscheinEntry le4(LieferscheinEntryBase(liefs,lznr));
       stueck=450;
       le4.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZM",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Minus \n\n"; return fehler();}

       LieferscheinEntry le5(LieferscheinEntryBase(liefs,lznr));
       stueck=350;
       le5.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LSZMK",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Minus Kunde \n\n"; return fehler();}

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
       erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LS_volllieferung",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Volllieferung (Mabella) anlegen\n\n"; return fehler();}
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);

       int lznr=1;
       LieferscheinEntry le1(LieferscheinEntryBase(liefs,lznr));
       int stueck=3;
       le1.changeMenge(stueck,0);
       erfolgreich=C.teste(Check::Lieferschein|Check::Menge,"_LS_mengenaenderung_minus",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinentry: Minus \n\n"; return fehler();}

       {// Produktionsplaner
        int znr=1;
        AufEintragBase OldAEB((AuftragBase(PRODPLANUNG,AuftragBase::ungeplante_id)),znr);
        {AufEintrag OldAE(OldAEB);
         Auftrag PA=Auftrag(Auftrag::Anlegen(PRODPLANUNG),ManuProC::DefaultValues::EigeneKundenId);
         OldAE.ProduktionsPlanung(UID,2,PA,PLANDATUM6,WEBEREI);
         erfolgreich=C.teste(Check::Menge,"_PP",mit_reparatur_programm);
         if(!erfolgreich) { cout << "ProduktionsPlanungWeberei \n\n"; return fehler();}
        }

        {AufEintrag OldAE(OldAEB);
         Auftrag PA=Auftrag(Auftrag::Anlegen(PRODPLANUNG),ManuProC::DefaultValues::EigeneKundenId);
         OldAE.ProduktionsPlanung(UID,20,PA,PLANDATUM6,EINKAUF);
         erfolgreich=C.teste(Check::Menge,"_PPE",mit_reparatur_programm);
         if(!erfolgreich) { cout << "ProduktionsPlanungEinkauf \n\n"; return fehler();}
        }
       }

      {// Überplanen des Einkaufs
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
      int znr=1;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
      int nznr=AEP.Planen(UID,27,PA,PLANDATUM5);
      erfolgreich=C.teste(Check::Menge,"_planen_kupfer",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Über-Planen des Einkaufs (Mabella) \n\n"; return fehler();}       
      }

      {// Weberei liefert mehr als geplant
       Lieferschein liefs(WEBEREI,cH_Kunde(Kunde::eigene_id));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
       liefs.push_back(ARTIKEL_TRIO,5,0,0);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LSZ",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein in Weberei mit Überlieferung (Mabella) \n\n"; return fehler();}
      }

      {// Einkauf liefert Teilmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
       liefs.push_back(ARTIKEL_TRIO,13,0,0);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LSZP",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein im Einkauf mit Teillieferung (Mabella) \n\n"; return fehler();}
      }

      {// Einkauf liefert Vollmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
       liefs.push_back(ARTIKEL_TRIO,25,0,0);
       erfolgreich=C.teste(Check::Menge|Check::Lieferschein,"_LSZM",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein im Einkauf Weberei mit Restlieferung (Mabella) \n\n"; return fehler();}
      }


      { // Bestellen eines Artikels ohne Kunden-Auftrag
        Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
        ManuProC::st_produziert sp(ARTIKEL_ZWEI,2222,getuid(),Kunde::eigene_id,LieferscheinBase::none_id,PA,SPLITDATUM);
        cH_ppsInstanz I(EINKAUF);
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
        I->Planen(sp);
        erfolgreich=C.teste(Check::Menge,"_zwei_auftraege_anlegen",mit_reparatur_programm);
        if(!erfolgreich) { cout << "Einkauf eines nicht-bestelleten Artikel (Mabella) \n\n"; return fehler();}       
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

      erfolgreich=C.teste(Check::Menge,"_zwei_auftraege_anlegen",mit_reparatur_programm,true);
      if(!erfolgreich) { cout << "Reparatur (Mabella) \n\n"; return fehler();}       
      
      cout << "Reparatur-Test für Mabella erfolgreich\n";

#endif
      break;
     }
    case ZweiKundenTest:
     {
       AufEintragBase AEB2=auftrag.anlegenK();
       erfolgreich=C.teste(Check::Menge,"_ZK_anlegen",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["<<AEB<<"] \n\n";
               return fehler();}

      {AufEintrag AE(AEB);
        AE.Produziert(300,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::Menge,"_ZK_abschreiben1T",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Zwei Kunden Teillieferung 1\n";
               return fehler();}

      {AufEintrag AE(AEB2);
        AE.Produziert(180,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::Menge,"_ZK_abschreiben2T",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Zwei Kunden Teillieferung 2\n";
               return fehler();}

      {AufEintrag AE(AEB);
//      ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
        AE.Produziert(200,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::Menge,"_ZK_abschreiben1U",mit_reparatur_programm);
      if(!erfolgreich) { cout << "Zwei Kunden Volllieferung 1\n";
               return fehler();}

      cout << "Zwei Kunden-Test erfolgreich\n";


       break;
     }
    case ZweiKundenMengeFreigebenTest:
     {
       AufEintragBase AEB2=auftrag.anlegenK();
       erfolgreich=C.teste(Check::Menge,"_ZK_anlegen",mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["<<AEB<<"] \n\n";
               return fehler();}

       {
         cH_ppsInstanz I(ppsInstanzID::Bandlager);
         int znrvon=1;
         AufEintragBase Von(AuftragBase(I,AuftragBase::plan_auftrag_id),znrvon);
         int znrnach=2;
         AufEintrag Fuer((class AufEintragBase(AuftragBase(I,AuftragBase::ungeplante_id),znrnach)));
         AufEintrag(Von).menge_fuer_aeb_freigeben(3000,Fuer,getuid());
         erfolgreich=C.teste(Check::Menge,"_ZKM",mit_reparatur_programm);
         if(!erfolgreich) { cout << "Menge freigeben für einen anderen Auftrag \n\n";
               return fehler();}
       }
       
       cout << "Zwei Kunden-Menge-Freigeben-Test erfolgreich\n";

       break;
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
       erfolgreich=C.teste(Check::Jumbo_richtig,mit_reparatur_programm);
       JR=JumboRolle::create(KK); // 103
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0);
       JR=JumboRolle::create(KK); // 104
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0);
       erfolgreich=C.teste(Check::Jumbo_falsch,mit_reparatur_programm);
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
       erfolgreich=C.teste(Check::Jumbo_doppelt,mit_reparatur_programm);
       break;
#endif
     }
    case None: assert(!"Never get here\n");
   }
 return kein_fehler();
}

void usage(const std::string &argv0,const std::string &argv1)
{
  cerr << argv0 <<" muß mit [(M)engentest|(P)lantest|(L)agertest|\n"
                  "\t(S)plittest|(Z)weiAuftraege|"
                  "\tZweiterAuftrag_frueheres(D)atum|\n"
                  "\t(L)iefer(s)cheine|(L)ieferscheine(m)engen|\n"
                  "\t(L)ieferschein(Z)usatzeintrag|(L)ieferscheinZweiter(A)uftrag_frueheresDatum|\n"
                  "\t(Z)wei(K)unden)\n"
                  "\t(Z)wei(K)unden(M)engeFreigeben\n"
                  "\t(M)anu(P)roCTest\n"
                  "\t(J)umboLager\n"
                  "\t(R)eparatur(P)hysikalischesLager\n"
                  "\t(R)eparatur_0er_2er_(g)leichzeitig\n"
                  "\t(R)eparatur_(K)undenprogramm\n"
                  "\t(R)eparatur_(Z)uordnungen\n"
                  "\t(R)eparartur(M)Mabella, =0er+2er OPEN, bestellt=0, Kundenid=1]\n"
                  "\taufgerufen werden\n"
       << " nicht mit '"<<argv1<<"'\n";
  exit(1);
}

int main(int argc,char *argv[])
{

   if(argc==1) usage(argv[0],"");
   e_mode mode=None;
   if(std::string(argv[1])=="M" || std::string(argv[1])=="Mengentest") mode=Mengentest;
   else if(std::string(argv[1])=="P" || std::string(argv[1])=="Plantest")  mode=Plantest;
   else if(std::string(argv[1])=="L" || std::string(argv[1])=="Lagertest")  mode=Lagertest;
   else if(std::string(argv[1])=="S" || std::string(argv[1])=="Splittest")  mode=Splittest;
   else if(std::string(argv[1])=="Z" || std::string(argv[1])=="ZweiAuftraege")  mode=ZweiAuftraege;
   else if(std::string(argv[1])=="D" || std::string(argv[1])=="ZweiterAuftrag_frueheresDatum")  mode=ZweiterAuftrag_frueheresDatum;
   else if(std::string(argv[1])=="Ls" || std::string(argv[1])=="Lieferscheintest")  mode=Lieferscheintest;
   else if(std::string(argv[1])=="Lm" || std::string(argv[1])=="LieferscheintestMenge")  mode=LieferscheintestMenge;
   else if(std::string(argv[1])=="LZ" || std::string(argv[1])=="LieferscheintestZusatz")  mode=LieferscheintestZusatz;
   else if(std::string(argv[1])=="LA" || std::string(argv[1])=="Lieferscheintest_ZweiterAuftrag_frueheresDatum")  mode=Lieferscheintest_ZweiterAuftrag_frueheresDatum;
   else if(std::string(argv[1])=="LA" || std::string(argv[1])=="Lieferscheintest_ZweiterAuftrag_frueheresDatum")  mode=Lieferscheintest_ZweiterAuftrag_frueheresDatum;
   else if(std::string(argv[1])=="LJ" || std::string(argv[1])=="Lieferscheintest_ZweiterAuftrag_frueheresDatum")  mode=LieferscheinJacek;
   else if(std::string(argv[1])=="ZK" || std::string(argv[1])=="ZweiKunden")  mode=ZweiKundenTest;
   else if(std::string(argv[1])=="ZKM"|| std::string(argv[1])=="ZweiKundenMengeFreigebenTest")  mode=ZweiKundenMengeFreigebenTest;
   else if(std::string(argv[1])=="MP" || std::string(argv[1])=="ManuProCTest")  mode=ManuProCTest;
   else if(std::string(argv[1])=="J" || std::string(argv[1])=="JumboLager")  mode=JumboLager;
   else if(std::string(argv[1])=="RM" || std::string(argv[1])=="ReparaturMabella")  mode=Rep_Mabella;
   else if(std::string(argv[1])=="RP")  mode=Rep_Petig_PhysikalischesLager;
   else if(std::string(argv[1])=="Rg")  mode=Rep_Petig_0er_2er_gleichzeitig;
   else if(std::string(argv[1])=="RK")  mode=Rep_KundenProgramm;
   else if(std::string(argv[1])=="RZ")  mode=Rep_Zuordnungen;
   if(mode==None) { usage(argv[0],argv[1]); return 1; }
   
   cout << "Initalisierung der Datenbank ...";
#ifdef MANU_PROC_TEST
   system((std::string(MANU_DATAPATH)+"/initdb.script "+MANU_DATAPATH).c_str());
   putenv("PGDATABASE=anleitungdb");
#elif defined MABELLA_TEST
   system((std::string(MANU_DATAPATH)+"/initdb.script "+MANU_DATAPATH).c_str());
   putenv("PGDATABASE=mabelladb");
#elif defined PETIG_TEST
   system("database_tables_init/initdb.script");
   putenv("PGDATABASE=testdb");
#endif
   cout << "...beendet\n";

   Petig::PrintUncaughtExceptions();
   try{
   Petig::dbconnect();  

//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
   
   DataBase_init();
   return auftragstests(mode);
   
   }catch(SQLerror &e){std::cout << e<<'\n';}
  return 0;
}

