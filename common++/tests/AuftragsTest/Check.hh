// $Id: Check.hh,v 1.7 2002/07/05 12:35:02 christof Exp $
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
 public:
   enum e_check {Open,Menge_Plus,Menge_Minus,Menge_MinusMinus,StatusClosed,
                 Datumsaenderung,Rohwarenlager_einlagern,Bandlager_einlagern,
                 Rohwarenlager_auslagern,Bandlager_auslagern,
                 Planen_Kupfer,Planen_WebereiL,Planen_WebereiP,Planen_Faerberei_teil,Planen_Faerberei,
                 Split,Split_Rohwarenlager_einlagern,Split_Rohwarenlager_auslagern,
                 Planen_Faerberei_ueber,ZweiAuftraege_anlegen,ZweiterAuftrag_frueheresDatum,
                 Jumbo_richtig,Jumbo_falsch,Jumbo_doppelt,
                 Kunden_Teillieferung};
 private:
   void dump(e_check);
   void vergleich(e_check);
 public:
   Check(){}; 
   void teste(e_check check);

};
