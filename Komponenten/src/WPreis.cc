// $Id: WPreis.cc,v 1.2 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#include "WPreis.hh"
//#include <Aux/Transaction.h>
//#include <Aux/SQLerror.h>
#include <gtk--/adjustment.h>

// strictly these are part of the ctor

void WPreis::betrag()
{
  Gtk::Adjustment *SP_adj = manage(new class Gtk::Adjustment(0, 0, 10000, 1, 10, 10));
  SP=manage(new class Gtk::SpinButton(*SP_adj, 1, 2));
  SP->show();
  SP->activate.connect(activate.slot());
  SP->changed.connect(preis_changed.slot());
  attach(*SP,1,2,0,1);
}
void WPreis::waehrung()
{
  if (wwaehrung_bool)
   {
     WW=manage(new WWaehrung(WWaehrung::KURZ));
     WW->activate.connect(activate.slot());
     attach(*WW,2,3,0,1);
   }
  else
   {
     LA=manage(new Gtk::Label("?"));
     LA->set_alignment(0, 0.5);
     LA->show();
     attach(*LA,2,3,0,1);
   }
}
void WPreis::preismenge()
{
  Gtk::Label *LAp=manage(new Gtk::Label("per"));
  LAp->set_justify(GTK_JUSTIFY_RIGHT);
  LAp->show();
  attach(*LAp,0,1,1,2);
  Gtk::Adjustment *SP_adj = manage(new class Gtk::Adjustment(1, 0, 10000, 1, 10, 10));
  SP2=manage(new class Gtk::SpinButton(*SP_adj, 3, 0));
  SP2->show();
  SP2->activate.connect(activate.slot());
  attach(*SP2,1,2,1,2);
}
void WPreis::einheit()
{
  LA2=manage(new Gtk::Label("kg"));
  LA2->set_alignment(0, 0.5);
  LA2->show();
  attach(*LA2,2,3,1,2);
}

WPreis::WPreis(bool wwaehrung)
: SP(0), SP2(0), LA(0), LA2(0), WW(0), wwaehrung_bool(wwaehrung)
{
  betrag();
  waehrung();
  preismenge();
  einheit(); 
// warum kann ich damit der Tabelle keine feste Breite geben, damit 
// damit sie unabh�ngig von der Anwendug immer gleichbreit bleibt?
// oder anders ausgedr�ckt: wie macht man das sonst?
// Antwort: Ein Problem des aufrufenden Widgets und der Child Parameter
  set_usize(50, -1);
  
  set_Waehrung(Waehrung::default_id);
//  show();
}

void WPreis::update()
{
  gtk_spin_button_update(SP->gtkobj());
  gtk_spin_button_update(SP2->gtkobj());
}

void WPreis::reset()
{ set_value(Preis());
}

void WPreis::set_value(const Preis &p)
{ SP->set_value(p.Wert());
  SP2->set_value(p.BezugsMenge());
  set_Waehrung(p.getWaehrung());
}

void WPreis::set_Waehrung(const cP_Waehrung &w) 
{ chwaehrung=w;
  if (wwaehrung_bool)  WW->set_value(w);
  else LA->set_text(w->Kurzbezeichnung());
}

cP_Waehrung WPreis::getWaehrung() const
{ return wwaehrung_bool?WW->get_value():chwaehrung;
}

const Preis WPreis::get_value() const
{ const_cast<WPreis*>(this)->update();
  return Preis(SP->get_value_as_float(),getWaehrung(),SP2->get_value_as_float());
}
