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

#ifndef LABELSPIN
#define LABELSPIN
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>


class LabelSpin : public Gtk::HBox
{
      int value;
      Gtk::Label *label;
      Gtk::SpinButton *spinbutton;
      
      void spin_activate();
      bool Focus_in_event(GdkEventFocus *ev);
      bool Focus_out_event(GdkEventFocus *ev);
      SigC::Signal0<void> activate;
      SigC::Signal0<void> FocusOutEvent;

   public:
      LabelSpin(int value=0,int lower=0,int upper=100); 
      SigC::Signal0<void> &signal_activate()
      {  return activate; }
      SigC::Signal0<void> &signal_FocusOutEvent()
      {  return FocusOutEvent; }

      void set_value(int v);
      int get_value() const {return value;}
      void deaktivate() const;
      void edit_new() const;
      void edit_add() const;
};

#endif

