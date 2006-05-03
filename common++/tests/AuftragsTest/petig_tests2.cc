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

#include <fstream>
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
#include <Auftrag/Verfuegbarkeit.h>
#endif

std::string AEB2filename(const AufEintragBase &AEB)
{  return itos(AEB.Instanz()->Id())+"."+itos(AEB.Id())+"."+itos(AEB.ZNr());
}

void verf_vergleichen(const std::string &postfix,const AufEintragBase &AEB)
{  Verfuegbarkeit::map_t mp;
   Verfuegbarkeit::verfuegbar(AEB,mp);
   vergleichstream vos=getCheck().vergleich_open("verf_"+postfix+"_"+AEB2filename(AEB));
   std::ofstream &os=(*vos.stream);
   os << "Verfügbarkeit für " << AEB << '\n';
   os << "Instanz\tArtikel\tKunde\tMenge\n";
   for (Verfuegbarkeit::map_t::const_iterator i=mp.begin();i!=mp.end();++i)
   {  os << i->first.inst << '\t'
   	<< i->first.art << '\t'
   	<< i->first.kunde << '\t';
      if (!!i->second.geliefert) os << 'g' << i->second.geliefert << ' ';
      if (!!i->second.vorraetig) os << 'v' << i->second.vorraetig << ' ';
      if (!!i->second.geplant) os << 'p' << i->second.geplant << ' ';
      if (!!i->second.ungeplant) os << 'u' << i->second.ungeplant << ' ';
      if (!!i->second.error) os << 'E' << i->second.error;
      os << '\n';
   }
   if (getCheck().vergleich_close(vos))
   {  std::cout << postfix << " Verfügbarkeit fehlgeschlagen\n";
      if (!Check::continue_) exit(1);
   }
}
void ben_vergleichen(const std::string &postfix,const AufEintragBase &AEB)
{  Verfuegbarkeit::map_t mp;
   Verfuegbarkeit::wozu_benoetigt(AEB,mp);
   vergleichstream vos=getCheck().vergleich_open("ben_"+postfix+"_"+AEB2filename(AEB));
   std::ofstream &os=(*vos.stream);
   os << "Verwendung für " << AEB << '\n';
   os << "Instanz\tArtikel\tKunde\tMenge\n";
   for (Verfuegbarkeit::map_t::const_iterator i=mp.begin();i!=mp.end();++i)
   {  os << i->first.inst << '\t'
   	<< i->first.art << '\t'
   	<< i->first.kunde << '\t';
      if (!!i->second.geliefert) os << 'g' << i->second.geliefert << ' ';
      if (!!i->second.vorraetig) os << 'v' << i->second.vorraetig << ' ';
      if (!!i->second.geplant) os << 'p' << i->second.geplant << ' ';
      if (!!i->second.ungeplant) os << 'u' << i->second.ungeplant << ' ';
      if (!!i->second.error) os << 'E' << i->second.error;
      os << '\n';
   }
   if (getCheck().vergleich_close(vos))
   {  std::cout << postfix << " Verwendung fehlgeschlagen\n";
      if (!Check::continue_) exit(1);
   }
}

void ben_vergleichen2(const std::string &postfix,const AufEintragBase &AEB)
{  Verfuegbarkeit::map_det_t mp;
   Verfuegbarkeit::wozu_benoetigt(AEB,mp);
   vergleichstream vos=getCheck().vergleich_open("ben2_"+postfix+"_"+AEB2filename(AEB));
   std::ofstream &os=(*vos.stream);
   os << "Verwendung für " << AEB << '\n';
   os << "AEB\tMenge\n";
   for (Verfuegbarkeit::map_det_t::const_iterator i=mp.begin();i!=mp.end();++i)
   {  os << i->first << '\t'
   	<< i->second << '\n';
   }
   if (getCheck().vergleich_close(vos))
   {  std::cout << postfix << " Verwendung2 fehlgeschlagen\n";
      if (!Check::continue_) exit(1);
   }
}

static bool f_Verfuegbarkeit()
{   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
	// soll Bandlager aufnehmen
    AufEintragBase AEB2=auftrag.push_back(2400,DATUM-20,ARTIKEL_FAERBEREI,OPEN,true);
    AufEintragBase AEB3=auftrag.push_back(400,DATUM-10,ARTIKEL_FAERBEREI,OPEN,true);
    AufEintragBase AEB4=auftrag.push_back(400,DATUM,ARTIKEL_FAERBEREI,OPEN,true);
    vergleichen(Check::Menge,"V_Ausgangspunkt","Ausgangspunkt","");
    
    Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
//  ehemals AufEintragBase AEBP=PA.Planen(1200,ARTIKEL_BANDLAGER,DATUM-40);
    AufEintragBase AEBP=PA.push_back(1200,DATUM-40,ARTIKEL_BANDLAGER,OPEN,true);
    vergleichen(Check::Menge,"V_Planung","Planung Weberei","");
    
    return true;
}

