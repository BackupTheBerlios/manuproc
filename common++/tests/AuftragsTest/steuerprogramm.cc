// $Id: steuerprogramm.cc,v 1.17 2002/10/24 14:06:50 thoma Exp $
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
// Jumbo
#include <Ketten/KettplanKette.h>
#include <Lager/JumboLager.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>

#if defined  PETIG_EXTENSIONS || defined MANU_PROC_TEST

enum e_mode {None,Mengentest,Plantest,Lagertest,Splittest,ZweiAuftraege,
      ZweiterAuftrag_frueheresDatum,Lieferscheintest,LieferscheintestMenge,
      LieferscheintestZusatz,Lieferscheintest_ZweiterAuftrag_frueheresDatum,
      ZweiKundenTest,ManuProCTest,
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
   AuftragsVerwaltung  auftrag; 
   AufEintragBase AEB=auftrag.anlegen();
//      ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
   AufEintrag AE=AEB;
   Check C;
   bool erfolgreich=false;
   cout << "Anlegen eines Auftrags ["<<AEB<<"] beendet\n\n";

   // ANLEGEN eines Auftrags mit Bandlager und Rohwarenlager
   if (mode!=JumboLager)
   {  
      AE.setStatus(OPEN,UID);
      erfolgreich=C.teste(Check::Open);
      if(!erfolgreich) 
         { cout << "Öffnen des Auftrags fehlgeschlagen\n"; return fehler();}
   }

//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);

   switch(mode) {
    case ManuProCTest :
     {
      #ifdef MANU_PROC_TEST

      {// Planen des Einkaufs (Granulat, grün)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=4;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,200,true,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      erfolgreich=C.teste(Check::Planen_Kupfer);
      if(!erfolgreich) { cout << "Planen des Einkaufs(Granulat) \n\n"; return fehler();}       

      // Spezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(PAE,ARTIKEL_GRANULAT_GRUEN,1,200,0);
//unspezifisch   liefs.push_back(ARTIKEL_GRANULAT_GRUEN,1,200,0);
      erfolgreich=C.teste(Check::LieferscheinTeil);
      if(!erfolgreich) { cout << "Lieferschein (mit AEB, Granulat) anlegen\n\n"; return  fehler();}
      }


      {// Planen des Einkaufs (Metall)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=5;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,10,true,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      erfolgreich=C.teste(Check::Planen_WebereiL);
      if(!erfolgreich) { cout << "Planen des Einkaufs(Metall) \n\n"; return fehler();}       


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_METALL,1,5,0);
      erfolgreich=C.teste(Check::LieferscheinVoll);
      if(!erfolgreich) { cout << "Lieferschein (unbestimmt, Metall) anlegen\n\n"; return  fehler();}
      }


      {// Planen der Gießerei (ARTIKEL_GRIFF_ROT)
      Auftrag PA=Auftrag(Auftrag::Anlegen(GIESSEREI),ManuProC::DefaultValues::EigeneKundenId);
      int znr=1;
      AufEintrag AEP((AufEintragBase(GIESSEREI,AuftragBase::ungeplante_id,znr)));
      int nznr=AEP.Planen(UID,500,true,PA,PLANDATUM5);
      AufEintrag PAE(AufEintragBase(PA,nznr));
      erfolgreich=C.teste(Check::Planen_Faerberei_teil);
      if(!erfolgreich) { cout << "Planen der Giesserei (Griff rot) \n\n"; return fehler();}       


      // Unspezifischen Lieferschein schreiben
      Lieferschein liefs(GIESSEREI,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(ARTIKEL_GRIFF_ROT,500,0,0);
      erfolgreich=C.teste(Check::LieferscheinZusatz);
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

      erfolgreich=C.teste(Check::LieferscheinZusatzPlus);
      if(!erfolgreich) { cout << "Produktion in der Werkstatt anlegen\n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für das Schraubenzieherlager => auslagern 
      Lieferschein liefs(SCHRAUBENZIEHERLAGER,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
//cout << "\n\n\nHier gehts los\n";
      liefs.push_back(ARTIKEL_SCHRAUBENZIEHER_ROT,450,0,0);
      erfolgreich=C.teste(Check::LieferscheinZusatzMinus);
      if(!erfolgreich) { cout << "Lieferschein für das Rohwarenlager (auslagern)\n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für den Kunden 
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
//cout << "\n\n\nHier gehts los\n";
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,450,0,0);
      erfolgreich=C.teste(Check::LieferscheinZusatzMinusKunde);
      if(!erfolgreich) { cout << "Lieferschein für den Kunden \n\n"; return  fehler();}
      }

      {// Lieferscheinschreiben für den Kunden  (Überlieferung)
      Lieferschein liefs(KUNDENINSTANZ,cH_Kunde(KUNDE));
//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
//cout << "\n\n\nHier gehts los\n";
      liefs.push_back(ARTIKEL_SORTIMENT_BUNT,60,0,0);
      erfolgreich=C.teste(Check::LieferscheinZweiAufTeil);
      if(!erfolgreich) { cout << "Lieferschein für den Kunden (Überlieferung)\n\n"; return  fehler();}
      }

      #endif
      break;
     }
    case Mengentest :
     {    
      // Menge des Auftrags erhöhen
      auftrag.kunden_bestellmenge_aendern(AEB,500);
      erfolgreich=C.teste(Check::Menge_Plus);
      if(!erfolgreich) {cout << "Erhöhen der Auftragmenge \n\n";
                        return fehler();}


      // Menge des Auftrags erniedrigen (Rohwarenlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,100);
      erfolgreich=C.teste(Check::Menge_Minus);
      if(!erfolgreich) { cout << "Reduzieren der Auftragmenge unter Rohwarenlagerbestand \n\n";
               return fehler();}


      AE.updateLieferdatum(NEWDATUM,UID);
      erfolgreich=C.teste(Check::Datumsaenderung);
      if(!erfolgreich) { cout << "Datumsänderung \n\n";
               return fehler();}


      // Menge des Auftrags weiter erniedrigen (Bandlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,10);
      erfolgreich=C.teste(Check::Menge_MinusMinus);
      if(!erfolgreich) { cout << "Reduzieren der Auftragmenge unter Bandlagerbestand \n\n";
               return fehler();}

      AufEintrag(AEB).setStatus(CLOSED,UID);
      erfolgreich=C.teste(Check::StatusClosed);
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
       AEP.Planen(UID,100,true,PA,PLANDATUM5);
       erfolgreich=C.teste(Check::Planen_Kupfer);
       if(!erfolgreich) { cout << "Planen des Kupfereinkaufs \n\n";
               return fehler();}       
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,7000,true,PA,PLANDATUM4);
       erfolgreich=C.teste(Check::Planen_Faerberei_teil);
       if(!erfolgreich) { cout << "Teil-Planen der Färberei \n\n";
               return fehler();}
       }
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));

//ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);


