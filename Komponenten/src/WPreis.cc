// $Id: WPreis.cc,v 1.7 2004/11/16 14:06:32 christof Exp $
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
#include <gtkmm/adjustment.h>
#include <gtk/gtksignal.h>

// strictly these are part of the ctor

gint WPreis::try_grab_focus(GtkWidget *w,gpointer gp)
{ 
   WPreis *this2((WPreis*)gp);
   assert(dynamic_cast<Gtk::Table*>(this2)); // very weak check
   this2->SP->grab_focus();
   this2->SP->select_region(0,-1);
   return true;
}

void WPreis::betrag()
{
  Gtk::Adjustment *SP_adj = manage(new class Gtk::Adjustment(0, 0, 10000, 1, 10, 10));
  SP=manage(new class Gtk::SpinButton(*SP_adj, 1, 
		ManuProC::Precision::GeldBetrag));
//  SP->set_usize(80,-1);
  SP->show();
  SP->signal_activate().connect(activate.slot());
  SP->signal_changed().connect(preis_changed.slot());
  attach(*SP,1,2,0,1);
}
void WPreis::waehrung()
{
  if (wwaehrung_bool)
   {
     WW=manage(new WWaehrung(WWaehrung::KURZ));
     WW->signal_activate().connect(activate.slot());
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
  LAp->set_justify(Gtk::JUSTIFY_RIGHT);
  LAp->show();
  attach(*LAp,0,1,1,2);
  Gtk::Adjustment *SP_adj = manage(new class Gtk::Adjustment(1, 0, 10000, 1, 10, 10));
  SP2=manage(new class Gtk::SpinButton(*SP_adj, 3, 0));
//  SP2->set_usize(80,-1);
  SP2->show();
  SP2->signal_activate().connect(activate.slot());
  attach(*SP2,1,2,1,2);
}

void WPreis::einheit()
{
  LA2=manage(new Gtk::Label(std::string(einh)));
  LA2->set_alignment(0, 0.5);
  LA2->show();
  attach(*LA2,2,3,1,2);
}

void WPreis::mindmenge()
{
 LabMindMenge=manage(new Gtk::Label("ab "+
		itos(mindestmenge)+" "+LA2->get_text()));
 LabMindMenge->set_justify(Gtk::JUSTIFY_RIGHT);
 LabMindMenge->set_alignment(0, 0.5);
 LabMindMenge->show();
 attach(*LabMindMenge,0,1,0,1);
}

WPreis::WPreis(bool wwaehrung)
: SP(), SP2(), LA(), LA2(), WW(), LabMindMenge(),
	mindestmenge(1), wwaehrung_bool(wwaehrung),
	einh(Einheit::default_id)
{
  betrag();
  waehrung();
  preismenge();
  einheit(); 
  mindmenge();
  set_Waehrung(Waehrung::default_id);
  gtk_signal_connect_after(GTK_OBJECT(gobj()), "grab_focus",GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
}

void WPreis::update()
{
  SP->update(); //gtk_spin_button_update(SP->gobj());
  SP2->update(); // gtk_spin_button_update(SP2->gobj());
}

void WPreis::reset()
{ set_value(Preis());
}

void WPreis::set_value(const Preis &p,int _mindmenge)
{ SP->set_value(p.Wert().as_float());
  SP2->set_value(p.BezugsMenge().as_float());
  set_Waehrung(p.getWaehrung());
  set_Mindmenge(_mindmenge);
}

void WPreis::set_value(const Artikelpreis &ap)
{ SP->set_value(ap.getPreis().Wert().as_float());
  SP2->set_value(ap.getPreis().BezugsMenge().as_float());
  set_Waehrung(ap.getPreis().getWaehrung());
  set_Mindmenge(ap.MindMenge());
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
  return Preis(SP->get_value(),getWaehrung(),SP2->get_value());
}
