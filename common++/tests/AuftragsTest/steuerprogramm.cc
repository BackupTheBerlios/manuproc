// $Id: steuerprogramm.cc,v 1.20 2002/11/25 15:21:52 thoma Exp $
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
#include <Lager/RohwarenLager.h>
#include <Lager/JumboLager.h>
#include <Auftrag/Auftrag.h>
#include <Instanzen/ppsInstanzProduziert.h>
#include <Misc/Trace.h>

#include <unistd.h>
// Jumbo
#include <Ketten/KettplanKette.h>
#include <Lager/JumboLager.h>
#include <Lager/RohwarenLager.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>


enum e_mode {None,Mengentest,Plantest,Lagertest,Splittest,ZweiAuftraege,
      ZweiterAuftrag_frueheresDatum,Lieferscheintest,LieferscheintestMenge,
      LieferscheintestZusatz,Lieferscheintest_ZweiterAuftrag_frueheresDatum,
      LieferscheinJacek,
      ZweiKundenTest,ZweiKundenMengeFreigebenTest,ManuProCTest,
      JumboLager};

int fehler()
{
  cerr << "ABBRUCH \n";
  return 1;
}


int kein_fehler()
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
      erfolgreich=C.teste(Check::Open,mit_reparatur_programm);
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
      erfolgreich=C.teste(Check::Planen_Kupfer,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Planen des Einkaufs(Granulat) \n\n"; return fehler();}       

      // Spezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(PAE,ARTIKEL_GRANULAT_GRUEN,1,200,0);
//unspezifisch   liefs.push_back(ARTIKEL_GRANULAT_GRUEN,1,200,0);
      erfolgreich=C.teste(Check::LieferscheinTeil,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein (mit AEB, Granulat) anlegen\n\n"; return  fehler();}
      }


      {// Planen des Einkaufs (Metall)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=5;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,10,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      erfolgreich=C.teste(Check::Planen_WebereiL,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Planen des Einkaufs(Metall) \n\n"; return fehler();}       


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_METALL,1,5,0);
      erfolgreich=C.teste(Check::LieferscheinVoll,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein (unbestimmt, Metall) anlegen\n\n"; return  fehler();}
      }


      {// Planen der Gießerei (ARTIKEL_GRIFF_ROT)
      Auftrag PA=Auftrag(Auftrag::Anlegen(GIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
      int znr=1;
      AufEintrag AEP((AufEintragBase(GIESSEREI,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,500,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      erfolgreich=C.teste(Check::Planen_Faerberei_teil,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Planen der Giesserei (Griff rot) \n\n"; return fehler();}       


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(GIESSEREI,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_GRIFF_ROT,500,0,0);
      erfolgreich=C.teste(Check::LieferscheinZusatz,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein (Gießerei) anlegen\n\n"; return  fehler();}
      }

      {// Produktion in der Werkstatt ohne daß vorher etwas geplant wurde
//      Lieferschein liefs(WERKSTATT,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
//cout << "\n\n\nHier gehts los\n";
//      liefs.push_back(ARTIKEL_SCHRAUBENZIEHER_GELB,600,0,0);
      ManuProC::st_produziert p(ARTIKEL_SCHRAUBENZIEHER_GELB,600,UID,ManuProC::DefaultValues::EigeneKundenId);
      cH_ppsInstanz I(WERKSTATT); 
      I->Produziert(p);
      erfolgreich=C.teste(Check::LieferscheinZusatzPlus,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Produktion in der Werkstatt anlegen\n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für das Schraubenzieherlager => auslagern 
      Lieferschein liefs(SCHRAUBENZIEHERLAGER,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
//cout << "\n\n\nHier gehts los\n";
      liefs.push_back(ARTIKEL_SCHRAUBENZIEHER_ROT,450,0,0);
      erfolgreich=C.teste(Check::LieferscheinZusatzMinus,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein für das Rohwarenlager (auslagern)\n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für den Kunden 
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
//cout << "\n\n\nHier gehts los\n";
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,450,0,0);
      erfolgreich=C.teste(Check::LieferscheinZusatzMinusKunde,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Lieferschein für den Kunden \n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für den Kunden  (Überlieferung)
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,60,0,0);
      erfolgreich=C.teste(Check::LieferscheinZweiAufTeil,mit_reparatur_programm);
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
      erfolgreich=C.teste(Check::Menge_Plus,mit_reparatur_programm);
      if(!erfolgreich) {cout << "Erhöhen der Auftragmenge \n\n";
                        return fehler();}

      // Menge des Auftrags erniedrigen (Rohwarenlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,100);
      erfolgreich=C.teste(Check::Menge_Minus,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Reduzieren der Auftragmenge unter Rohwarenlagerbestand \n\n";
               return fehler();}


      AE.updateLieferdatum(NEWDATUM,UID);
      erfolgreich=C.teste(Check::Datumsaenderung,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Datumsänderung \n\n";
               return fehler();}


      // Menge des Auftrags weiter erniedrigen (Bandlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,10);
      erfolgreich=C.teste(Check::Menge_MinusMinus,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Reduzieren der Auftragmenge unter Bandlagerbestand \n\n";
               return fehler();}

      AufEintrag(AEB).setStatus(CLOSED,UID);
      erfolgreich=C.teste(Check::StatusClosed,mit_reparatur_programm);
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
       erfolgreich=C.teste(Check::Planen_Kupfer,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Planen des Kupfereinkaufs \n\n";
               return fehler();}       
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,7000,PA,PLANDATUM4);
       erfolgreich=C.teste(Check::Planen_Faerberei_teil,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Teil-Planen der Färberei \n\n";
               return fehler();}
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));


       AEP.Planen(UID,5000,PA,PLANDATUM6);
       erfolgreich=C.teste(Check::Planen_WebereiP,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Planen der Weberei \n\n";
               return fehler();}
       }
      cout << "Plan-Test erfolgreich\n";

      break;
     }
    case Splittest :
     {
      AE.split(UID,300,SPLITDATUM);
      erfolgreich=C.teste(Check::Split,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Splitten einer Auftragszeile \n\n";
               return fehler();}
#ifdef PETIG_TEST
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,UID,dummystring);
cout << dummystring<<'\n';
//      RL.rein_ins_lager(ARTIKEL_KUPFER,100,UID);
      erfolgreich=C.teste(Check::Split_Rohwarenlager_einlagern,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Rohwarenlager einlagern\n";
               return fehler();}

//      RL.raus_aus_lager(ARTIKEL_KUPFER,100,UID);
      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,UID,dummystring);
cout << dummystring<<'\n';
      erfolgreich=C.teste(Check::Split_Rohwarenlager_auslagern,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Rohwarenlager auslagern\n";
               return fehler();}
      cout << "Split-Test erfolgreich\n";
#endif
      break;
     }
    case Lagertest :
     {    
#ifdef PETIG_TEST
      RohwarenLager RL;
      RohwarenLager::st_rohlager stRL(LagerPlatzKupfer2,100,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      std::string dummystring;
      RL.RL_Einlagern(LagerPlatzKupfer2,stRL,UID,dummystring);
cout << dummystring<<'\n';
//      Lager RL((cH_ppsInstanz(ppsInstanzID::Rohwarenlager)));
//      RL.rein_ins_lager(ARTIKEL_KUPFER,100,UID);
      erfolgreich=C.teste(Check::Rohwarenlager_einlagern,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Rohwarenlager einlagern\n";
               return fehler();}

//      RL.raus_aus_lager(ARTIKEL_KUPFER,120,UID);
      RohwarenLager::st_rohlager stRL2(LagerPlatzKupfer2,120,1,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
      RL.RL_Entnahme(stRL2,UID,dummystring);
cout << dummystring<<'\n';
      erfolgreich=C.teste(Check::Rohwarenlager_auslagern,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Rohwarenlager auslagern\n";
               return fehler();}

      Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
      int weberei_znr=1;
      AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
      assert(AEP.getStueck()==AEP.getRestStk());
      AEP.Planen(UID,5000,PA,PLANDATUM5);
      erfolgreich=C.teste(Check::Planen_WebereiL,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Planen der Weberei zum späteren Test des Bandlagers \n\n";
               return fehler();}

/*
      Lager BL((cH_ppsInstanz(ppsInstanzID::Bandlager)));
      BL.rein_ins_lager(ARTIKEL_BANDLAGER,12000,UID);
*/
      class JumboLager JL;
      Kette K(MASCHIENE-10,SCHAERDATUM);
      std::vector <ArtikelGang> artgang;
      artgang.push_back(ArtikelGang(GAENGE,ARTIKEL_BANDLAGER));
      KettplanKette KK=KettplanKette::create(K,artgang,12000,12000); 
      vector<JumboRolle> JR=JumboRolle::create(KK);
      assert(JR.size()==1);
      Zeitpunkt_new zp("2002-1-1 12:00");

//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);

      JL.Jumbo_Einlagern(LagerPlatzJumbo,JR.front(),JumboLager::Einlagern,UID,"testuser",&zp);
cout << dummystring<<'\n';
      erfolgreich=C.teste(Check::Bandlager_einlagern,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Bandlager einlagern\n";
               return fehler();}

      {AufEintrag AE(AEB);
        AE.Produziert(300,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::Kunden_Teillieferung,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Kunde Teillieferung\n";
               return fehler();}

//cout << "\n\n120 ABSCHREIBEN\n\n\n";
      {AufEintrag AE(AEB);
        AE.Produziert(120,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::Kunden_Ueberlieferung,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Kunde Überlieferung\n";
               return fehler();} 

      cout << "Lager-Test erfolgreich\n";
#endif
      break;
     }
    case ZweiAuftraege:
     {
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,13000,PA,PLANDATUM6);
       erfolgreich=C.teste(Check::Planen_Faerberei_ueber,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Über-Planen der Färberei \n\n";
               return fehler();}
       }
       AufEintragBase AEB=auftrag.anlegen2();
       erfolgreich=C.teste(Check::ZweiAuftraege_anlegen,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] \n\n";
               return fehler();}
      cout << "ZweiAufträge-Test erfolgreich\n";
 
       break;
     }    
    case ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();

       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin \n\n";
               return fehler();}

       {
          AufEintrag AE(AEB);
        AE.Produziert(200,Lieferschein::none_id);
       }
       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum_abschreiben,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Teil-Abschreiben des zweiten Auftrags ["<<AEB<<"] \n\n";
               return fehler();}

//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
 
       AufEintrag(AEB).setStatus(CLOSED,UID);
       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum_closed,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Statussänderung(2) (Closed) \n\n";
               return fehler();}

       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       assert(AEP.getStueck()==AEP.getRestStk());
       AEP.Planen(UID,7000,PA,PLANDATUM5);
       erfolgreich=C.teste(Check::Planen_WebereiD,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Planen der Weberei\n\n";
               return fehler();}
 
       AE.setStatus(CLOSED,UID);
       erfolgreich=C.teste(Check::ErsterAuftrag_frueheresDatum_closed,mit_reparatur_programm);
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
       erfolgreich=C.teste(Check::LieferscheinTeil,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung anlegen\n\n"; return fehler();}

       int lznr=1;
       LieferscheinEntry le((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le);
       erfolgreich=C.teste(Check::LieferscheinZeileLoeschen,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinzeile löschen\n\n";return fehler();}              

       liefs.push_back(ARTIKEL_ROLLEREI,450,0,0);
       erfolgreich=C.teste(Check::LieferscheinVoll,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Volllieferung \n\n"; return fehler();}

       LieferscheinEntry le2((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le2);
       erfolgreich=C.teste(Check::LieferscheinZeileLoeschen_n,mit_reparatur_programm);
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
       erfolgreich=C.teste(Check::LieferscheinTeil,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung anlegen\n\n"; return fehler();}

       int stueck=140;
       AuftragBase::mengen_t menge=0;
       int lznr=1;
       LieferscheinEntry le(LieferscheinEntryBase(liefs,lznr));
       le.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinMengenaenderungMinus,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein Mengenaenderung Minus \n\n"; return fehler();}

       stueck=400;
       LieferscheinEntry le2(LieferscheinEntryBase(liefs,lznr));
       le2.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinMengenaenderungPlus,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein Mengenaenderung Plus \n\n"; return fehler();}

      cout << "Lieferschein Mengen-Test erfolgreich\n";

#endif


       break;
     }
    case Lieferscheintest_ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();
       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin \n\n";
                          return fehler();}

#ifdef PETIG_TEST

       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,50,0,0);
       erfolgreich=C.teste(Check::LieferscheinZweiAufTeil,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung und 2 Aufträgen anlegen\n\n"; return fehler();}

       liefs.push_back(ARTIKEL_ROLLEREI,600,0,0);
       erfolgreich=C.teste(Check::LieferscheinZweiAufVoll,mit_reparatur_programm);
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
       erfolgreich=C.teste(Check::LieferscheinZusatz,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Zusatzeintrag anlegen\n\n"; return fehler();}

       int stueck=633;
       AuftragBase::mengen_t menge=0;

       int lznr=1;
       LieferscheinEntry le3(LieferscheinEntryBase(liefs,lznr));
       le3.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinZusatzPlus,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Plus \n\n"; return fehler();}

       LieferscheinEntry le4(LieferscheinEntryBase(liefs,lznr));
       stueck=450;
       le4.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinZusatzMinus,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Minus \n\n"; return fehler();}

       LieferscheinEntry le5(LieferscheinEntryBase(liefs,lznr));
       stueck=350;
       le5.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinZusatzMinusKunde,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Minus Kunde \n\n"; return fehler();}

      cout << "Lieferschein Zusatz-Test erfolgreich\n";

#endif
       break;
     }
    case LieferscheinJacek:
     {
#ifdef MABELLA_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_TRIO,10,0,0);
       erfolgreich=C.teste(Check::LieferscheinVoll,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein mit Volllieferung (Mabella) anlegen\n\n"; return fehler();}
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);

       int lznr=1;
       LieferscheinEntry le1(LieferscheinEntryBase(liefs,lznr));
       int stueck=3;
       le1.changeMenge(stueck,0);
       erfolgreich=C.teste(Check::LieferscheinMengenaenderungMinus,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferscheinentry: Minus \n\n"; return fehler();}


       {// Produktionsplaner
        int znr=1;
        AufEintragBase OldAEB((AuftragBase(PRODPLANUNG,AuftragBase::ungeplante_id)),znr);
        {AufEintrag OldAE(OldAEB);
         Auftrag PA=Auftrag(Auftrag::Anlegen(PRODPLANUNG),ManuProC::DefaultValues::EigeneKundenId);
         OldAE.ProduktionsPlanung(UID,2,PA,PLANDATUM6,WEBEREI);
         erfolgreich=C.teste(Check::ProduktionsPlanungWeberei,mit_reparatur_programm);
         if(!erfolgreich) { cout << "ProduktionsPlanungWeberei \n\n"; return fehler();}
        }

        {AufEintrag OldAE(OldAEB);
         Auftrag PA=Auftrag(Auftrag::Anlegen(PRODPLANUNG),ManuProC::DefaultValues::EigeneKundenId);
         OldAE.ProduktionsPlanung(UID,20,PA,PLANDATUM6,EINKAUF);
         erfolgreich=C.teste(Check::ProduktionsPlanungEinkauf,mit_reparatur_programm);
         if(!erfolgreich) { cout << "ProduktionsPlanungEinkauf \n\n"; return fehler();}
        }
       }

      {// Überplanen des Einkaufs
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
      int znr=1;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
      int nznr=AEP.Planen(UID,27,PA,PLANDATUM5);
      erfolgreich=C.teste(Check::Planen_Kupfer,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Über-Planen des Einkaufs (Mabella) \n\n"; return fehler();}       
      }
//exit(1);

      {// Weberei liefert mehr als geplant
       Lieferschein liefs(WEBEREI,cH_Kunde(Kunde::eigene_id));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
       liefs.push_back(ARTIKEL_TRIO,5,0,0);
       erfolgreich=C.teste(Check::LieferscheinZusatz,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein in Weberei mit Überlieferung (Mabella) \n\n"; return fehler();}
      }

      {// Einkauf liefert Teilmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
       liefs.push_back(ARTIKEL_TRIO,13,0,0);
       erfolgreich=C.teste(Check::LieferscheinZusatzPlus,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein im Einkauf mit Teillieferung (Mabella) \n\n"; return fehler();}
      }

      {// Einkauf liefert Vollmenge
       Lieferschein liefs(EINKAUF,cH_Kunde(KUNDE2));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
       liefs.push_back(ARTIKEL_TRIO,25,0,0);
       erfolgreich=C.teste(Check::LieferscheinZusatzMinus,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Lieferschein im Einkauf Weberei mit Restlieferung (Mabella) \n\n"; return fehler();}
      }


      { // Bestellen eines Artikels ohne Kunden-Auftrag
        Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),KUNDE2);
        ManuProC::st_produziert sp(ARTIKEL_ZWEI,2222,getuid(),Kunde::eigene_id,LieferscheinBase::none_id,PA,SPLITDATUM);
        cH_ppsInstanz I(EINKAUF);
//cout << "L O S \n";
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
        I->Planen(sp);
        erfolgreich=C.teste(Check::ZweiAuftraege_anlegen,mit_reparatur_programm);
        if(!erfolgreich) { cout << "Einkauf eines nicht-bestelleten Artikel (Mabella) \n\n"; return fehler();}       
      }

      cout << "Test für Mabella erfolgreich\n";

#endif
      break;
     }
    case ZweiKundenTest:
     {
       AufEintragBase AEB2=auftrag.anlegenK();
       erfolgreich=C.teste(Check::ZweiKundenTest_anlegen,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["<<AEB<<"] \n\n";
               return fehler();}

      {AufEintrag AE(AEB);
        AE.Produziert(300,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::ZweiKunden_Teil1,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Zwei Kunden Teillieferung 1\n";
               return fehler();}

      {AufEintrag AE(AEB2);
        AE.Produziert(180,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::ZweiKunden_Teil2,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Zwei Kunden Teillieferung 2\n";
               return fehler();}

      {AufEintrag AE(AEB);

//cout << "\n\nnun\n\n";
//      ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
        AE.Produziert(200,Lieferschein::none_id);
      }
      erfolgreich=C.teste(Check::ZweiKunden_Ueber1,mit_reparatur_programm);
      if(!erfolgreich) { cout << "Zwei Kunden Volllieferung 1\n";
               return fehler();}

      cout << "Zwei Kunden-Test erfolgreich\n";


       break;
     }
    case ZweiKundenMengeFreigebenTest:
     {
       AufEintragBase AEB2=auftrag.anlegenK();
       erfolgreich=C.teste(Check::ZweiKundenTest_anlegen,mit_reparatur_programm);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["<<AEB<<"] \n\n";
               return fehler();}

       {
         cH_ppsInstanz I(ppsInstanzID::Bandlager);
         int znrvon=1;
         AufEintragBase Von(AuftragBase(I,AuftragBase::plan_auftrag_id),znrvon);
         int znrnach=2;
         AufEintrag Fuer((class AufEintragBase(AuftragBase(I,AuftragBase::ungeplante_id),znrnach)));
         AufEintrag(Von).menge_fuer_aeb_freigeben(3000,Fuer,getuid());
         erfolgreich=C.teste(Check::ZweiKundenMengeFuer,mit_reparatur_programm);
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
       JR=JumboRolle::create(KK);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1);
       erfolgreich=C.teste(Check::Jumbo_richtig,mit_reparatur_programm);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp1);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp0);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,UID,"TEST",&zp1);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0);
       erfolgreich=C.teste(Check::Jumbo_falsch,mit_reparatur_programm);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,UID,"TEST",&zp0);
       try
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
                  "\t(S)plittest|(Z)weiAuftraege|ZweiterAuftrag_frueheres(D)atum|\n"
                  "\t(L)iefer(s)cheine|(L)ieferscheine(m)engen|\n"
                  "\t(L)ieferschein(Z)usatzeintrag|(L)ieferscheinZweiter(A)uftrag_frueheresDatum|\n"
                  "\t(Z)wei(K)unden)\n"
                  "\t(Z)wei(K)unden(M)engeFreigeben\n"
                  "\t(M)anu(P)roCTest\n"
                  "\t(J)umboLager] aufgerufen werden\n"
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