static TestReihe Verfuegbarkeit_(&f_Verfuegbarkeit,"Verfügbarkeit","V");

static std::vector<AufEintragBase> Verfuegbarkeit2_init()
{   std::vector<AufEintragBase> result;
    // Lager leeren
    RohwarenLager RL;
    std::string os;
    {RohwarenLager::st_rohlager rohlager(
    		LagerPlatz(ppsInstanzID::Rohwarenlager,KUPFER_LAGERPLATZ),
    		0,0,0,0,ARTIKEL_KUPFER,ManuProC::Datum().today());
     RL.RL_Entnahme(rohlager,os,true,true,false); // Lagerplatz leeren
    }
#if 0    
    {RohwarenLager::st_rohlager rohlager(
    		LagerPlatz(ppsInstanzID::Rohwarenlager,ACETAT_LAGERPLATZ),
    		0,0,0,0,ARTIKEL_ACETAT,ManuProC::Datum().today());
     RL.RL_Entnahme(rohlager,os,true,true,false); // Lagerplatz leeren
    }
#endif    
    {  class JumboLager JL;
       JumboRolle jr(JumboRolle::Pruefziffer_anhaengen(100));
       JL.Jumbo_Entnahme(jr,JumboLager::Auslagern,"TEST",0,false);
    }

    // Einkauf planen
    Auftrag PAE=Auftrag(Auftrag::Anlegen(ppsInstanzID::Einkauf),Kunde::default_id);
    AufEintragBase AEBPE=PAE.push_back(15,DATUM-60,ARTIKEL_KUPFER,OPEN,true);
    result.push_back(AEBPE);

    // Weberei planen
    Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
    AufEintragBase AEBP=PA.push_back(400,DATUM-20,ARTIKEL_BANDLAGER,OPEN,true);
    result.push_back(AEBP);
//    vergleichen(Check::Menge,"V2_Planung","Planung Weberei","");

    // Kunde bestellt
    Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
    AufEintragBase AEB3=auftrag.push_back(0,DATUM,ARTIKEL_BANDLAGER,OPEN,true);
    Auftrag auftrag2=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE2);
    AufEintragBase AEB4=auftrag2.push_back(100,DATUM,ARTIKEL_BANDLAGER,OPEN,true);
    AufEintrag(AEB3).MengeAendern(100);
    AufEintrag(AEB4).MengeAendern(100);
    AufEintrag(AEB3).MengeAendern(100);
    result.push_back(AEB3);
    result.push_back(AEB4);
//    vergleichen(Check::Menge,"V2_Ausgangspunkt","Ausgangspunkt","");
    return result;
}

static void Verfuegbarkeit2_check(const std::string &was, const std::vector<AufEintragBase> &aebs)
{   verf_vergleichen(was,aebs[2]);
    verf_vergleichen(was,aebs[3]);
    ben_vergleichen(was,aebs[0]);
    ben_vergleichen(was,AufEintragBase(ppsInstanzID::Einkauf,0,1));
    ben_vergleichen2(was,aebs[0]);
}

static bool Verfuegbarkeit2()
{   std::vector<AufEintragBase> aebs=Verfuegbarkeit2_init();
    // Weberei liefern    
    // erzeugt Inkonsistenz !!!
#if 0 // kann keinen Lieferschein schreiben ...
    Lieferschein liefs(ppsInstanzID::Weberei,cH_Kunde(KUNDE));
    LieferscheinEntryBase lsb(liefs,liefs.push_back(ARTIKEL_BANDLAGER,1,100));
    LieferscheinEntry(lsb).changeStatus(OPEN,false);
#else
    AufEintrag(make_value(aebs[1])).ProduziertNG(100,ProductionContext2());
#endif    

    if (Check::reparieren || Check::analyse)
    {  std::cerr << "V2 geht nur ohne -y/-r, die Inkonsistenz ist gewollt\n";
       return false;
    }
    vergleichen(Check::Menge,"V2_Auslieferung","Auslieferung","L");
    Verfuegbarkeit2_check("V2_Auslieferung",aebs);
    return true;
}

static TestReihe Verfuegbarkeit2_(&Verfuegbarkeit2,"Verfügbarkeit Grafik","V2");

