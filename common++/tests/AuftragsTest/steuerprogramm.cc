// $Id: steuerprogramm.cc,v 1.7 2002/06/27 07:26:10 christof Exp $
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
// Jumbo
#include <Ketten/KettplanKette.h>
#include <Lager/JumboLager.h>

enum e_mode {None,Mengentest,Plantest,Lagertest,Splittest,ZweiAuftraege,
      ZweiterAuftrag_frueheresDatum,JumboLager};

void auftragstests(e_mode mode)
{
   AuftragsVerwaltung  auftrag; 
   AufEintragBase AEB=auftrag.anlegen();
   AufEintrag AE=AEB;
   Check C;
   cout << "Anlegen eines Auftrags ["<<AEB<<"] beendet\n\n";

   // ANLEGEN eines Auftrags mit Bandlager und Rohwarenlager
   if (mode!=JumboLager)
   {  AE.setStatus(OPEN,UID);
      C.teste(Check::Open);
      cout << "Öffnen des Auftrags beendet\n";
   }

   switch(mode) {
    case Mengentest :
     {    
      // Menge des Auftrags erhöhen
      auftrag.kunden_bestellmenge_aendern(AEB,500);
      C.teste(Check::Menge_Plus);
      cout << "Erhöhen der Auftragmenge beendet\n\n";

      // Menge des Auftrags erniedrigen (Rohwarenlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,100);
      C.teste(Check::Menge_Minus);
      cout << "Reduzieren der Auftragmenge unter Rohwarenlagerbestand beendet\n\n";

      AE.updateLieferdatum(NEWDATUM,UID);
      C.teste(Check::Datumsaenderung);
      cout << "Datumsänderung beendet\n\n";

      // Menge des Auftrags weiter erniedrigen (Bandlager Menge reicht jetzt aus)
      auftrag.kunden_bestellmenge_aendern(AEB,10);
      C.teste(Check::Menge_MinusMinus);
      cout << "Reduzieren der Auftragmenge unter Bandlagerbestand beendet\n\n";

      AE.setStatus(CLOSED,UID);
      C.teste(Check::StatusClosed);
      cout << "Statussänderung (Closed) beendet\n\n";

      break;
     }
    case Plantest:
     {
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::_Garn__Einkauf),Kunde::default_id);
       int kupfer_znr=2;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::_Garn__Einkauf,AuftragBase::ungeplante_id,kupfer_znr));
       AEP.Planen(UID,100,true,PA,PLANDATUM5);
       C.teste(Check::Planen_Kupfer);
       cout << "Planen des Kupfereinkaufs beendet\n\n";
       }

       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,7000,true,PA,PLANDATUM4);
       C.teste(Check::Planen_Faerberei_teil);
       cout << "Teil-Planen der Färberei beendet\n\n";
       }

       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
       int weberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
       AEP.Planen(UID,5000,true,PA,PLANDATUM6);
       C.teste(Check::Planen_Weberei1);
       cout << "Planen der Weberei beendet\n\n";
       }

      break;
     }
    case Splittest :
     {
      AE.split(UID,100,SPLITDATUM);
      C.teste(Check::Split);
      cout << "Splitten einer Auftragszeile beendet\n\n";

      H_Lager RL((cH_ppsInstanz(ppsInstanzID::Rohwarenlager)));
      RL->rein_ins_lager(ARTIKEL_KUPFER,100,UID);
      C.teste(Check::Split_Rohwarenlager_einlagern);
      cout << "Rohwarenlager einlagern\n";

      RL->raus_aus_lager(ARTIKEL_KUPFER,100,UID);
      C.teste(Check::Split_Rohwarenlager_auslagern);
      cout << "Rohwarenlager auslagern\n";

      break;
     }
    case Lagertest :
     {    
      H_Lager RL((cH_ppsInstanz(ppsInstanzID::Rohwarenlager)));
      RL->rein_ins_lager(ARTIKEL_KUPFER,100,UID);
      C.teste(Check::Rohwarenlager_einlagern);
      cout << "Rohwarenlager einlagern\n";

      RL->raus_aus_lager(ARTIKEL_KUPFER,100,UID);
      C.teste(Check::Rohwarenlager_auslagern);
      cout << "Rohwarenlager auslagern\n";

      Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
      int weberei_znr=1;
      AufEintrag AEP(AufEintragBase(ppsInstanzID::Weberei,AuftragBase::ungeplante_id,weberei_znr));
      assert(AEP.getStueck()==AEP.getRestStk());
      AEP.Planen(UID,5000,true,PA,PLANDATUM5);
      C.teste(Check::Planen_Weberei);
      cout << "Planen der Weberei zum späteren Test des Bandlagers beendet\n\n";

      H_Lager BL((cH_ppsInstanz(ppsInstanzID::Bandlager)));
      BL->rein_ins_lager(ARTIKEL_BANDLAGER,12000,UID);
      C.teste(Check::Bandlager_einlagern);
      cout << "Bandlager einlagern\n";

      break;
     }
    case ZweiAuftraege:
     {
       {
       Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Faerberei),Kunde::default_id);
       int faerberei_znr=1;
       AufEintrag AEP(AufEintragBase(ppsInstanzID::Faerberei,AuftragBase::ungeplante_id,faerberei_znr));
       AEP.Planen(UID,13000,true,PA,PLANDATUM6);
       C.teste(Check::Planen_Faerberei_ueber);
       cout << "Über-Planen der Färberei beendet\n\n";
       }

       AufEintragBase AEB=auftrag.anlegen2();
       C.teste(Check::ZweiAuftraege_anlegen);
       cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] beendet\n\n";
      break;
     }    
    case ZweiterAuftrag_frueheresDatum:
     {
       AufEintragBase AEB=auftrag.anlegen3();
       C.teste(Check::ZweiterAuftrag_frueheresDatum);
       cout << "Anlegen eines zweiten (offenen) Auftrags ["<<AEB<<"] mit früherem Liefertermin beendet\n\n";
      break;
     }    
    case JumboLager:
     { LagerPlatz LP(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ);
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
       C.teste(Check::Jumbo_richtig);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp1);
       JL.Jumbo_Einlagern(LP,JR.front(),JumboLager::Einlagern,"TEST",&zp0);
       JR=JumboRolle::create(KK);
       JL.Jumbo_Einlagern(LP2,JR.front(),JumboLager::Einlagern,"TEST",&zp1);
       JL.Jumbo_Entnahme(JR.front(),JumboLager::Auslagern,"TEST",&zp0);
       C.teste(Check::Jumbo_falsch);
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
       C.teste(Check::Jumbo_doppelt);
       break;
     }
    case None: assert(!"Never get here\n");
   }
}

void usage(const std::string &argv0,const std::string &argv1)
{
  cerr << argv0 <<" muß mit [(M)engentest|(P)lantest|(L)agertest|(S)plittest,(Z)weiAuftraege,ZweiterAuftrag_frueheres(D)atum,(J)umboLager] aufgerufen werden\n"
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
   auftragstests(mode);
   
   }catch(SQLerror &e){std::cout << e<<'\n';}
  return 0;
}

