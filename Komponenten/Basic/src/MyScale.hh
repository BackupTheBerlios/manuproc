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

#ifndef MYSCALE
#define MYSCALE

#include <gtkmm/table.h>
#include <gtkmm/label.h>
#include <gtkmm/scale.h>
#include <gtkmm/button.h>


class MyScale : public Gtk::Table
{
   private:
      int value;
      void init();
      bool on_scale_release_event(GdkEventButton *ev);
      void on_bup_clicked();
      void on_bdown_clicked();
      void set_labels();
      void changed();


      Gtk::Label *ltop,*lbottom;
      Gtk::VScale *scale;
      Gtk::Button *bup,*bdown;

      SigC::Signal0<void> activate;
   public:
      MyScale():value(50),ltop(0),lbottom(0),bup(0),bdown(0) {init();}

      void show_arrows(bool b);

      int get_value() const {return value;}
      void set_value(int v);

      SigC::Signal0<void> &signal_activate()
      {  return activate; }
};


#endif
