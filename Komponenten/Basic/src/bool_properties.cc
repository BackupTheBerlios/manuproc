// $Id: bool_properties.cc,v 1.2 2004/05/03 13:44:36 christof Exp $
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

#include "bool_properties.hh"
#include <gtkmm/widget.h>
#include <sigc++/bind.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

static void visibility_cb(gpointer gp,Model_ref<bool> model,Gtk::Widget *w)
{  if (model.matches(gp))
   {  if (model.Value()) w->show();
      else w->hide();
   }
}

static void visibility_cb_inv(gpointer gp,Model_ref<bool> model,Gtk::Widget *w)
{  if (model.matches(gp))
   {  if (model.Value()) w->hide();
      else w->show();
   }
}

static void sensitivity_cb(gpointer gp,Model_ref<bool> model,Gtk::Widget *w)
{  if (model.matches(gp)) w->set_sensitive(model.Value());
}

static void sensitivity_cb_inv(gpointer gp,Model_ref<bool> model,Gtk::Widget *w)
{  if (model.matches(gp)) w->set_sensitive(!model.Value());
}

SigC::Connection Gtk::AssociateVisibility(Gtk::Widget *w,const Model_ref<bool> &model, bool invert)
{  void (*cb)(gpointer gp,Model_ref<bool> model,Gtk::Widget *w)
	=invert ? &visibility_cb_inv : &visibility_cb;
   (*cb)(const_cast<gpointer>(static_cast<const void*>(&model.Value())),model,w);
   return model.signal_changed().connect(SigC::bind(SigC::slot(cb), model, w));
}

SigC::Connection Gtk::AssociateSensitivity(Gtk::Widget *w,const Model_ref<bool> &model, bool invert)
{  void (*cb)(gpointer gp,Model_ref<bool> model,Gtk::Widget *w)
	=invert ? &sensitivity_cb_inv : &sensitivity_cb;
   (*cb)(const_cast<gpointer>(static_cast<const void*>(&model.Value())),model,w);
   return model.signal_changed().connect(SigC::bind(SigC::slot(cb), model, w));
}
