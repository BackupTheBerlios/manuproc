// $Id: Auftrag_deserialize.cc,v 1.1 2006/11/16 15:31:38 christof Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) Christof Petig
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

#include <Misc/Tag.h>
#include <AuftragFull.h>
#include <Artikel/Einheiten.h>

ArtikelBase deserialize_Artikel(std::string& log, Tag const& a, bool anlegen=false)
{
}

cP_Waehrung deserialize_waehrung(std::string& log, Tag const& k)
{
}

cH_Kunde deserialize_kunde(std::string& log, Tag const& k, bool anlegen=false)
{ if (k.hasAttr("unsereNummer"))
  { if (create(k.getAttr<int>("unsereNummer"))!=k.getAttr<std::string>("unsereNummer"))
    { log+=String::Compose(_("Merkwürdige Formatierung der Kundennummer '%1'\n"),k.getAttr<std::string>("unsereNummer"));
      return cH_Kunde(Handle<const Kunde>());
    }
    cH_Kunde res=cH_Kunde(k.getAttr<int>("unsereNummer"));
    if (res->
    return res;
  }
  assert (!anlegen); // noch nicht fertig
  return cH_Kunde(Handle<const Kunde>());
}

AuftragBase deserialize_auftrag(std::string& log, Tag const& t, bool kunden_anlegen=false)
{ 
}
