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
#include <Lager/Lager.h>
#include <Auftrag/Auftrag.h>
#include <Misc/Trace.h>
#include <unistd.h>
// Lieferschein
#include <Lieferschein/Lieferschein.h>
//#include <Misc/inbetween.h>
#include "TestReihe.h"

static bool ZweiKundenTest(AufEintrag &AE)
{      AuftragsVerwaltung  auftrag;
       AufEintragBase AEB2=auftrag.anlegenK();
       vergleichen(Check::Menge,"ZK_anlegen","Anlegen eines zweiten (offenen) Auftrags für einen anderen Kunden ["+AE.str()+"]","");

        AE.Produziert(300,Lieferschein::none_id);
      vergleichen(Check::Menge,"ZK_abschreiben1T","Zwei Kunden Teillieferung 1\n","");

      {AufEintrag AE(AEB2);
        AE.Produziert(180,Lieferschein::none_id);
      }
      vergleichen(Check::Menge,"ZK_abschreiben2T","Zwei Kunden Teillieferung 2\n","");

        AE.Produziert(200,Lieferschein::none_id);
      vergleichen(Check::Menge,"ZK_abschreiben1U","Zwei Kunden Volllieferung 1\n","");
       return true;
}

static TestReihe ZweiKundenTest_(&ZweiKundenTest,"Zwei Kunden","ZK");

