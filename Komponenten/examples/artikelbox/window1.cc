// $Id: window1.cc,v 1.11 2004/11/16 11:40:06 christof Exp $
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

#include "window1.hh"
#include <gtkmm/main.h>
#include <Artikel/ArtikelBezeichnung.h>

void window1::andererKunde()
{  artikelbox->setExtBezSchema(cH_Kunde(kundenbox->get_value())->getSchema(0));
}

void window1::on_Beenden_activate()
{  Gtk::Main::instance()->quit();
}

void window1::menu_cb(gpointer data)
{  cout << "menu_cb " << data << '\n';
}

window1::window1()
{  signal_destroy().connect(Gtk::Main::quit.slot());
   artikelbox->AddUserMenu("My User Menu 0",(gpointer)0);
   artikelbox->AddUserMenu("My User Menu 1",(gpointer)1);
   artikelbox->signal_MenueAusgewaehlt().connect(SigC::slot(*this,&window1::menu_cb));
   artikelbox->Einschraenken("exists (select true from webangaben where "
   		"artboxtable.id=webangaben.artikel)");
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
      cH_ArtikelBezeichnung AB(artikelbox->get_value().Id());
      std::cout << "Artikel "<<artikelbox->get_value().Id() <<'\t'<<AB->Bezeichnung() <<'\n';
    }   
   if (artikelbox2->get_value().Id())
    {
      cH_ArtikelBezeichnung AB(artikelbox2->get_value().Id());
      std::cout << "Artikel2 "<<artikelbox2->get_value().Id() <<'\t'<<AB->Bezeichnung() <<'\n';
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
   std::cout << "Preis "<< WPreis->get_value()<<"\n";
}
