// $Id: steuerprogramm.cc,v 1.15 2002/10/04 08:23:21 thoma Exp $
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
#include <Instanzen/Produziert.h>
#include <unistd.h>
// Jumbo
#include <Ketten/KettplanKette.h>
#include <Lager/JumboLager.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>

#ifdef PETIG_EXTENSIONS

enum e_mode {None,Mengentest,Plantest,Lagertest,Splittest,ZweiAuftraege,
      ZweiterAuftrag_frueheresDatum,Lieferscheintest,LieferscheintestMenge,
      LieferscheintestZusatz,Lieferscheintest_ZweiterAuftrag_frueheresDatum,
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
   AufEintrag AE=AEB;
   Check C;
   bool erfolgreich=false;
   cout << "Anlegen eines Auftrags ["<<AEB<<"] beendet\n\n";

   // ANLEGEN eines Auftrags mit Bandlager und Rohwarenlager
   if (mode!=JumboLager)
   {  AE.setStatus(OPEN,UID);
      erfolgreich=C.teste(Check::Open);
      if(!erfolgreich) 
         { cout << "Öffnen des Auftrags fehlgeschlagen\n"; return fehler();}
   }

   switch(mode) {
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
       AEP.Planen(UID,5000,true,PA,PLANDATUM6);
       erfolgreich=C.teste(Check::Planen_WebereiP);
       if(!erfolgreich) { cout << "Planen der Weberei \n\n";
               return fehler();}
       }
      break;
     }
    case Splittest :
     {
      AE.split(UID,300,SPLITDATUM);
      erfolgreich=C.teste(Check::Split);
      if(!erfolgreich) { cout << "Splitten einer Auftragszeile \n\n";
               return fehler();}

      H_Lager RL((cH_ppsInstanz(ppsInstanzID::Rohwarenlager)));
      RL->rein_ins_lager(ARTIKEL_KUPFER,100,UID);
      erfolgreich=C.teste(Check::Split_Rohwarenlager_einlagern);
      if(!erfolgreich) { cout << "Rohwarenlager einlagern\n";
               return fehler();}

      RL->raus_aus_lager(ARTIKEL_KUPFER,100,UID);
      erfolgreich=C.teste(Check::Split_Rohwarenlager_auslagern);
      if(!erfolgreich) { cout << "Rohwarenlager auslagern\n";
               return fehler();}

      break;
     }
    case Lagertest :
     {    
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

//      AufEintrag(AEB).abschreiben(300);
      {AufEintrag AE(AEB);
       Produziert(AEB.Instanz()->Id(),AE.Artikel(),300,getuid()).NichtSelbst();
      }
      erfolgreich=C.teste(Check::Kunden_Teillieferung);
      if(!erfolgreich) { cout << "Kunde Teillieferung\n";
               return fehler();}

      {AufEintrag AE(AEB);
       Produziert(AEB.Instanz()->Id(),AE.Artikel(),120,getuid()).NichtSelbst();
      }
//      AufEintrag(AEB).abschreiben(120);
      erfolgreich=C.teste(Check::Kunden_Ueberlieferung);
      if(!erfolgreich) { cout << "Kunde Überlieferung\n";
               return fehler();}      break;
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
 
       break;
     }    
    case ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();

       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin \n\n";
               return fehler();}

       {
//          AufEintrag(AEB).abschreiben(200);
          AufEintrag AE(AEB);
       Produziert(AEB.Instanz()->Id(),AE.Artikel(),200,getuid()).NichtSelbst();
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

      break;
     }    
    case Lieferscheintest:
     {
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
      
       break;
     }
    case LieferscheintestMenge:
     {
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

       break;
     }
    case Lieferscheintest_ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();
       erfolgreich=C.teste(Check::ZweiterAuftrag_frueheresDatum);
       if(!erfolgreich) { cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin \n\n";
                          return fehler();}

       Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
       liefs.push_back(ARTIKEL_ROLLEREI,50,0,0);
       erfolgreich=C.teste(Check::LieferscheinZweiAufTeil);
       if(!erfolgreich) { cout << "Lieferschein mit Teillieferung und 2 Aufträgen anlegen\n\n"; return fehler();}

       liefs.push_back(ARTIKEL_ROLLEREI,600,0,0);
       erfolgreich=C.teste(Check::LieferscheinZweiAufVoll);
       if(!erfolgreich) { cout << "Lieferschein mit Volllieferung und 2 Aufträgen anlegen\n\n"; return fehler();}


       break;
     }
    case LieferscheintestZusatz:
     {
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
   else if(std::string(argv[1])=="J" || std::string(argv[1])=="JumboLager")  mode=JumboLager;

   if(mode==None) { usage(argv[0],argv[1]); return 1; }
   
   cout << "Initalisierung der Datenbank ...";
   system("database_tables_init/initdb.script");
   cout << "...beendet\n";

   Petig::PrintUncaughtExceptions();
   try{
   putenv("PGDATABASE=testdb");
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
