// $Id: Auftrag_serialize.cc,v 1.8 2006/08/03 11:17:44 christof Exp $
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

void serialize(Tag &where, cH_Kunde const& k,bool nest=true)
{ where.setAttr<int>("Nummer",k->Id());
  if (!k->idnr().empty()) 
    where.setAttr("ID_Nummer",k->idnr());
  Tag &Adresse=where.push_back("Adresse");
  Adresse.setAttr("Anrede",k->Anrede()->getAnrede());
  Adresse.setAttr("Name",k->firma());
  if (!k->postanwvor().empty())
    Adresse.push_back("Zeile",k->postanwvor());
  Adresse.push_back("Straße",k->strasse());
  if (!k->hausnr().empty())
    Adresse.push_back("Hausnummer",k->hausnr());
  if (!k->postanwnach().empty())
    Adresse.push_back("Zeile",k->postanwnach());
  if (!k->plz().empty())
    Adresse.push_back("PLZ",k->plz());
  Adresse.push_back("Ort",k->ort());
  Adresse.push_back("Land",k->land()->Name())
    .setAttr("Kurzbezeichnung",k->land()->LKZ());
  if (!k->postfach().empty())
  { Adresse.push_back("Postfach",k->postfach())
      .setAttr("PLZ",k->postfachplz());
  }
  if (k->Id()!=Kunde::eigene_id && !k->UnsereKundenNr().empty())
    where.setAttr("UnsereNummerBeiIhnen",k->UnsereKundenNr());
  // Kontakte (Telnr, Fax, E-Mail)
  // Betreuer (Name, Kontakt)
  if (nest && k->Rngan()>0 && k->Rngan()!=k->Id())
  { serialize(where.push_back("Rechnungsempfänger"),cH_Kunde(k->Rngan()),false);
  }
  if (nest && k->Lfran()>0 && k->Lfran()!=k->Id())
  { serialize(where.push_back("LieferungAn"),cH_Kunde(k->Lfran()),false);
  }
}

void serialize(Tag &dest, cH_ArtikelBezeichnung const& bez)
{ // Signifikanzen trennen?
  for (ExtBezSchema::const_iterator i=bez->getExtBezSchema()->begin();
      i!=bez->getExtBezSchema()->end();++i)
  { // bezkomptext ist meist kein gültiger XML-Identifier
    Tag &komp=dest.push_back("Komponente",(*bez)[i->bezkomptype]->getStrVal());
    komp.setAttr("Name",i->bezkomptext);
    komp.setAttr("Signifikanz",i->signifikanz);
  }
}

void serialize(Tag &dest, Preis const& pr, Einheit const& e)
{ dest.setAttr("Wert",pr.Wert().String());
  if (pr.BezugsMenge()!=1)
    dest.setAttr("Bezugsmenge",pr.BezugsMenge().String());
  if (e!=EinheitID::St_)
    dest.setAttr("Einheit",e.Bezeichnung());
  dest.setAttr("Währung",pr.getWaehrung()->Kurzbezeichnung());
}

void serialize(Tag &dest,AuftragFull const& a, bool bestaetigung)
{ dest.Type(bestaetigung?"Auftragsbestätigung":"Auftragserteilung");
  Tag &Auftraggeber=dest.push_back("Auftraggeber");
  cH_Kunde kunde(a.getKundennr());
  serialize(Auftraggeber,bestaetigung?kunde:cH_Kunde(Kunde::eigene_id));
  Tag &Lieferant=dest.push_back("Lieferant");
  serialize(Lieferant,bestaetigung?cH_Kunde(Kunde::eigene_id):kunde);
  dest.setAttr("unsereNummer",a.getAuftragidToStr());
  if (bestaetigung) 
  { dest.setAttr("IhreNummer",a.getYourAufNr());
    Tag &zahlungsart=dest.push_back("Zahlungsart",a.Zahlart()->Bezeichnung());
    zahlungsart.setAttr("Frist",a.Zahlart()->getZahlungsfrist());
    zahlungsart.setAttr("Kurzbezeichnung",a.Zahlart()->Kurzbezeichnung());
  }
  dest.setAttr("Kennung",AuftragBase::getLabel(a.Label()));
  if (!a.Notiz().empty()) dest.push_back("Notiz",a.Notiz());
  dest.push_back("Währung",a.getWaehrung()->Langbezeichnung())
    .setAttr("Kurzbezeichnung",a.getWaehrung()->Kurzbezeichnung());
  if (!!a.getAuftragsRabatt()) 
    dest.setAttr("Rabatt",a.getAuftragsRabatt().String());
  if (!a.getBemerkung().empty()) dest.push_back("Bemerkung",a.getBemerkung());
  dest.setAttr("Datum",a.getDatum().to_locale());
  
  for (AuftragFull::const_iterator i=a.begin();i!=a.end();++i)
  { Tag &zeile=dest.push_back("Zeile");
    zeile.setAttr("Nummer",i->ZNr());
    Tag &menge=zeile.push_back("Menge",i->getStueck().String());
    Einheit einh(i->Artikel());
    menge.setAttr("Einheit",einh.Bezeichnung());
    menge.setAttr("Termin",i->getLieferdatum().to_locale());
    { ArtikelTyp at(i->Artikel());
      Tag &warengruppe=zeile.push_back("Warengruppe",at.Bezeichnung());
      warengruppe.setAttr<int>("Id",at.Id());
    }
    cH_ArtikelBezeichnung unsArtB(i->Artikel())
        ,yourArtB(i->Artikel(),kunde->getSchemaId());
    if (!(unsArtB->getExtBezSchema()==yourArtB->getExtBezSchema()))
    { Tag &unsArt=zeile.push_back("UnserArtikel"); // Bezeichnung+Schema
      serialize(unsArt, unsArtB);
    }
    Tag &yourArt=zeile.push_back("IhrArtikel"); // B+S
    serialize(yourArt, yourArtB);
    Tag &spreis=zeile.push_back("Einzelpreis");
    serialize(spreis,i->EPreis(),einh);
    if (!!i->Rabatt()) spreis.setAttr("Rabatt",i->Rabatt().String());
    Tag &gpreis=zeile.push_back("Gesamtpreis");
    serialize(gpreis,i->GPreis(),Einheit(EinheitID::St_));
  }
  // Betrag, MwSt etc
}