       AEP.Planen(UID,5000,true,PA,PLANDATUM6);
       erfolgreich=C.teste(Check::Planen_WebereiP);
       if(!erfolgreich) { cout << "Planen der Weberei \n\n";
               return fehler();}
       }
      cout << "Plan-Test erfolgreich\n";

      break;
     }
    case Splittest :
     {
      AE.split(UID,300,SPLITDATUM);
      erfolgreich=C.teste(Check::Split);
      if(!erfolgreich) { cout << "Splitten einer Auftragszeile \n\n";
               return fehler();}

#ifndef MANU_PROC_TEST
      H_Lager RL((cH_ppsInstanz(ppsInstanzID::Rohwarenlager)));
      RL->rein_ins_lager(ARTIKEL_KUPFER,100,UID);
      erfolgreich=C.teste(Check::Split_Rohwarenlager_einlagern);
      if(!erfolgreich) { cout << "Rohwarenlager einlagern\n";
               return fehler();}

      RL->raus_aus_lager(ARTIKEL_KUPFER,100,UID);
      erfolgreich=C.teste(Check::Split_Rohwarenlager_auslagern);
      if(!erfolgreich) { cout << "Rohwarenlager auslagern\n";
               return fehler();}
      cout << "Split-Test erfolgreich\n";
#endif

      break;
     }
    case Lagertest :
     {    
#ifndef MANU_PROC_TEST
      H_Lager RL((cH_ppsInstanz(ppsInstanzID::Rohwarenlager)));
      RL->rein_ins_lager(ARTIKEL_KUPFER,100,UID);
      erfolgreich=C.teste(Check::Rohwarenlager_einlagern);
      if(!erfolgreich) { cout << "Rohwarenlager einlagern\n";
               return fehler();}

      RL->raus_aus_lager(ARTIKEL_KUPFER,120,UID);
      erfolgreich=C.teste(Check::Rohwarenlager_auslagern);
      if(!erfolgreich) { cout << "Rohwarenlager auslagern\n";
               return fehler();}

      Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
      int weberei_znr=1;
      AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
      assert(AEP.getStueck()==AEP.getRestStk());
      AEP.Planen(UID,5000,true,PA,PLANDATUM5);
      erfolgreich=C.teste(Check::Planen_WebereiL);
      if(!erfolgreich) { cout << "Planen der Weberei zum späteren Test des Bandlagers \n\n";
               return fehler();}

      H_Lager BL((cH_ppsInstanz(ppsInstanzID::Bandlager)));
      BL->rein_ins_lager(ARTIKEL_BANDLAGER,12000,UID);
      erfolgreich=C.teste(Check::Bandlager_einlagern);
      if(!erfolgreich) { cout << "Bandlager einlagern\n";
               return fehler();}

      {AufEintrag AE(AEB);
        AE.Produziert(300);
      }
      erfolgreich=C.teste(Check::Kunden_Teillieferung);
      if(!erfolgreich) { cout << "Kunde Teillieferung\n";
               return fehler();}

//cout << "\n\n120 ABSCHREIBEN\n\n\n";
      {AufEintrag AE(AEB);
        AE.Produziert(120);
      }
      erfolgreich=C.teste(Check::Kunden_Ueberlieferung);
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
       AEP.Planen(UID,13000,true,PA,PLANDATUM6);
       erfolgreich=C.teste(Check::Planen_Faerberei_ueber);
       if(!erfolgreich) { cout << "Über-Planen der Färberei \n\n";
               return fehler();}
       }
       AufEintragBase AEB=auftrag.anlegen2();
       erfolgreich=C.teste(Check::ZweiAuftraege_anlegen);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] \n\n";
               return fehler();}
      cout << "ZweiAufträge-Test erfolgreich\n";
 
       break;
     }    
    case ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();

       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin \n\n";
               return fehler();}

       {
          AufEintrag AE(AEB);
        AE.Produziert(200);
       }
       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum_abschreiben);
       if(!erfolgreich) { cout << "Teil-Abschreiben des zweiten Auftrags ["<<AEB<<"] \n\n";
               return fehler();}
 
       AufEintrag(AEB).setStatus(CLOSED,UID);
       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum_closed);
       if(!erfolgreich) { cout << "Statussänderung(2) (Closed) \n\n";
               return fehler();}

       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       assert(AEP.getStueck()==AEP.getRestStk());
       AEP.Planen(UID,7000,true,PA,PLANDATUM5);
       erfolgreich=C.teste(Check::Planen_WebereiD);
       if(!erfolgreich) { cout << "Planen der Weberei\n\n";
               return fehler();}
 
       AE.setStatus(CLOSED,UID);
       erfolgreich=C.teste(Check::ErsterAuftrag_frueheresDatum_closed);
       if(!erfolgreich) { cout << "Statussänderung(1) (Closed)\n\n";
               return fehler();}       

      cout << "ZweiAufträgeDatum erfolgreich\n";

      break;
     }    
    case Lieferscheintest:
     {
#ifndef MANU_PROC_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,150,0,0);
       erfolgreich=C.teste(Check::LieferscheinTeil);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung anlegen\n\n"; return fehler();}

       int lznr=1;
       LieferscheinEntry le((LieferscheinEntryBase(liefs,lznr)));
       LieferscheinEntry::deleteEntry(le);
       erfolgreich=C.teste(Check::LieferscheinZeileLoeschen);
       if(!erfolgreich) { cout << "Lieferscheinzeile löschen\n\n";return fehler();}              

       liefs.push_back(ARTIKEL_ROLLEREI,450,0,0);
       erfolgreich=C.teste(Check::LieferscheinVoll);
       if(!erfolgreich) { cout << "Lieferschein mit Volllieferung \n\n"; return fehler();}
      
      cout << "Lieferschein Test erfolgreich\n";
#endif

       break;
     }
    case LieferscheintestMenge:
     {
#ifndef MANU_PROC_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,150,0,0);
       erfolgreich=C.teste(Check::LieferscheinTeil);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung anlegen\n\n"; return fehler();}

       int stueck=140;
       AuftragBase::mengen_t menge=0;
       int lznr=1;
       LieferscheinEntry le(LieferscheinEntryBase(liefs,lznr));
       le.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinMengenaenderungMinus);
       if(!erfolgreich) { cout << "Lieferschein Mengenaenderung Minus \n\n"; return fehler();}

       stueck=400;
       LieferscheinEntry le2(LieferscheinEntryBase(liefs,lznr));
       le2.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinMengenaenderungPlus);
       if(!erfolgreich) { cout << "Lieferschein Mengenaenderung Plus \n\n"; return fehler();}

      cout << "Lieferschein Mengen-Test erfolgreich\n";

