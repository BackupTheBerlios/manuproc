/*  Gtk--addons: a collection of gtk-- addons Copyright (C) 2003 Adolf Petig
 *  GmbH & Co. KG, written by Christof Petig
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "ChoiceButton.h"
#include <gtkmm/box.h>
#include <gtkmm/menuitem.h>
#include <gtk/gtkmenu.h>

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#  include <sigc++/bind.h>
#endif

ManuProC::ChoiceButton::ChoiceButton(bool tearoff)
	: actual_index(), image(), label(), menu(), tips(),
		activate_on_change(true)
{  rebuild_button(false);
   
   menu=new Gtk::Menu();
   Gtk::TearoffMenuItem *tomi=manage(new Gtk::TearoffMenuItem());
   menu->add(*tomi);
   tomi->show();
   
   signal_clicked().connect(SigC::slot(*this,&ChoiceButton::on_button_pressed));
   signal_secondpressed().connect(SigC::slot(*this,&ChoiceButton::on_sbutton_pressed));
}

ManuProC::ChoiceButton::~ChoiceButton()
{  if (menu) delete menu;
}

void ManuProC::ChoiceButton::add(const Glib::RefPtr<Gdk::Pixbuf> &_image, 
        const Glib::ustring &text, const Glib::ustring &tooltip,
        const SigC::Slot0<void> &callback)
{  unsigned old_size=images.size();
   images.push_back(_image);
   texts.push_back(text);
   tooltips.push_back(tooltip);
   callbacks.push_back(callback);
   // scale?
   Gtk::Image *im=manage(new Gtk::Image(_image));
   menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(text,*im));
   Gtk::MenuItem *mi=(Gtk::ImageMenuItem *)&menu->items().back();
   if (tips && !tooltip.empty()) tips->set_tip(*mi, tooltip);
   im->show();
   mi->show();
   mi->signal_activate().connect(SigC::bind(SigC::slot(*this,&ChoiceButton::on_menuitem_selected),old_size));
   
   if (!old_size) set_index(0);
}

void ManuProC::ChoiceButton::on_button_pressed()
{  if (callbacks.empty() || actual_index<0 || actual_index>=callbacks.size()) 
      return;
   callbacks[actual_index]();
}

static void PosCalc(GtkMenu *menu,gint *x,gint *y,gboolean *push_in,gpointer user_data)
{  (*y)+=4;
   (*push_in)=true;
}

#if 0
static void PosCalc(int &x, int &y, bool &push_in)
{  y+=4;
   push_in=true;
}

static Gtk::Menu::SlotPositionCalc pc_slot=&PosCalc;
#endif

void ManuProC::ChoiceButton::on_sbutton_pressed(int mbutton)
{  if (mbutton!=1) menu->popup(mbutton,0);
   else 
   {  gtk_menu_popup(menu->gobj(),0,0,&PosCalc,0,0,0);
   }
}

void ManuProC::ChoiceButton::on_menuitem_selected(unsigned idx)
{  // index wechsel, Bild darstellen, callback ausführen
   set_index(idx);
   signal_changed()();
   if (activate_on_change) callbacks[idx]();
}

void ManuProC::ChoiceButton::set_index(unsigned idx)
{  // index wechsel, Bild darstellen
   image->set(images[idx]);
   label->set_text(texts[idx]);
  if (tips)
  { if (!label->is_visible()) 
      tips->set_tip(*this, texts[idx]+(tooltips[idx].empty()
              ?Glib::ustring()
              :("\n"+tooltips[idx])));
    else if (!tooltips[idx].empty())
      tips->set_tip(*this,tooltips[idx]);
  }
  actual_index=idx;
//   signal_changed()(); ?
}

void ManuProC::ChoiceButton::set_style(bool _image, bool _text, bool horizontal)
{  if (_image) image->show(); else image->hide();
   if (_text) 
   {  label->show();
      if (tips) tips->unset_tip(*this);
   }
   else 
   {  label->hide();
      if (tips) tips->set_tip(*this, texts[actual_index]);
   }
   if (horizontal && !dynamic_cast<Gtk::HBox*>(get_child())) 
   // you cannot turn this off - if you want to do it, implement it
     rebuild_button(horizontal);
}

void ManuProC::ChoiceButton::set_tooltips(Gtk::Tooltips *_tips)
{  tips=_tips; 
   if (!label->is_visible() && tips) 
         tips->set_tip(*this, texts[actual_index]);
}

void ManuProC::ChoiceButton::set_index_sensitive(unsigned idx,bool sensitive)
{ menu->items()[idx+1].set_sensitive(sensitive);
}

void ManuProC::ChoiceButton::rebuild_button(bool horizontal)
{ remove();
   Gtk::Box *box=0;
   if (horizontal) box=manage(new Gtk::HBox());
   else box=manage(new Gtk::VBox());
   DoubleButton::add(*box);
   image=manage(new Gtk::Image());
   box->add(*image);
   label=manage(new Gtk::Label());
   box->add(*label);
   
   image->show();
   label->show();
   box->show();
}

void ManuProC::ChoiceButton::add(const Glib::RefPtr<Gdk::Pixbuf> &_image, 
        const Glib::ustring &text, const SigC::Slot0<void> &callback)
{ add(_image,text,Glib::ustring(),callback);
}

void ManuProC::ChoiceButton::add(const Glib::ustring &text, const SigC::Slot0<void> &callback)
{ add(Glib::RefPtr<Gdk::Pixbuf>(),text,Glib::ustring(),callback);
}

