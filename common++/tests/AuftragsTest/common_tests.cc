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

#include <ManuProCConfig.h>
#include "steuerprogramm.hh"

#include "AuftragsVerwaltung.hh"
#include "Check.hh"
#include <Auftrag/Auftrag.h>
#include <Lieferschein/Lieferschein.h>
#include "TestReihe.h"

static bool ZweiKundenTest(AufEintrag &AE)
{      AuftragsVerwaltung  auftrag;
       AufEintragBase AEB2=auftrag.anlegenK();
       vergleichen(Check::Menge,"ZK_anlegen","Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["+AE.str()+"]","");

        AE.ProduziertNG(300,LieferscheinEntryBase());
      vergleichen(Check::Menge,"ZK_abschreiben1T","Zwei Kunden Teillieferung 1\n","");

      {AufEintrag AE(AEB2);
        AE.ProduziertNG(180,LieferscheinEntryBase());
      }
      vergleichen(Check::Menge,"ZK_abschreiben2T","Zwei Kunden Teillieferung 2\n","");

        AE.ProduziertNG(200,LieferscheinEntryBase());
      vergleichen(Check::Menge,"ZK_abschreiben1U","Zwei Kunden Volllieferung 1\n","");
       return true;
}

static TestReihe ZweiKundenTest_(&ZweiKundenTest,"Zwei Kunden","ZK");

// es fehlen noch: Auftrag::Copy

static bool Funktionstest(AufEintrag &AE)
{{std::string uv("481186c6-4dc0-4dca-9309-650ce51749c4");
  { Auftrag a(AE);
    a.setBemerkung(uv);
  }
  { Auftrag a(AE);
    assert(a.getBemerkung()==uv);
 }}
 {std::string uv("634a8c8a-87d5");
  { Auftrag a(AE);
    a.setYourAufNr(uv);
  }
  { Auftrag a(AE);
    assert(a.getYourAufNr()==uv);
 }}
 {cP_Waehrung uv(Waehrung::ID(3));
  { Auftrag a(AE);
    a.setWaehrung(uv);
  }
  { Auftrag a(AE);
    assert(a.getWaehrung()==uv);
 }}
 {Auftrag::rabatt_t uv(4);
  { Auftrag a(AE);
    a.setRabatt(uv);
  }
  { Auftrag a(AE);
    assert(a.getAuftragsRabatt()==uv);
 }}
 {ManuProC::Datum uv(ManuProC::Datum::today()+10);
  { Auftrag a(AE);
    a.Zahlziel(uv);
  }
  { Auftrag a(AE);
    assert(a.Zahlziel()==uv);
 }}
 Query("insert into rechnung_zahlungsart (id,kurzbezeichnung) values (2,'test')");
 {cH_Zahlungsart uv(2);
  { Auftrag a(AE);
    a.Zahlart(uv);
  }
  { Auftrag a(AE);
    assert(a.Zahlart()==uv);
 }}
 {std::string uv("80b259b7-8900-4edb-9fbd-49459d506bcf");
  { Auftrag a(AE);
    a.Notiz(uv);
  }
  { Auftrag a(AE);
    assert(a.Notiz()==uv);
 }}
 {unsigned uv(2);
  { Auftrag a(AE);
    a.Label(uv);
  }
  { Auftrag a(AE);
    assert(a.Label()==uv);
 }}
#ifdef MABELLA_EXTENSIONS 
 {Kunde::ID uv(123);
  { Auftrag a(AE);
    a.setVerknr(uv);
  }
  { //Auftrag a(AE);
    assert((Query("select verknr from auftrag where (instanz,auftragid)=(?,?)")
          << AuftragBase(AE)).FetchOne<int>()==uv);
 }}
#endif 
  return true;
}

static TestReihe Funktionstest_(&Funktionstest,"korrekte Funktion","fun");
