// $Id: datewin.cc,v 1.4 2002/04/30 08:12:41 christof Exp $
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
#include <Aux/itos.h>
#include <Aux/Global_Settings.h>
#include <unistd.h>

datewin::datewin() : block(false)
{  set_value(Petig::Datum::today());
   jahr->activate.connect(activate.slot());
   gtk_signal_connect(GTK_OBJECT(gtkobj()), "grab_focus",
    		GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
   set_scrollable(false); // for now ...
}

Petig::Datum datewin::get_value() const throw()
{  Petig::Datum d;

   switch(get_current_page_num())
   {  case p_Datum:
         tag->update();
         monat->update();
         jahr->update();
         d=Petig::Datum(tag->get_value_as_int(),monat->get_value_as_int()
		,jahr->get_value_as_int());
         break;
      case p_Woche:
         kw_spinbutton->update();
         jahr_spinbutton->update();
         d=Petig::Datum(Kalenderwoche(kw_spinbutton->get_value_as_int(),
         	jahr_spinbutton->get_value_as_int()));
         break;
      case p_Kalender:
         {  guint y=0,m=0,day=0;
            calendar1->get_date(&y,&m,&day);
            d=Petig::Datum(day,m,y);
         }
         break;
      case p_leer: break;
   }
//   const_cast<datewin*>(this)->set_value(d);
   return d;
}

void datewin::set_value (const Petig::Datum &d) throw()
{
   if (d.valid())
   {  tag->set_value (d.Tag());
      monat->set_value (d.Monat());
      jahr->set_value (d.Jahr());
      kw_spinbutton->set_value(d.KW().Woche());
      jahr_spinbutton->set_value(d.KW().Jahr());
      calendar1->select_month(d.Monat(),d.Jahr());
      block=true;
      calendar1->select_day(d.Tag());
      block=false;
      set_page(load_settings());
   }
   else 
   {  set_page(p_leer);
   }
}

gint datewin::try_grab_focus(GtkWidget *w,gpointer gp) throw()
{  assert(Gtk::Notebook::isA((Gtk::Object *)gp)); // very weak check
   datewin *_this=static_cast<datewin*>(gp);
   switch(_this->get_current_page_num())
   {  case p_Datum:
	  _this->tag->grab_focus();
   	  _this->tag->select_region(0,_this->tag->get_text_length());
   	  _this->monat->select_region(0,_this->monat->get_text_length());
   	  _this->jahr->select_region(0,_this->jahr->get_text_length());
   	  break;
      case p_Woche:
          _this->kw_spinbutton->grab_focus();
   	  _this->kw_spinbutton->select_region(0,_this->kw_spinbutton->get_text_length());
   	  _this->jahr_spinbutton->select_region(0,_this->jahr_spinbutton->get_text_length());
   	  break;
      case p_Kalender:
          _this->calendar1->grab_focus();
          break;
      case p_leer:
	  break;
   }
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

void datewin::setLabel(const std::string &s)
{  set_show_tabs(!s.empty());
   set_scrollable(!s.empty());
   if (!s.empty()) datum_label->set_text(s);
   std::cout << "datewin::setLabel("<<s<<");\n";
}

void datewin::datum_activate()
{  save_settings();
   set_value(get_value());
   activate();
}
void datewin::kw_activate()
{  save_settings();
   set_value(get_value());
   activate();
}
void datewin::on_day_selected()
{  if (block) return;
   save_settings();
   set_value(get_value());
   activate();
}
void datewin::datum_setzen()
{  set_page(load_settings());
}

void datewin::save_settings() const
{  int u=getuid();
   Global_Settings(u,"","datewin:page").set_Wert(itos(get_current_page_num()));
}

int datewin::load_settings() const
{  return atoi(Global_Settings(getuid(),"","datewin:page").get_Wert().c_str());
}

void datewin::on_datewin_switch_page(Gtk::Notebook_Helpers::Page *p0, guint p1)
{  if (p1==p_Kalender)
      calendar1->show();
   else calendar1->hide();
}