#endif


       break;
     }
    case Lieferscheintest_ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();
       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin \n\n";
                          return fehler();}

#ifndef MANU_PROC_TEST

       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,50,0,0);
       erfolgreich=C.teste(Check::LieferscheinZweiAufTeil);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung und 2 Aufträgen anlegen\n\n"; return fehler();}

       liefs.push_back(ARTIKEL_ROLLEREI,600,0,0);
       erfolgreich=C.teste(Check::LieferscheinZweiAufVoll);
       if(!erfolgreich) { cout << "Lieferschein mit Volllieferung und 2 Aufträgen anlegen\n\n"; return fehler();}

      cout << "Lieferschein Datums-Test erfolgreich\n";
#endif

       break;
     }
    case LieferscheintestZusatz:
     {
#ifndef MANU_PROC_TEST
       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,550,0,0);
       erfolgreich=C.teste(Check::LieferscheinZusatz);
       if(!erfolgreich) { cout << "Lieferschein mit Zusatzeintrag anlegen\n\n"; return fehler();}

       int stueck=633;
       AuftragBase::mengen_t menge=0;
       int lznr=2;
       LieferscheinEntry le(LieferscheinEntryBase(liefs,lznr));
       le.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinZusatz);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag darf sich nicht ändern lassen Minus \n\n"; return fehler();}

       lznr=3;
       LieferscheinEntry le2(LieferscheinEntryBase(liefs,lznr));
       le2.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinZusatz);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag darf sich nicht ändern lassen Minus \n\n"; return fehler();}

       lznr=1;
       LieferscheinEntry le3(LieferscheinEntryBase(liefs,lznr));
       le3.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinZusatzPlus);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Plus \n\n"; return fehler();}

       LieferscheinEntry le4(LieferscheinEntryBase(liefs,lznr));
       stueck=450;
       le4.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinZusatzMinus);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Minus \n\n"; return fehler();}

       LieferscheinEntry le5(LieferscheinEntryBase(liefs,lznr));
       stueck=350;
       le4.changeMenge(stueck,menge);
       erfolgreich=C.teste(Check::LieferscheinZusatzMinusKunde);
       if(!erfolgreich) { cout << "Lieferscheinentry mit Zusatzeintrag Minus Kunde \n\n"; return fehler();}

      cout << "Lieferschein Zusatz-Test erfolgreich\n";

