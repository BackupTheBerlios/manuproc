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

static bool Auftrag_Kunde(AufEintrag &AE)
{
 Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE); 

 vergleichen(Check::Menge,"auftrag_anlegen","Kundenauftrag anlegen","AM");
	
 return true;
}

static TestReihe Lieferschein_Kunde_(&Auftrag_Kunde,"Kundenauftrag (Mabella)","AM");


static bool Rollereiplanung()
{  
       Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
       AufEintragBase AEB2=auftrag.push_back(40,DATUM,ARTIKEL_TRIO,OPEN,true);
       vergleichen(Check::Menge,"roll_Ausgangspunkt","Ausgangspunkt","a");
       
       AufEintragBase sourceb(ppsInstanzID::Einkauf,AuftragBase::ungeplante_id,1);
       AufEintrag source(sourceb);
       assert(source.Artikel()==ARTIKEL_TRIO);
       AuftragBase dest(ppsInstanzID::Rollerei,AuftragBase::ungeplante_id);
       source.Planen(20,dest,source.getLieferdatum());
       vergleichen(Check::Menge,"roll_Plan","Hälfte selbst weben","b");
       
       return true;
}

static TestReihe Rollereiplanung_(&Rollereiplanung,"Rollereiplanung","roll");

#include <Artikel/ArtikelStamm.h>

static bool MindestMenge()
{  Lager L(FERTIGWLAGER);
   L.rein_ins_lager(ARTIKEL_TRIO,5,false);
   ArtikelStamm(make_value(ArtikelBase(ARTIKEL_TRIO))).setMindBest(5);
   vergleichen(Check::Menge,"minmen_Ausgangspunkt","Ausgangspunkt","a");
   
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
   AufEintragBase AEB2=auftrag.push_back(4,DATUM,ARTIKEL_TRIO,OPEN,true);
   vergleichen(Check::Menge,"minmen_Bestellung","Bestellung","b");   
   
   AufEintragBase AEB3=auftrag.push_back(4,DATUM9,ARTIKEL_TRIO,OPEN,true);
   vergleichen(Check::Menge,"minmen_Best2","Bestellung 2","b2");
   
   AufEintrag(AEB2).changeStatus(STORNO);
   vergleichen(Check::Menge,"minmen_storno","Storno","s");   
   
   Lieferschein liefs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
   LieferscheinEntryBase lsb(liefs,liefs.push_back(ARTIKEL_TRIO,5));
   LieferscheinEntry(lsb).changeStatus(OPEN,false);
   vergleichen(Check::Lieferschein|Check::Menge,"minmen_Auslief","Lieferung","l");

   return true;
}

static TestReihe MindestMenge_(&MindestMenge,"Mindestmenge","minMen");
#endif
