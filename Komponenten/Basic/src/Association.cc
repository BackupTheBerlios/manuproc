// $Id: Association.cc,v 1.5 2004/07/16 06:58:43 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG
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

#include "Association.h"
#include "bool_CheckButton.hh"
#include "bool_CheckMenuItem.hh"
#include "int_SpinButton.hh"
#include "string_Entry.hh"
#include "string_Label.hh"
#include "int_ImageButton.hh"
#include <glibmm/quark.h>
#include <memory>

static std::auto_ptr<Glib::Quark> quark;
static const char * const quarkname = "ManuProC::Association";

static void attach(Glib::Object &widget, SigC::Object &connobj)
{  ManuProC::attach(widget,connobj,quark,quarkname);
}

static void destruct_object(void *obj)
{  if (obj) delete static_cast<SigC::Object*>(obj);
}

static SigC::Object *lookup(Glib::Object &widget)
{  if (!quark.get()) quark.reset(new Glib::Quark(quarkname));
   return static_cast<SigC::Object *>(widget.get_data(*quark));
}

void ManuProC::attach(Glib::Object &widget, SigC::Object &connobj, 
	std::auto_ptr<Glib::Quark> &q,const char * const qname)
{  if (!q.get()) q.reset(new Glib::Quark(qname));
   widget.set_data(*q,&connobj,&destruct_object);
}

template <class T,class W,class C>
 static typename C::Connection &Association_impl(W &w)
{  SigC::Object *r=0;
   if ((r=lookup(w))) 
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
      // since sigc::trackable is not polymorphic ...
      return *static_cast<typename C::Connection *>(r);
#else
      return *dynamic_cast<typename C::Connection *>(r);
#endif      
   typename C::Connection *r2=new typename C::Connection(&w);
   attach(w,*r2);
   return *r2;
}

ModelWidgetConnection<int,Gtk::SpinButton> &ManuProC::Association(Gtk::SpinButton &w)
{  return Association_impl<int,Gtk::SpinButton,int_SpinButton>(w);
}

ModelWidgetConnection<std::string,Gtk::Entry> &ManuProC::Association(Gtk::Entry &w)
{  return Association_impl<std::string,Gtk::Entry,string_Entry>(w);
}

ModelWidgetConnection<bool,Gtk::ToggleButton> &ManuProC::Association(Gtk::ToggleButton &w)
{  return Association_impl<bool,Gtk::ToggleButton,bool_CheckButton>(w);
}

ModelWidgetConnection<std::string,Gtk::Label> &ManuProC::Association(Gtk::Label &w)
{  return Association_impl<std::string,Gtk::Label,string_Label>(w);
}

ModelWidgetConnection<bool,Gtk::CheckMenuItem> &ManuProC::Association(Gtk::CheckMenuItem &w)
{  return Association_impl<bool,Gtk::CheckMenuItem,bool_CheckMenuItem>(w);
}

ModelWidgetConnection<int,Gtk::Image> &ManuProC::Association(Gtk::EventBox &e,Gtk::Image &i)
{  int_ImageButton::Connection &res
      =Association_impl<int,Gtk::Image,int_ImageButton>(i);
   res.set_widget(&i,&e);
   e.set_events(Gdk::BUTTON_PRESS_MASK); // really ?
   return res;
}
