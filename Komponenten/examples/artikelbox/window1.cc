// $Id: window1.cc,v 1.6 2001/09/19 15:00:04 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
// generated 1999/2/1 11:32:33 MET by christof@petig.petig.de
// using glademm V0.2.6
//
// newer (non customized) versions of this file go to window1.cc_glade

// This file is for your program, I won't touch it again!

#include "window1.hh"
#include <gtk--/main.h>
#include <Artikel/ArtikelBezeichnung.h>

void window1::andererKunde()
{  artikelbox->setExtBezSchema(cH_Kunde(kundenbox->get_value())->getSchema());
}

void window1::on_Beenden_activate()
{  Gtk::Main::instance()->quit();
}

window1::window1()
{  destroy.connect(Gtk::Main::quit.slot());
}

void window1::on_reset()
{  artikelbox->reset();
   artikelbox->grab_focus();
}

void window1::on_show_clicked()
{  std::cout << "Datum "<<datum->get_value() << '\n';
   std::cout << "Prozess " << prozess->get_value()->Id() << '\n';
   if (artikelbox->get_value().Id())
    {
      ArtikelBezeichnung AB(artikelbox->get_value().Id());
      std::cout << "Artikel "<<artikelbox->get_value().Id() <<'\t'<<AB.Bezeichnung() <<'\n';
    }   
   std::cout << "Kunde " << kundenbox->get_value() << '\n';
//   std::cout << "Waehrung "<< WWaehrung->Langbezeichnung()<<"\t"<<WWaehrung->Kurzbezeichnung()<<"\n";
   std::cout << "Preis " << WPreis->get_Betrag()<<"\n";
//WWaehrung->set_History(Waehrung::USD);
//WPreis->set_Waehrung(Waehrung::USD);
}

void window1::on_show_complete()
{ std::cout << artikelbox->get_value().Id() << '\n';
}

	
void window1::on_activate_wwaehrung()
{
   std::cout << "Waehrung "<< WWaehrung->Langbezeichnung()<<"\t"<<WWaehrung->Kurzbezeichnung()<<"\n";
   std::cout << "Enum "<<WWaehrung->get_enum()<<"\n";
}

void window1::on_activate_wpreis()
{
  WPreis->update();
   std::cout << "Preis "<< WPreis->get_Betrag()<<"\n";//<<WWaehrung->Kurzbezeichnung()<<"\n";
}
