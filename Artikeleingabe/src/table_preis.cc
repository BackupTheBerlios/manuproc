// $Id: table_preis.cc,v 1.1 2005/05/06 00:11:53 christof Exp $
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

void table_preis::andere_Liste()
{ // Laden und Staffelung anzeigen 
#if 0
    preis->set_value(0.0);
    waehrung->set_value(Waehrung::default_id);
    preismenge->set_value(1.0);
    einheit->set_text("???");
   Einheit e(artikel->get_value());
   einheit->set_text((string)e);
   // bestellmenge
   Artikelpreis ap(cH_Kunde(kunde->get_value()),artikel->get_value());
std::cout << ap << '\n';
    preis->set_value(ap.Wert().as_float());
    waehrung->set_value(ap.getWaehrung());
    preismenge->set_value(ap.PreisMenge().as_float());
#endif
  preis->select_region(0,-1);
  preis->grab_focus();
}

void table_preis::preismenge_activate()
{ preis_uebernehmen();
// aufraeumen();
}

void table_preis::mindestmenge_activate()
{ 
}

void table_preis::waehrung_deactivate()
{  preis_uebernehmen();
}

void table_preis::activate_preis()
{  preis_uebernehmen();
   preismenge->select_region(0,preismenge->get_text_length());
   preismenge->grab_focus();
}

void table_preis::preis_uebernehmen()
{try {   
   preis->update();
   preismenge->update();
#if 0   
   Artikelpreis::insert_or_change(cH_Kunde(kunde->get_value())->Preislisten().back().second,
	artikel->get_value(),
	Preis(preis->get_value_as_float(),waehrung->get_value(),preismenge->get_value_as_int()));
#endif	
 } catch (SQLerror &e)
 {  std::cerr << e <<'\n';
 }
}

void table_preis::staffelzeile_loeschen()
{
}
