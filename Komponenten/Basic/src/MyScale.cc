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

#include "MyScale.hh"
#include <gtkmm/adjustment.h>
#include <gtkmm/label.h>
#include <gtkmm/arrow.h>
#include <Misc/itos.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

void MyScale::init()
{
   ltop    = manage(new class Gtk::Label("x%"));
   lbottom = manage(new class Gtk::Label("x%"));

   ltop->set_alignment(0.5,0.5);
   ltop->set_padding(0,0);
   ltop->set_justify(Gtk::JUSTIFY_CENTER);
   ltop->set_line_wrap(false);
   lbottom->set_alignment(0.5,0.5);
   lbottom->set_padding(0,0);
   lbottom->set_justify(Gtk::JUSTIFY_CENTER);
   lbottom->set_line_wrap(false);

   Gtk::Arrow *arrowdown = manage(new class Gtk::Arrow(Gtk::ARROW_DOWN, Gtk::SHADOW_OUT));
   Gtk::Arrow *arrowup = manage(new class Gtk::Arrow(Gtk::ARROW_UP, Gtk::SHADOW_OUT));
   bup = manage(new class Gtk::Button());
   bup->set_flags(Gtk::CAN_FOCUS);
   bup->set_relief(Gtk::RELIEF_NONE);
   bup->add(*arrowup);
   bup->signal_clicked().connect(SigC::slot(*this, &MyScale::on_bup_clicked));
   bdown = manage(new class Gtk::Button());
   bdown->set_flags(Gtk::CAN_FOCUS);
   bdown->set_relief(Gtk::RELIEF_NONE);
   bdown->add(*arrowdown);
   bdown->signal_clicked().connect(SigC::slot(*this, &MyScale::on_bdown_clicked));


   Gtk::Adjustment *adj= manage(new class Gtk::Adjustment(value, 0, 100, 1, 1, 0));
   scale = manage(new class Gtk::VScale(*adj));  
   scale->signal_button_release_event().connect(SigC::slot(*static_cast<class MyScale*>(this),&MyScale::on_scale_release_event));
   scale->set_digits(0);
   scale->set_draw_value(false);
   scale->set_flags(Gtk::CAN_FOCUS);
   
   attach(*ltop, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(0), 0, 0); 
   attach(*bup, 0, 1, 1, 2, Gtk::AttachOptions(0), Gtk::AttachOptions(0), 0, 0); 
   attach(*scale, 0, 1, 2, 3, Gtk::AttachOptions(0), Gtk::EXPAND|Gtk::FILL,0, 0);
   attach(*bdown, 0, 1, 3, 4, Gtk::AttachOptions(0), Gtk::AttachOptions(0), 0, 0); 
   attach(*lbottom, 0, 1, 4, 5, Gtk::FILL, Gtk::AttachOptions(0), 0, 0); 
   show_all();

   set_labels();
}

void MyScale::show_arrows(bool b)
{
  if(b) {bup->show();bdown->show();}
  else  {bup->hide();bdown->hide();}
}


void MyScale::set_value(int v)
{
  if     (v<0)  v=0;
  else if(v>100)v=100; 
  value=v;
  scale->get_adjustment()->set_value(value);
  set_labels();
}


bool MyScale::on_scale_release_event(GdkEventButton *ev)
{
   value=(int)scale->get_adjustment()->get_value();   
   set_labels();
   activate();
   return false;
}

void MyScale::set_labels()
{
   ltop->set_text(itos(100-value)+"%");
   lbottom->set_text(itos(value)+"%");
}

void MyScale::on_bup_clicked()
{
  int v=(int)scale->get_adjustment()->get_value();
  set_value(v-1);
}

void MyScale::on_bdown_clicked()
{
  int v=(int)scale->get_adjustment()->get_value();
  set_value(v+1);
}