#endif
       break;
     }
    case ZweiKundenTest:
     {
       AufEintragBase AEB2=auftrag.anlegenK();
       erfolgreich=C.teste(Check::ZweiKundenTest_anlegen);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["<<AEB<<"] \n\n";
               return fehler();}

      {AufEintrag AE(AEB);
        AE.Produziert(300);
      }
      erfolgreich=C.teste(Check::ZweiKunden_Teil1);
      if(!erfolgreich) { cout << "Zwei Kunden Teillieferung 1\n";
               return fehler();}

      {AufEintrag AE(AEB2);
        AE.Produziert(180);
      }
      erfolgreich=C.teste(Check::ZweiKunden_Teil2);
      if(!erfolgreich) { cout << "Zwei Kunden Teillieferung 2\n";
               return fehler();}

      {AufEintrag AE(AEB);

//cout << "\n\nnun\n\n";
//      ManuProC::Tracer::Enable(ManuProC::Tracer::Auftrag);
        AE.Produziert(200);
      }
      erfolgreich=C.teste(Check::ZweiKunden_Ueber1);
      if(!erfolgreich) { cout << "Zwei Kunden Volllieferung 1\n";
               return fehler();}

      cout << "Zwei Kunden-Test erfolgreich\n";


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
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,"TEST",&zp1);
       erfolgreich=C.teste(Check::Jumbo_richtig);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,"TEST",&zp1);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0);
       erfolgreich=C.teste(Check::Jumbo_falsch);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0);
       try
       {JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0b);
        assert(!"Jumbo_Entnahme sollte 100 werfen");
       }catch (SQLerror &e)
       {  assert(e.Code()==100);
       }
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,"TEST",&zp1);
       erfolgreich=C.teste(Check::Jumbo_doppelt);
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
   else if(std::string(argv[1])=="ZK" || std::string(argv[1])=="ZweiKunden")  mode=ZweiKundenTest;
   else if(std::string(argv[1])=="MP" || std::string(argv[1])=="ManuProCTest")  mode=ManuProCTest;
   else if(std::string(argv[1])=="J" || std::string(argv[1])=="JumboLager")  mode=JumboLager;

   if(mode==None) { usage(argv[0],argv[1]); return 1; }
   
   cout << "Initalisierung der Datenbank ...";
#ifdef MANU_PROC_TEST
   system((std::string(MANU_DATAPATH)+"/initdb.script "+MANU_DATAPATH).c_str());
   putenv("PGDATABASE=anleitungdb");
#else
   system("database_tables_init/initdb.script");
   putenv("PGDATABASE=testdb");
#endif
   cout << "...beendet\n";

   Petig::PrintUncaughtExceptions();
   try{
   Petig::dbconnect();  
   
   DataBase_init();
   return auftragstests(mode);
   
   }catch(SQLerror &e){std::cout << e<<'\n';}
  return 0;
}
#else
int main()
{
}
#endif