static bool Verfuegbarkeit3()
{   std::vector<AufEintragBase> aebs=Verfuegbarkeit2_init();

    // Einkauf liefert
    {Lieferschein liefs(make_value(cH_ppsInstanz(ppsInstanzID::Einkauf)),cH_Kunde(Kunde::default_id));
     LieferscheinEntryBase lsb(liefs,liefs.push_back(ARTIKEL_KUPFER,1,13));
     LieferscheinEntry(lsb).changeStatus(OPEN,false);
    }
    vergleichen(Check::Menge,"V3_Einkauf","Einkauf","E");
    Verfuegbarkeit2_check("V3_Einkauf",aebs);

    // einlagern     
    {RohwarenLager RL;
     std::string os;
     RohwarenLager::st_rohlager rohlager(
                LagerPlatz(ppsInstanzID::Rohwarenlager,KUPFER_LAGERPLATZ),
                0,0,1,13,ARTIKEL_KUPFER,ManuProC::Datum().today());
     RL.RL_Einlagern(LagerPlatz(ppsInstanzID::Rohwarenlager,KUPFER_LAGERPLATZ),
     		rohlager,os,true,true);
    }
    vergleichen(Check::Menge,"V3_Einlagern","Einlagern","R");
    Verfuegbarkeit2_check("V3_Einlagern",aebs);

    JumboRolle::ID jrid=0;    
    // gefertigt
    {LagerPlatz LP(ppsInstanzID::Bandlager,JUMBO_LAGERPLATZ+1);
     KettplanKette KK=KettplanKette(Kette(MASCHINE,SCHAERDATUM));
     std::vector<JumboRolle> JR=JumboRolle::create(KK);
     jrid=JR.front().Id();
     Query("update rohjumbo set barcoist=?,barcofert_datum=now() where code=?")
    	<< 140
    	<< jrid/10;
     // jr aus DB neuladen
     JumboRolle J2(jrid);
     JumboLager().Jumbo_Einlagern(LP,J2,JumboLager::Einlagern,"TEST",0,true);
    }
    vergleichen(Check::Menge,"V3_gewebt","gewebt","W");
    Verfuegbarkeit2_check("V3_gewebt",aebs);

    // ausliefern (etwas zuviel)
    {JumboRolle J2(jrid);
     JumboLager().Jumbo_Entnahme(J2,JumboLager::Auslagern,"TEST",0,true);
     Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE2));
     LieferscheinEntryBase lsb(liefs,liefs.push_back(ARTIKEL_BANDLAGER,1,170));
     LieferscheinEntry(lsb).changeStatus(OPEN,false);
    }
    vergleichen(Check::Menge,"V3_Lieferung","Lieferung","L");
    Verfuegbarkeit2_check("V3_Lieferung",aebs);
    return true;
}

static TestReihe Verfuegbarkeit3_(&Verfuegbarkeit3,"Verfügbarkeit Grafik 3","V3");

static bool Lieferscheinzeile_aufteilen(AufEintrag &AE)
{ Auftrag auftrag=Auftrag(Auftrag::Anlegen(AE.Instanz()->Id()),AE.getKdNr());
  AufEintragBase AEB2=auftrag.push_back(AE.getStueck(),AE.getLieferdatum()+1,AE.Artikel(),OPEN,true);
  vergleichen(Check::Menge,"LZa_Ausgangspunkt","Ausgangspunkt","");

  // Lieferung global
  {Lieferschein liefs(AE.Instanz(),cH_Kunde(AE.getKdNr()));
   LieferscheinEntryBase lsb(liefs,liefs.push_back(AE.Artikel(),1,1.5*AE.getStueck()));
   LieferscheinEntry(lsb).changeStatus(OPEN,false);
   vergleichen(Check::Menge|Check::Lieferschein,"LZa_LieferA","Lieferung 1.5","");
   LieferscheinEntry(lsb).changeStatus(STORNO,false);
  }
  // Lieferung gezielt 2.
  {Lieferschein liefs(AE.Instanz(),cH_Kunde(AE.getKdNr()));
   AufEintrag AE2(AEB2);
   LieferscheinEntryBase lsb(liefs,liefs.push_back(AE2,AE2.Artikel(),1,AE2.getStueck()));
   LieferscheinEntry(lsb).changeStatus(OPEN,false);
   vergleichen(Check::Menge|Check::Lieferschein,"LZa_LieferB","Lieferung B","b");
  }
  return true;
}

static TestReihe Lieferscheinzeile_aufteilen_(&Lieferscheinzeile_aufteilen,"Lieferscheinzeile aufteilen","LZa");

#endif
