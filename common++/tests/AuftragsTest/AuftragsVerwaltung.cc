// $Id: AuftragsVerwaltung.cc,v 1.4 2002/09/02 13:04:04 christof Exp $
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
//#include <Aux/FetchIStream.h>
//#include <Aux/itos.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintrag.h>

void Problems(void *,AufEintrag::st_problems e)
{
assert(!"never get here\n");
  switch (e.art) {
     case AufEintrag::Geplant :
        cout << "WARNUNG: es werden "<<e.menge_input<<" in Auftrag "<<e.AEB
             << " geändert, aber es sind nur noch "<<e.menge_output
             << " nicht verplant\n\n";
        break;          
     case AufEintrag::Geliefert :
        cout << "WARNUNG: Auftrag "<<e.AEB<<" ist schon ausgeliefert worden, reduzieren nicht möglich\n";
        break;
     case AufEintrag::GeliefertFatal :
        cout << "ACHTUNG: Zuviel ausgeliefert, vollständige Änderung nicht möglich:\n"
             << "Auftrag "<<e.AEB<<"\tgewünschte Menge: "<<e.menge_input<<
             "\tgeänderte Menge: "<<e.menge_output<<'\n';
        break;
     case AufEintrag::Lager :
        cout << "Im Lager ist von Artikel "<<AufEintrag(e.AEB).ArtId()<<' '
             <<cH_ArtikelBezeichnung(AufEintrag(e.AEB).ArtId())->Bezeichnung()
             << " "<<abs(e.menge_input-e.menge_output)<<" freigeworden\n";
         break;
     case AufEintrag::Geplant_nolager :
       {
         AufEintrag AE(e.AEB);
         cout << "Artikel "<<cH_ArtikelBezeichnung(AE.ArtId())->Bezeichnung()
             << " ist in Instanz "<<e.AEB.Instanz()->Name()
             << " am "<<AE.LastEditDate().c_str() <<" geplant worden"
             << " Mengenreduzierung nicht möglich.\n";
         break;
       }
      default :  assert(!"never get here");
   }
}


AufEintragBase AuftragsVerwaltung::anlegen()
{
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
   int znr=auftrag.insertNewEntry(400,DATUM,ARTIKEL_ROLLEREI,UNCOMMITED,UID,true);
   return AufEintragBase(auftrag,znr);
}

void AuftragsVerwaltung::kunden_bestellmenge_aendern(AufEintragBase aeb,AuftragBase::mengen_t menge)
{
   AufEintrag AE(aeb);
   AuftragBase::mengen_t diffmenge=menge-AE.getStueck();
   AufEintrag(aeb).updateStkDiff__(UID,diffmenge,true,AufEintragBase::r_Standard);
}

AufEintragBase AuftragsVerwaltung::anlegen2()
{
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
   int znr=auftrag.insertNewEntry(300,DATUM,ARTIKEL_ROLLEREI,OPEN,UID,true);
   return AufEintragBase(auftrag,znr);
}

AufEintragBase AuftragsVerwaltung::anlegen3()
{
   Auftrag auftrag=Auftrag(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),KUNDE);
   int znr=auftrag.insertNewEntry(300,DATUM9,ARTIKEL_ROLLEREI,OPEN,UID,true);
   return AufEintragBase(auftrag,znr);
}

//#endif
