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

#ifdef MANU_PROC_TEST
#include "AuftragsVerwaltung.hh"
#include "Check.hh"
#include <Lager/Lager.h>
#include <Auftrag/Auftrag.h>
#include <Misc/Trace.h>
#include <unistd.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>
//#include <Misc/inbetween.h>
#include "TestReihe.h"

static bool ManuProCTest(AufEintrag &AE)
{
      {// Planen des Einkaufs (Granulat, grün)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=3;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      assert(AEP.Artikel()==ARTIKEL_GRANULAT_GRUEN);
      AufEintrag PAE(AEP.Planen(UID,200,PA,PLANDATUM5));
      vergleichen(Check::Menge,"planen_granulat","Planen des Einkaufs(Granulat)","P");

      // Spezifischen Lieferschein schreiben
      Lieferschein liefs(EINKAUF,cH_Kunde(ManuProC::DefaultValues::EigeneKundenId));
      liefs.push_back(PAE, ARTIKEL_GRANULAT_GRUEN,1,200);
//unspezifisch   liefs.push_back(ARTIKEL_GRANULAT_GRUEN,1,200);
      vergleichen(Check::Menge|Check::Lieferschein,"LS_teillieferung","Lieferschein (mit AEB, Granulat) anlegen","L");
      }

      {// Planen des Einkaufs (Metall)
      Auftrag PA=Auftrag(Auftrag::Anlegen(EINKAUF),ManuProC::DefaultValues::EigeneKundenId);
      int znr=4;
      AufEintrag AEP((AufEintragBase(EINKAUF,AuftragBase::ungeplante_id,znr)));
      assert(AEP.Artikel()==ARTIKEL_METALL);
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
      assert(AEP.Artikel()==ARTIKEL_GRIFF_ROT);
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
