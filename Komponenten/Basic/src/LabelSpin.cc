/*  Midgard Character Generator
 *  Copyright (C) 2001-2002 Malte Thoma
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


#include "LabelSpin.hh"
#include <Misc/itos.h>
#include <gtkmm/adjustment.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

LabelSpin::LabelSpin(int v,int lower,int upper)
: value(v)
{
   label=manage(new class Gtk::Label(itos(value)));
   Gtk::Adjustment *adj = manage(new class Gtk::Adjustment(value, lower, upper));
   spinbutton = manage(new class Gtk::SpinButton(*adj));
   spinbutton->signal_activate().connect(SigC::slot(*static_cast<class LabelSpin*>(this), &LabelSpin::spin_activate),true);   
   
   set_flags(Gtk::CAN_FOCUS);
   signal_focus_in_event().connect(SigC::slot(*this, &LabelSpin::Focus_in_event),true);
   signal_focus_out_event().connect(SigC::slot(*this, &LabelSpin::Focus_out_event),true);


   pack_start(*label);
   pack_start(*spinbutton);
   label->show();   
   show();
}

void LabelSpin::spin_activate()
{
  spinbutton->update();
  if(label->is_visible()) value+=spinbutton->get_value_as_int();
  else  value=spinbutton->get_value_as_int();
  spinbutton->hide();
  label->set_text(itos(value));
  label->show();
  activate();
}

void LabelSpin::deaktivate() const
{
  spinbutton->hide();
  label->set_text(itos(value));
  label->show();
}

void LabelSpin::set_value(int v)
{
  value=v;
  label->set_text(itos(value));
  spinbutton->set_value(value);
}

void LabelSpin::edit_new() const
{
  spinbutton->set_value(value);
  spinbutton->show();
  label->hide();
  spinbutton->grab_focus();
  spinbutton->select_region(0,-1);
}

void LabelSpin::edit_add() const
{
  label->set_text(label->get_text()+" + ");
  spinbutton->set_value(0);
  spinbutton->show();
  spinbutton->grab_focus();
  spinbutton->select_region(0,-1);
}

bool LabelSpin::Focus_in_event(GdkEventFocus *ev)
{
 spinbutton->grab_focus();
 spinbutton->select_region(0,-1);
 return false;
}

bool LabelSpin::Focus_out_event(GdkEventFocus *ev)
{
// spin_activate();
 FocusOutEvent();
 return false;
}

