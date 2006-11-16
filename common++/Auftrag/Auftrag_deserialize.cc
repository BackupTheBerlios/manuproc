// $Id: Auftrag_deserialize.cc,v 1.3 2006/11/16 15:32:00 christof Exp $
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
#include <Misc/compose.hpp>
#include <Misc/create_parse.h>
#include <Misc/i18n.h>

ArtikelBase deserialize_Artikel(std::string& log, Tag const& a, bool anlegen=false)
{
}

cP_Waehrung deserialize_waehrung(std::string& log, Tag const& k)
{
}

cH_Kunde deserialize_kunde(std::string& log, Tag const& k, bool anlegen=false)
{ if (k.hasAttr("UnsereNummerBeiIhnen"))
  { int kdid=k.getAttr<int>("UnsereNummerBeiIhnen");
    if (ManuProC::create(kdid)!=k.getAttr<std::string>("UnsereNummerBeiIhnen"))
    { log+=String::compose(_("Merkwürdige Formatierung der Kundennummer '%1'\n"),k.getAttr<std::string>("UnsereNummerBeiIhnen"));
      return cH_Kunde(Handle<const Kunde>());
    }
    cH_Kunde res=cH_Kunde(kdid);
    Tag::const_iterator adresse=k.find(k.begin(),"Adresse");
    if (adresse!=k.end())
    { if (res->getName()!=adresse->getAttr<std::string>("Name"))
        log+=String::compose(_("Kundenname stimmt nicht überein: '%1' vs. '%2'\n"),
            res->getName(),adresse->getAttr<std::string>("Name"));
      Tag::const_iterator ort=adresse->find(adresse->begin(),"Ort");
      if (ort!=adresse->end() && res->ort()!=ort->Value())
        log+=String::compose(_("Kundenort stimmt nicht überein: '%1' vs. '%2'\n"),
            res->ort(),ort->Value());
      // usw ...
    }
    return res;
  }
  assert (!anlegen); // noch nicht fertig
  return cH_Kunde(Handle<const Kunde>());
}

AuftragBase deserialize_auftrag(std::string& log, Tag const& t, bool kunden_anlegen=false)
{ 
}
