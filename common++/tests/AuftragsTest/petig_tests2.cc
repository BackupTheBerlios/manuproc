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
{
}

static bool Verfuegbarkeit2()
{   // Lager leeren
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
       JL.Jumbo_Entnahme(jr,JumboLager::Auslagern,"test",0,false);
    }

    // Einkauf planen
    Auftrag PAE=Auftrag(Auftrag::Anlegen(ppsInstanzID::_Garn__Einkauf),Kunde::default_id);
    AufEintragBase AEBPE=PAE.push_back(15,DATUM-60,ARTIKEL_KUPFER,OPEN,true);

    // Weberei planen
    Auftrag PA=Auftrag(Auftrag::Anlegen(ppsInstanzID::Weberei),Kunde::default_id);
    AufEintragBase AEBP=PA.push_back(400,DATUM-20,ARTIKEL_BANDLAGER,OPEN,true);
//    vergleichen(Check::Menge,"V2_Planung","Planung Weberei","");

    // Kunde bestellt
    Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
    AufEintragBase AEB3=auftrag.push_back(0,DATUM,ARTIKEL_BANDLAGER,OPEN,true);
    Auftrag auftrag2=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE2);
    AufEintragBase AEB4=auftrag2.push_back(100,DATUM,ARTIKEL_BANDLAGER,OPEN,true);
    AufEintrag(AEB3).MengeAendern(100);
    AufEintrag(AEB4).MengeAendern(100);
    AufEintrag(AEB3).MengeAendern(100);
//    vergleichen(Check::Menge,"V2_Ausgangspunkt","Ausgangspunkt","");

    // Weberei liefern    
    // erzeugt Inkonsistenz !!!
#if 0 // kann keinen Lieferschein schreiben ...
    Lieferschein liefs(ppsInstanzID::Weberei,cH_Kunde(KUNDE));
    LieferscheinEntryBase lsb(liefs,liefs.push_back(ARTIKEL_BANDLAGER,1,100));
    LieferscheinEntry(lsb).changeStatus(OPEN,false);
#else
    AufEintrag(AEBP).ProduziertNG(100,ProductionContext2());
#endif    

    if (Check::reparieren || Check::analyse)
    {  std::cerr << "V2 geht nur ohne -y/-r, die Inkonsistenz ist gewollt\n";
       return false;
    }
    vergleichen(Check::Menge,"V2_Auslieferung","Auslieferung","L");
    
    verf_vergleichen("V2_Auslieferung",AEB3);
    verf_vergleichen("V2_Auslieferung",AEB4);
    ben_vergleichen("V2_Auslieferung",AEBPE);
    ben_vergleichen("V2_Auslieferung",AufEintragBase(ppsInstanzID::_Garn__Einkauf,0,1));

    return true;
}

static TestReihe Verfuegbarkeit2_(&Verfuegbarkeit2,"Verfügbarkeit Grafik","V2");

#endif
