// $Id: WPreis.hh,v 1.19 2004/11/16 11:47:51 christof Exp $
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

#ifndef _WPREIS_HH
#  define _WPREIS_HH

#include <gtkmm/table.h>
#include <Aux/Waehrung.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/label.h>
#include "WWaehrung.hh"
#include <Artikel/Preis.h>
#include <Artikel/Artikelpreis.h>
#include <Artikel/Einheiten.h>
#include <Aux/itos.h>

class WPreis : public Gtk::Table 
{
      Gtk::SpinButton *SP,*SP2;
      Gtk::Label *LA,*LA2;
      WWaehrung *WW;
      Gtk::Label *LabMindMenge;
      int mindestmenge;


      bool wwaehrung_bool;
      Einheit einh;	
      cP_Waehrung chwaehrung;

      // erzeugt die entsprechenden Unterwidgets
      void betrag();
      void waehrung();
      void preismenge();
      void einheit();
      void mindmenge();

	static gint try_grab_focus(GtkWidget *w,gpointer gp);

   public:
      void update();
      WPreis(bool wwaehrung=true);
      ~WPreis(){}

      SigC::Signal0<void> activate;
      SigC::Signal0<void> preis_changed;
      
      void set_value(const Preis &p,int _mindm=1);
      void set_value(const Artikelpreis &ap);
      const Preis get_value() const;
      void reset();
      
      void set_Einheit(const Einheit &e)
      { einh=e; 
	LA2->set_text(std::string(einh)); 
	set_Mindmenge(mindestmenge);
	}

// bitte nur mit gutem Grund verwenden
      void set_Waehrung(const cP_Waehrung &w);
      cP_Waehrung getWaehrung() const;

// veraltet
      void set_all(Preis::pfennig_cent_t d1, Preis::preismenge_t d2) // und Waehrung?
      {  set_value(Preis(d1,getWaehrung(),d2));
      }
      void set_Betrag(Preis::pfennig_cent_t d){SP->set_value(d.as_float());}
      void set_Preismenge(Preis::preismenge_t d){SP2->set_value(d.as_float());}
private: // echt zu alt
      void set_Einheit(const std::string s){LA2->set_text(s);}
public:
      void set_Mindmenge(int _mind)
		{mindestmenge=_mind;
		 LabMindMenge->set_text("ab "+
			itos(mindestmenge)+" "+LA2->get_text());
		}

      Preis::pfennig_cent_t get_Betrag() const 
      {  return get_value().Wert(); }
      Preis::preismenge_t get_Preismenge() const 
      {  return get_value().BezugsMenge(); }
      WaehrungID::enum_t get_Waehrung_enum() const 
      {  return getWaehrung()->Id();}
      const Preis get_Preis() const 
      {  return get_value(); }
      int Mindmenge() const { return mindestmenge; }
};
#endif
