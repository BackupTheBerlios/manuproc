// $Id: datewin.cc,v 1.24 2005/12/21 07:24:36 christof Exp $
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
#include <Misc/itos.h>
#include <Misc/Global_Settings.h>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <gtk/gtksignal.h>
#include "datewin_popup.hh"

datewin::datewin() : // const std::string &inst) : block(false), 
	expandyear(true),kw_bevorzugen(), popup()
{  set_value(ManuProC::Datum::today());
   jahr->signal_activate().connect(activate.slot());
   gtk_signal_connect_after(GTK_OBJECT(gobj()), "grab_focus",
    		GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
   tag->signal_changed().connect(changed.slot());
   monat->signal_changed().connect(changed.slot());
   jahr->signal_changed().connect(changed.slot());
   kw_spinbutton->signal_changed().connect(changed.slot());
   jahr_spinbutton->signal_changed().connect(changed.slot());
}

ManuProC::Datum datewin::get_value() const throw()
{  ManuProC::Datum d;

   switch(notebook->get_current_page())
   {  case p_Datum:
         tag->update();
         monat->update();
         jahr->update();
         d=ManuProC::Datum(tag->get_value_as_int(),monat->get_value_as_int()
		,jahr->get_value_as_int(),expandyear);
         break;
      case p_Woche:
         kw_spinbutton->update();
         jahr_spinbutton->update();
         d=ManuProC::Datum(Kalenderwoche(kw_spinbutton->get_value_as_int(),
         	jahr_spinbutton->get_value_as_int()));
         break;
      case p_leer: d=ManuProC::Datum(); break;
   }
   return d;
}

void datewin::set_value (const ManuProC::Datum &d) throw()
{
   if (d.valid())
   {  tag->set_value (d.Tag());
      monat->set_value (d.Monat());
      jahr->set_value (d.Jahr());
      kw_spinbutton->set_value(d.KW().Woche());
      jahr_spinbutton->set_value(d.KW().Jahr());
      int pg=kw_bevorzugen?p_Woche:p_Datum;
      if (pg==p_Woche && d.Tag()!=ManuProC::Datum(d.KW()).Tag()) pg=p_Datum;
      notebook->set_current_page(pg);
   }
   else 
   {  notebook->set_current_page(p_leer);
   }
   changed();
}

gint datewin::try_grab_focus(GtkWidget *w,gpointer gp) throw()
{  datewin *_this=dynamic_cast<datewin*>((Glib::Object*)gp);
   assert(_this);
   switch(_this->notebook->get_current_page())
   {  case p_Datum:
   	  _this->jahr->select_region(0,_this->jahr->get_text_length());
   	  _this->monat->select_region(0,_this->monat->get_text_length());
   	  _this->tag->select_region(0,_this->tag->get_text_length());
	  _this->tag->grab_focus();
   	  break;
      case p_Woche:
   	  _this->jahr_spinbutton->select_region(0,_this->jahr_spinbutton->get_text_length());
   	  _this->kw_spinbutton->select_region(0,_this->kw_spinbutton->get_text_length());
          _this->kw_spinbutton->grab_focus();
   	  break;
      case p_leer:
	  break;
   }
   return true;
}

void datewin::on_tag_activate()
{
 this->monat->grab_focus();
 this->monat->select_region(0,this->monat->get_text_length());
}

void datewin::on_monat_activate()
{
 this->jahr->grab_focus();
 this->jahr->select_region(0,this->jahr->get_text_length());
}


void datewin::setLabel(const std::string &s)
{  datum_label->set_text(s);
}

void datewin::datum_activate()
{  set_value(get_value());
   activate();
}
void datewin::kw_activate()
{  set_value(get_value());
   activate();
}

// display datewin_popup
void datewin::on_togglebutton_menu_toggled()
{ if (popup || !togglebutton_menu->get_active()) { delete popup; popup=0; }
  else popup=new datewin_popup(this);
}
