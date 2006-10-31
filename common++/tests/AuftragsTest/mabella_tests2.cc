/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2006 Christof Petig
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

static bool LagerBuchungLieferschein()
{  

 Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE); 
 AufEintragBase AEB=auftrag.push_back(16,DATUM,ARTIKEL_TRIO,OPEN,true);
 auftrag.push_back(15,DATUM,ARTIKEL_ZWEI,OPEN,true);
 vergleichen(Check::Menge,"LBL","Ausgangspunkt","a");

   Lieferschein ls(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
   LieferscheinEntryBase lsb(ls,ls.push_back(AEB,ARTIKEL_TRIO,20,0,0));
   LieferscheinEntry le(lsb);
   le.setLagerid(1);
   le.changeStatus(OPEN,true);
   vergleichen(Check::Menge,"LBL_l","Lieferung","l");
   
   LieferscheinEntryBase lsb2(ls,ls.push_back(ARTIKEL_ZWEI,30,0,0));
   LieferscheinEntry le2(lsb2);
   le2.setLagerid(FERTIGWLAGER);
#if 0 // geht nicht ...   
   try
   { le2.changeStatus(OPEN,true);
     return false; // darf nicht sein 
   }
   catch (LagerError &e)
   { // so muss es sein ...
   }
#else
   Lager L(FERTIGWLAGER);
   L.rein_ins_lager(ARTIKEL_TRIO,15,false);
   L.rein_ins_lager(ARTIKEL_ZWEI,30,false);
   le2.changeStatus(OPEN,true);
#endif
   vergleichen(Check::Menge,"LBL_l2","Lieferung 2","");
  
   LieferscheinEntry::deleteEntry(le);
   LieferscheinEntry::deleteEntry(le2);
   vergleichen(Check::Menge,"LBL_d","Zurücknehmen","d");
   return true;
}

static TestReihe LagerBuchungLieferschein_(&LagerBuchungLieferschein,"Lager-Buchung per Lieferschein","LBL");

#endif
