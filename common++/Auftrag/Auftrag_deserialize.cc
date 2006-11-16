// $Id: Auftrag_deserialize.cc,v 1.4 2006/11/16 15:32:12 christof Exp $
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
#include <Misc/Date.h>

ManuProC::Date deserialize_Datum(std::string& log, std::string const& a)
{ return ManuProC::Date(a.c_str());
}

ArtikelBase deserialize_Artikel(std::string& log, Tag const& a, bool anlegen=false)
{
}

cP_Waehrung deserialize_waehrung(std::string& log, Tag const& k)
{ assert(k.getAttr("Kurzbezeichnung")=="EUR" && k.Value()=="Euro");
  return cP_Waehrung(WaehrungID::EUR);
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

static Tag const& get_tag(Tag const& parent, std::string const& name)
{ Tag::const_iterator i= parent.find(parent.begin(),name);
  assert(i!=parent.end());
  return *i;
}

static Preis deserialize_Preis(std::string& log, Tag const& t)
{ // @@
  return Preis();
}

AuftragBase deserialize_auftrag(std::string& log, Tag const& t, bool kunden_anlegen=false)
{ bool erteilung= t.Type()=="Auftragserteilung";
  if (erteilung)
  { Tag::const_iterator auftraggeber=t.find(t.begin(),"Auftraggeber");
    assert(auftraggeber!=t.end());
    cH_Kunde k_auftrag=deserialize_kunde(log,*auftraggeber,kunden_anlegen);
    if (!k_auftrag) return AuftragBase();
    assert(t.hasAttr("unsereNummer"));
    if (Auftrag::getIdFromYourAufId(ppsInstanzID::Kundenauftraege,
              t.getAttr<std::string>("unsereNummer"),
              k_auftrag->Id())!=Auftrag::none_id)
    { log+=String::compose(_("Es existiert bereits ein Auftrag mit Nummer '%1'\n"),
            t.getAttr<std::string>("unsereNummer"));
      return AuftragBase();
    }
    Auftrag a(Auftrag::Anlegen(ppsInstanzID::Kundenauftraege),k_auftrag->Id(),
          t.getAttr<std::string>("unsereNummer"),UNCOMMITED);
    assert(deserialize_waehrung(log,get_tag(t,"Währung"))->Id()==WaehrungID::EUR);
    a.Datum(deserialize_Datum(log, t.getAttr("Datum")));
    FOR_EACH_CONST_TAG_OF(i,t,"Zeile")
    { Tag const& menge=get_tag(*i, "Menge");
      Auftrag::mengen_t bestellt(menge.Value());
      ManuProC::Date lieferdatum=deserialize_Datum(log,menge.getAttr<std::string>("Termin"));
      ArtikelBase artikel=deserialize_Artikel(log,get_tag(*i,"IhrArtikel"));
      Preis preis=deserialize_Preis(log,get_tag(*i,"Einzelpreis"));
      Auftrag::rabatt_t rabatt;
      
      a.push_back(bestellt, lieferdatum, artikel, UNCOMMITED, true, preis, rabatt);
    }
    return a;
  }
  return AuftragBase();
}
