// $Id: Association.cc,v 1.1 2003/05/09 20:56:00 christof Exp $
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
#include <glibmm/quark.h>
#include <memory>

static std::auto_ptr<Glib::Quark> quark;

static const char * const quarkname = "ManuProC::Association";

static void destruct_object(void *obj)
{  if (obj) delete static_cast<SigC::Object*>(obj);
}

static SigC::Object *lookup(Glib::Object &widget)
{  if (!quark.get()) quark.reset(new Glib::Quark(quarkname));
   return static_cast<SigC::Object *>(widget.get_data(*quark));
}

static void attach(Glib::Object &widget, SigC::Object &connobj)
{  if (!quark.get()) quark.reset(new Glib::Quark(quarkname));
   widget.set_data(*quark,&connobj,&destruct_object);
}

template <class T,class W,class C>
 static typename C::Connection::this_t &Association_impl(W &w)
{  SigC::Object *r=0;
   if ((r=lookup(w))) return *dynamic_cast<typename C::Connection::this_t *>(r);
   typename C::Connection::this_t *r2=new typename C::Connection(&w);
   attach(w,*r2);
   return *r2;
}

ModelWidgetConnection<int,Gtk::SpinButton> &Association(Gtk::SpinButton &w)
{  return Association_impl<int,Gtk::SpinButton,int_SpinButton>(w);
}

ModelWidgetConnection<std::string,Gtk::Entry> &Association(Gtk::Entry &w)
{  return Association_impl<std::string,Gtk::Entry,string_Entry>(w);
}

ModelWidgetConnection<bool,Gtk::ToggleButton> &Association(Gtk::ToggleButton &w)
{  return Association_impl<bool,Gtk::ToggleButton,bool_CheckButton>(w);
}

ModelWidgetConnection<std::string,Gtk::Label> &Association(Gtk::Label &w)
{  return Association_impl<std::string,Gtk::Label,string_Label>(w);
}

ModelWidgetConnection<bool,Gtk::CheckMenuItem> &Association(Gtk::CheckMenuItem &w)
{  return Association_impl<bool,Gtk::CheckMenuItem,bool_CheckMenuItem>(w);
}
