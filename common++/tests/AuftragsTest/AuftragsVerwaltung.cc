// $Id: AuftragsVerwaltung.cc,v 1.14 2003/01/29 12:30:33 christof Exp $
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
//#ifdef PETIG_EXTENSIONS

#include "steuerprogramm.hh"
#include "AuftragsVerwaltung.hh"
//#include <Misc/FetchIStream.h>
//#include <Misc/itos.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintrag.h>


AufEintragBase AuftragsVerwaltung::anlegen()
{
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
#ifdef MANU_PROC_TEST
   return auftrag.push_back(500,DATUM,ARTIKEL_SORTIMENT_BUNT,UNCOMMITED,UID,true);
#elif defined PETIG_TEST 
   return auftrag.push_back(400,DATUM,ARTIKEL_ROLLEREI,UNCOMMITED,UID,true);
#elif defined MABELLA_TEST 
   return auftrag.push_back(10,DATUM,ARTIKEL_TRIO,OPEN,UID,true);
#endif
}

void AuftragsVerwaltung::kunden_bestellmenge_aendern(AufEintrag &AE,AuftragBase::mengen_t menge)
{
   AuftragBase::mengen_t diffmenge=menge-AE.getStueck();
   AE.MengeAendern(UID,diffmenge,true,AufEintragBase(),ManuProC::Auftrag::r_Anlegen);
}

AufEintragBase AuftragsVerwaltung::anlegen2()
{
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
#ifdef MANU_PROC_TEST
   return auftrag.push_back(300,DATUM,ARTIKEL_SORTIMENT_BUNT,OPEN,UID,true);
#elif defined PETIG_TEST 
   return auftrag.push_back(300,DATUM,ARTIKEL_ROLLEREI,OPEN,UID,true);
#elif defined MABELLA_TEST 
assert(!"never get here");
   return auftrag.push_back(400,DATUM,ARTIKEL_TRIO,UNCOMMITED,UID,true);
#endif
}

AufEintragBase AuftragsVerwaltung::anlegen3()
{
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
#ifdef MANU_PROC_TEST
   return auftrag.push_back(300,DATUM9,ARTIKEL_SORTIMENT_BUNT,OPEN,UID,true);
#elif defined PETIG_TEST 
   return auftrag.push_back(300,DATUM9,ARTIKEL_ROLLEREI,OPEN,UID,true);
#elif defined MABELLA_TEST 
assert(!"never get here");
   return auftrag.push_back(400,DATUM,ARTIKEL_TRIO,UNCOMMITED,UID,true);
#endif
}

AufEintragBase AuftragsVerwaltung::anlegenK()
{
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE2);
#ifdef MANU_PROC_TEST
   return auftrag.push_back(200,NEWDATUM,ARTIKEL_SORTIMENT_BUNT,OPEN,UID,true);
#elif defined PETIG_TEST 
   return auftrag.push_back(200,NEWDATUM,ARTIKEL_ROLLEREI,OPEN,UID,true);
#elif defined MABELLA_TEST 
assert(!"never get here");
   return auftrag.push_back(400,DATUM,ARTIKEL_TRIO,UNCOMMITED,UID,true);
#endif
}

