/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2006 Jacek Jakubowski
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

#ifdef MABELLA_TEST
#include "AuftragsVerwaltung.hh"
#include "Check.hh"
#include <Lager/Lager.h>
#include <Auftrag/Auftrag.h>
#include <Misc/Trace.h>
#include <unistd.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>
#include "TestReihe.h"
#include <Auftrag/AufEintragBase.h>

static bool Auslieferung_ArtikelOhneLager()
{  
 Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE); 
 AufEintragBase AEB=auftrag.push_back(10,DATUM,ARTIKEL_TRAY,OPEN,true);
 vergleichen(Check::Menge,"AOL","Ausgangspunkt","a");

   Lieferschein ls(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
   LieferscheinEntryBase lsb(ls,ls.push_back(AEB,ARTIKEL_TRAY,1));
   LieferscheinEntry le(lsb);
   le.setLagerid(0);
   le.changeStatus(OPEN,true);
   vergleichen(Check::Menge,"AOL_l","Lieferung","l");

   return true;
}

static TestReihe Auslieferung_ArtikelOhneLager_(&Auslieferung_ArtikelOhneLager,"Artikel liefern ohne Lager","AOL");

#endif

