// $Id: datewin.cc,v 1.20 2005/10/30 00:58:22 christof Exp $
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

datewin::datewin(const std::string &inst) : block(false), 
	expandyear(true),instance(inst)
{  set_value(ManuProC::Datum::today());
   jahr->signal_activate().connect(activate.slot());
   gtk_signal_connect_after(GTK_OBJECT(gobj()), "grab_focus",
    		GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
   set_scrollable(false); // for now ...
   
   tag->signal_changed().connect(changed.slot());
   monat->signal_changed().connect(changed.slot());
   jahr->signal_changed().connect(changed.slot());
   kw_spinbutton->signal_changed().connect(changed.slot());
   jahr_spinbutton->signal_changed().connect(changed.slot());
}

ManuProC::Datum datewin::get_value() const throw()
{  ManuProC::Datum d;

   switch(get_current_page())
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
      case p_Kalender:
         {  guint y=0,m=0,day=0;
            calendar1->get_date(y,m,day);
            d=ManuProC::Datum(day,m+1,y,expandyear);
         }
         break;
      case p_leer: d=ManuProC::Datum(); break;
   }
//   const_cast<datewin*>(this)->set_value(d);
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
      calendar1->select_month(d.Monat()-1,d.Jahr());
      block=true;
      calendar1->select_day(d.Tag());
      block=false;
      int pg=load_settings();
      // Tag ist in Kalenderwochendarstellung nicht exakt darstellbar
      // (Informationsverlust) => Datum nehmen
      if (pg==p_Woche && d.Tag()!=ManuProC::Datum(d.KW()).Tag()) pg=p_Datum;
      set_current_page(pg);
   }
   else 
   {  block=true;
      set_current_page(p_leer);
      block=false;
   }
   changed();
}

gint datewin::try_grab_focus(GtkWidget *w,gpointer gp) throw()
{  datewin *_this=dynamic_cast<datewin*>((Glib::Object*)gp);
   assert(_this);
   switch(_this->get_current_page())
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
      case p_Kalender:
          _this->calendar1->grab_focus();
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
{  set_show_tabs(!s.empty());
   set_scrollable(!s.empty());
   if (!s.empty()) datum_label->set_text(s);
//   std::cout << "datewin::setLabel("<<s<<");\n";
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
{  if (!tag->get_value_as_int())
   {  set_value(ManuProC::Datum::today());
   }
   else set_current_page(load_settings());
}

// cache
datewin::PAGE datewin::defaultpage=datewin::p_leer;

void datewin::save_settings() const
{  int u=getuid();
   Global_Settings(u,instance,"datewin:page").set_Wert(itos(get_current_page()));
   defaultpage=PAGE(get_current_page());
}

int datewin::load_settings() const
{  if (defaultpage==p_leer)
      defaultpage=PAGE(atoi(Global_Settings(getuid(),instance,"datewin:page").get_Wert().c_str()));
   return int(defaultpage);
}

//void datewin::on_datewin_switch_page(Gtk::Notebook_Helpers::Page *p0, guint p1)
void datewin::on_datewin_switch_page(_GtkNotebookPage *p0, guint p1)
{  changed();
   if (p1==p_Kalender)
      calendar1->show();
   else calendar1->hide();
   if(p1==p_leer && !block)  // um das Datum auch zurücksetzen zu können
     activate();
}

void datewin::setInstance(const std::string &ins)
{instance=ins;}

