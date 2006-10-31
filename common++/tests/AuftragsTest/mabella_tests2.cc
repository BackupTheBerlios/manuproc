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
  FertigWaren fw(ARTIKEL_TRIO,FertigWaren::eInventur,100); 
  FertigWarenLager fwl(fw,1);
  fwl.Inventur();
  FertigWaren fw2(ARTIKEL_ZWEI,FertigWaren::eInventur,100); 
  FertigWarenLager fwl2(fw2,1);
  fwl2.Inventur();


 Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE); 
 AufEintragBase AEB=auftrag.push_back(16,DATUM,ARTIKEL_TRIO,OPEN,true);
 auftrag.push_back(15,DATUM,ARTIKEL_ZWEI,OPEN,true);
 vergleichen(Check::Menge,"LBL","Ausgangspunkt","a");

   Lieferschein ls(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE));
   LieferscheinEntryBase lsb(ls,ls.push_back(AEB,ARTIKEL_TRIO,20));
   LieferscheinEntry le(lsb);
   le.setLagerid(1);
   le.changeStatus(OPEN,true);
   vergleichen(Check::Menge,"LBL_l","Lieferung","l");
   
   LieferscheinEntryBase lsb2(ls,ls.push_back(ARTIKEL_ZWEI,30));
   LieferscheinEntry le2(lsb2);
//hmmm,ich denke Du meinst hier die Lagerid und nicht die Instanz
//   le2.setLagerid(FERTIGWLAGER);
   le2.setLagerid(1);

#if 0 // geht nicht ...   
   try
   { le2.changeStatus(OPEN,true);
     return false; // darf nicht sein 
   }
   catch (LagerError &e)
   { // so muss es sein ...
   }
#else
//   Lager L(FERTIGWLAGER);
//   L.rein_ins_lager(ARTIKEL_TRIO,15,false);
//   L.rein_ins_lager(ARTIKEL_ZWEI,30,false);
   le2.changeStatus(OPEN,true);
#endif
   vergleichen(Check::Menge|Check::Lieferschein|Check::FertigwLager
           ,"LBL_l2","Lieferung 2","");
  
   LieferscheinEntry::deleteEntry(le);
   LieferscheinEntry::deleteEntry(le2);
   vergleichen(Check::Menge|Check::Lieferschein|Check::FertigwLager
           ,"LBL_d","Zurücknehmen","d");

   return true;
}

static TestReihe LagerBuchungLieferschein_(&LagerBuchungLieferschein,"Lager-Buchung per Lieferschein","LBL");

static bool LagerBuchungRuecknahme()
{  
  FertigWaren fw(ARTIKEL_TRIO,FertigWaren::eInventur,10); 
  FertigWarenLager fwl(fw,1);
  fwl.Inventur();
  FertigWaren fw2(ARTIKEL_ZWEI,FertigWaren::eInventur,10); 
  FertigWarenLager fwl2(fw2,1);
  fwl2.Inventur();

  Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE); 
  AufEintragBase AEB=auftrag.push_back(16,DATUM,ARTIKEL_TRIO,OPEN,true);
  vergleichen(Check::Menge,"LBR","Ausgangspunkt","a");

// Freie EIntrag mit negativer Menge, Ruecknahmeschein
   Lieferschein rs(ppsInstanzID::Kundenauftraege,cH_Kunde(KUNDE),0,Lieferschein::LART_RUECK);
   LieferscheinEntryBase rsb(rs,rs.push_back(ARTIKEL_TRIO,-5));
   LieferscheinEntry re(rsb);
   re.setLagerid(1);
   re.changeStatus(OPEN,true);
   vergleichen(Check::Menge|Check::Lieferschein|Check::FertigwLager
           ,"LBR_r","Rücknahme","r");

   return true;
}

static TestReihe LagerBuchungRuecknahme_(&LagerBuchungRuecknahme,"Lager-Buchung per Rücknahmeschein","LBR");

#endif
