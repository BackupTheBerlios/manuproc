// $Id: AuftragsVerwaltung.hh,v 1.6 2003/01/29 12:30:33 christof Exp $
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

#ifndef AVERWALTUNG_H
#define AVERWALTUNG_H

///////////////////////////////////////////////////////////////////////////
#include <Auftrag/AufEintragBase.h>

#define BESTELL_MENGE AuftragBase::mengen_t(400)
#define STATUS UNCOMMITED
#define STATUS_NEW OPEN
#define STATUS_NEW2 CLOSED

// Zum �ndern der Auftragsmenge:
#define NEW_BESTELLMENGE_MEHR AuftragBase::mengen_t(600)
#define NEW_BESTELLMENGE_WENIGER AuftragBase::mengen_t(300)
#define NEW_BESTELLMENGE_WENIGER_ALS_IM_LAGER AuftragBase::mengen_t(100)

class AuftragsVerwaltung
{
  public:
    AuftragsVerwaltung() {}

    AufEintragBase anlegen();
    AufEintragBase anlegen2();
    AufEintragBase anlegen3();
    AufEintragBase anlegenK();
    static void kunden_bestellmenge_aendern(AufEintrag &ae,AuftragBase::mengen_t menge);
};

#endif
