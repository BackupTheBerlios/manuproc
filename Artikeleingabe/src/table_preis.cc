// $Id: table_preis.cc,v 1.8 2005/10/24 11:40:24 christof Exp $
/*  Artikeleingabe: ManuProC's article management program
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#include "config.h"
#include "Artikeleingabe.hh"
#include <Artikel/Artikelpreis.h>
#include <Artikel/Einheiten.h>
#include <Aux/string0.h>
#include <Misc/FetchIStream.h>
#include <Misc/EntryValueFixed.h>

void table_preis::andere_Liste()
{ // Laden und Staffelung anzeigen 
  preis->set_value(0.0);
  waehrung->set_value(Waehrung::default_id);
  preismenge->set_value(1.0);
  mindestmenge->set_value(0);
  try {
    Artikelpreis ap(cH_PreisListe(preisliste->get_value()),artikelbox->get_value());
    preis->set_value(ap.Wert().as_float());
    waehrung->set_value(ap.getWaehrung());
    preismenge->set_value(ap.PreisMenge().as_float());
    mindestmenge->set_value(ap.MindMenge());
  } catch (...) {}
  preis->select_region(0,-1);
  preis->grab_focus();
}

void table_preis::preismenge_activate()
{ // preis_uebernehmen();
// aufraeumen();
  mindestmenge->select_region(0,-1);
  mindestmenge->grab_focus();
}

void table_preis::mindestmenge_activate()
{ preis_uebernehmen();
}

void table_preis::waehrung_deactivate()
{//  preis_uebernehmen();
   preismenge->select_region(0,preismenge->get_text_length());
   preismenge->grab_focus();
}

void table_preis::activate_preis()
{//  preis_uebernehmen();
   preismenge->select_region(0,preismenge->get_text_length());
   preismenge->grab_focus();
}

enum Spalten
{ SP_PRNUM, SP_PRNAM, SP_STAFFEL, SP_PRICE, SP_CURRENCY, SP_PER };

class PrRowData : public RowDataBase
{ PreisListe::ID preisliste;
  Preis pr;
  int mindestmenge;
public:
  static Einheit einheit;
  PrRowData(PreisListe::ID i, int mm, Preis p) : preisliste(i), pr(p), mindestmenge(mm)
  {}
  virtual const cH_EntryValue Value(guint _seqnr,gpointer gp) const
  { switch((Spalten)_seqnr)
    { case SP_PRNUM: return cH_EntryValueIntString(preisliste);
      case SP_PRNAM: return cH_EntryValueIntString(PreisListe(preisliste).Name());
      case SP_STAFFEL: return cH_EntryValueIntString(mindestmenge);
      case SP_PRICE: return cH_EntryValueFixed<2,double,long,false>(pr.Wert());
      case SP_CURRENCY: return cH_EntryValueIntString(pr.getWaehrung()->Kurzbezeichnung());
      case SP_PER: if (pr.BezugsMenge()==1) return cH_EntryValueIntString(einheit.Bezeichnung());
        else return cH_EntryValueIntString(pr.BezugsMenge().String(true,0,".",",")+einheit.Bezeichnung());
    }
    return cH_EntryValue();
  }
};

table_preis::table_preis(GlademmData *gmm_data)
        : table_preis_glade(gmm_data), artikelbox()
{ std::vector<std::string> cols;
  cols.push_back("Preisliste #");
  cols.push_back("Preisliste");
  cols.push_back("Mindestmenge");
  cols.push_back("Preis");
  cols.push_back("Währung");
  cols.push_back("per");
  preisstaffel->setTitles(cols);
  std::vector<float> align(cols.size());
  align[SP_PRICE]=1;
  align[SP_STAFFEL]=1;
  align[SP_PRNUM]=1;
  preisstaffel->setAlignment(align);
  preisstaffel->getModel().set_editable(SP_PRICE);
  preisstaffel->getModel().set_editable(SP_STAFFEL);
  preisstaffel->getModel().set_editable(SP_CURRENCY);
  preisstaffel->getModel().set_editable(SP_PER);
}

void table_preis::preis_uebernehmen()
{try {   
   preis->update();
   preismenge->update();
   mindestmenge->update();
   Artikelpreis::create_single(preisliste->get_value(),artikelbox->get_value(),
	Preis(preis->get_value(),waehrung->get_value(),
	    preismenge->get_value_as_int()),
	mindestmenge->get_value_as_int());
   Load();
 } catch (SQLerror &e)
 {  std::cerr << e <<'\n';
 }
}

void table_preis::staffelzeile_loeschen()
{
}

Einheit PrRowData::einheit=Einheit::null();

void table_preis::Load()
{ ArtikelBase art=artikelbox->get_value();
  std::vector<cH_RowDataBase> datavec;
  Query q("select kundennr,mindestmenge,preis,preismenge,waehrung "
      "from artikelpreise where artikelid=?");
  q << art;
  Query::Row r;
  while ((q >> r).good())
  { PreisListe::ID preisliste;
    Preis pr;
    int mindestmenge;
    r >> preisliste >> mindestmenge >> pr >> Query::check_eol();
    datavec.push_back(new PrRowData(preisliste,mindestmenge,pr));
  }
  preisstaffel->setDataVec(datavec);
  preisstaffel->set_remember("artikeleingabe","preisstaffel");
  PrRowData::einheit=Einheit(art);
  einheit->set_text(PrRowData::einheit.Bezeichnung());
  einheit2->set_text(PrRowData::einheit.Bezeichnung());
}
