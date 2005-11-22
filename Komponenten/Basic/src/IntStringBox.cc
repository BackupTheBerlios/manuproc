// $Id: IntStringBox.cc,v 1.2 2005/11/22 13:03:33 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++ Copyright (C)
 *  1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "IntStringBox.hh"
#include <Misc/itos.h>
#include <gtk/gtksignal.h>
#include <cassert>

void IntStringBox::hide_int(bool b)
{
 if(b)
  { label_int->hide(); 
    sc_int->hide();
  }
 else 
  { label_int->show(); 
    sc_int->show();
  }
}



void IntStringBox::show_string2(bool b) 
{ 
 if(b)
   {
    label_string2->show(); 
    sc2_string->show();
   }
 else
   {
    label_string2->hide(); 
    sc2_string->hide();
   }
}

void IntStringBox::string2_info_only(bool b)
{
  sc2_string->set_sensitive(!b);
  string_2_info_only=b;
}

void IntStringBox::setLabel(const std::string &nr,const std::string &name,const std::string& namez)
{
 label_int->set_text(nr);
 label_string1->set_text(name);
 label_string2->set_text(namez);
}

void IntStringBox::int_activate()
{  // search Name and activate
   set_value(strtol(sc_int->get_text().c_str(),0,10));
   activate();
}

void IntStringBox::string1_activate()
{
  if(!try_to_get_id() && sc2_string->is_visible())  
   {  sc2_string->set_always_fill(true);
      sc2_string->reset();
      sc2_string->grab_focus();
   }
}

void IntStringBox::string2_activate()
{  if(!try_to_get_id())  
   {  sc1_string->set_always_fill(true);
      sc1_string->reset();
      sc1_string->grab_focus();
   }
}

gint IntStringBox::try_grab_focus(GtkWidget *w,gpointer gp)
{
   IntStringBox *this2((IntStringBox*)gp);
   assert(dynamic_cast<Gtk::EventBox*>(this2));
/*
   assert(Gtk::Table::isA(this2)); // very weak check
*/
   this2->sc_int->grab_focus();
   return true;
}

IntStringBox::IntStringBox(ManuProcEntity<>::ID __none_id) 
: id(__none_id),tr("",false), tr2("",false), 
tr3("",false),
  eingeschraenkt(false),
  string_2_info_only(false),
  translate(false),
  _none_id_(__none_id)
{
 show_string2(false);
 // redirect our grab_focus
 gtk_signal_connect(GTK_OBJECT(gobj()), "grab_focus",
                 GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
 sc_int->get_entry()->set_width_chars(8);
}

void IntStringBox::set_value(ManuProcEntity<>::ID i,const std::string &s, 
				const std::string& sz)
{
   sc_int->set_text(itos(i));
   sc1_string->set_text(s);
   sc2_string->set_text(sz);
   id=i;
}

void IntStringBox::reset()
{  
   sc_int->reset();
   sc1_string->reset();
   sc1_string->set_always_fill(false);
   sc2_string->reset();
   sc2_string->set_always_fill(false);
   id=_none_id_;
   reset_signal();

}


void IntStringBox::Einschraenken(bool an)
{
  eingeschraenkt=true;
}

void IntStringBox::Einschraenkung(const std::string &e, bool an)
{
 eingeschraenkt=an;
 einschraenkung=e;
}

void IntStringBox::Join(const std::string j)
{
 joinstring=j;
}
  

void IntStringBox::setExpandStr2(bool expand)
{
 sc2_string->set_autoexpand(expand);
 sc2_string->set_enable_tab(true);  
}
 
void IntStringBox::setExpandStr1(bool expand)
{
 sc1_string->set_autoexpand(expand);
 sc1_string->set_enable_tab(true);  
}



