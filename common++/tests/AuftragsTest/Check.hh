
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


// Die folgende Zeile bzw. überschreibt die Sollzustände der Datenbank 
// durch den Istzustand. Bitte nur verwenden, wenn sie genau wissen, 
// was sie tun
//#define CREATE_TEST_DATABASE  true


///////////////////////////////////////////////////////////////////////////
// Do not change anything below this line /////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include <Artikel/ArtikelBaum.h>
#include <Artikel/Prozess.h>
#include <Lager/LagerPlatz.hh>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>

class Check
{
   enum was_checken_p { b_Jumbo, b_Lieferschein, b_Menge };
 public:
   enum was_checken { Nix,
   		Jumbo=1<<b_Jumbo,
   		Lieferschein=1<<b_Lieferschein,
   		Menge=1<<b_Menge };
   		
   enum e_check {Open,Menge_Plus,Menge_Minus,Menge_MinusMinus,StatusClosed,
                 Datumsaenderung,Rohwarenlager_einlagern,Bandlager_einlagern,
                 Rohwarenlager_auslagern,
                 Planen_Kupfer,Planen_WebereiL,Planen_WebereiP,Planen_Faerberei_teil,
                 LieferscheinEinkaufTeillieferung,
                 Split,Split_Rohwarenlager_einlagern,Split_Rohwarenlager_auslagern,
                 Planen_Faerberei_ueber,ZweiAuftraege_anlegen,
                 ZweiterAuftrag_frueheresDatum,ZweiterAuftrag_frueheresDatum_abschreiben,
                 ZweiterAuftrag_frueheresDatum_closed,ErsterAuftrag_frueheresDatum_closed,Planen_WebereiD,
                 Jumbo_richtig,Jumbo_falsch,Jumbo_doppelt,
                 Kunden_Teillieferung,Kunden_Ueberlieferung,
                 LieferscheinTeil,LieferscheinZeileLoeschen,LieferscheinZeileLoeschen_n,LieferscheinVoll,
                 LieferscheinMengenaenderungPlus,LieferscheinMengenaenderungMinus,
                 LieferscheinZusatz,LieferscheinZusatzPlus,LieferscheinZusatzMinus,
                 LieferscheinZusatzMinusKunde,LieferscheinZweiAufTeil,
                 LieferscheinZweiAufVoll,LieferscheinJacek0,ProduktionsPlanungWeberei,
                 ProduktionsPlanungEinkauf,
                 ZweiKundenTest_anlegen,ZweiKunden_Teil1,ZweiKunden_Teil2,
                 ZweiKunden_Ueber1,ZweiKundenMengeFuer
                     };
 private:
   static was_checken WasChecken(e_check);
   static std::string Zusatz(e_check);

   void dump(was_checken);
   bool vergleich(was_checken,const std::string &name);
 public:
   Check(){}; 
   // old variant
   bool teste(e_check check,bool mit_reparatur_programm,bool vor_dem_test_reparieren=false); 
   // new variant (needs no enum) CP
   // Erfolgreich: true; sonst false
   bool teste(was_checken was, const std::string &name,bool mit_reparatur_programm,bool vor_dem_test_reparieren=false);
};

Check::was_checken operator|(Check::was_checken a, Check::was_checken b);
bool operator&(Check::was_checken a, Check::was_checken b);
