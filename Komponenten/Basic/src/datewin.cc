// $Id: datewin.cc,v 1.2 2001/12/19 10:59:11 christof Exp $
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

#include "datewin.h"

datewin::datewin()
{  Petig::Datum heute=Petig::Datum::today();
   tag->set_value(heute.Tag());
   monat->set_value(heute.Monat());
   jahr->set_value(heute.Jahr());
   jahr->activate.connect(activate.slot());
   gtk_signal_connect(GTK_OBJECT(gtkobj()), "grab_focus",
    		GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
}

Petig::Datum datewin::get_value() const throw()
{
   gtk_spin_button_update(tag->gtkobj());
   gtk_spin_button_update(monat->gtkobj());
   gtk_spin_button_update(jahr->gtkobj());
   return Petig::Datum(tag->get_value_as_int(),monat->get_value_as_int()
		,jahr->get_value_as_int());
}

void datewin::set_value (const Petig::Datum &d) throw()
{
   tag->set_value (d.Tag());
   monat->set_value (d.Monat());
   jahr->set_value (d.Jahr());
}

gint datewin::try_grab_focus(GtkWidget *w,gpointer gp) throw()
{  assert(Gtk::HBox::isA((Gtk::Object *)gp)); // very weak check
   ((datewin*)gp)->tag->grab_focus();
   ((datewin*)gp)->tag->select_region(0,((datewin*)gp)->tag->get_text_length());
   return true;
}

void datewin::on_activate(int i)
{
  FELD feld=FELD(i);
  switch(feld) {
    case TAG:  
         { this->monat->grab_focus();
           this->monat->select_region(0,this->monat->get_text_length());
           break;
         }
    case MONAT:  
         { this->jahr->grab_focus();
           this->jahr->select_region(0,this->jahr->get_text_length());
         }
   }
}

