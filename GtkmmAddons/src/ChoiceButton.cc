/*  Gtk--addons: a collection of gtk-- addons
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

ManuProC::ChoiceButton::ChoiceButton(bool tearoff)
	: actual_index(0), image(0), label(0), menu(0)
{  Gtk::VBox *vbox=manage(new Gtk::VBox());
   DoubleButton::add(*vbox);
   image=manage(new Gtk::Image());
   vbox->add(*image);
   label=manage(new Gtk::Label());
   vbox->add(*label);
   
   image->show();
   label->show();
   vbox->show();
   
   menu=new Gtk::Menu();
   signal_clicked().connect(SigC::slot(*this,&ChoiceButton::on_button_pressed));
   signal_secondpressed().connect(SigC::slot(*this,&ChoiceButton::on_sbutton_pressed));
}

ManuProC::ChoiceButton::~ChoiceButton()
{  if (menu) delete menu;
}

void ManuProC::ChoiceButton::add(const Glib::RefPtr<Gdk::Pixbuf> &_image, const Glib::ustring &text, const SigC::Slot0<void> &callback)
{  unsigned old_size=images.size();
   images.push_back(_image);
   texts.push_back(text);
   callbacks.push_back(callback);
   // menuitem erzeugen
   // scale?
   Gtk::Image *im=manage(new Gtk::Image(_image));
   menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(text,*im));
   Gtk::MenuItem *mi=(Gtk::ImageMenuItem *)&menu->items().back();
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

void ManuProC::ChoiceButton::on_sbutton_pressed(int mbutton)
{  // menu zeigen
   if (mbutton!=1) menu->popup(mbutton,0);
   else menu->popup(0,0);
}

void ManuProC::ChoiceButton::on_menuitem_selected(unsigned idx)
{  // index wechsel, Bild darstellen, callback ausführen
   set_index(idx);
   callbacks[idx]();
}

void ManuProC::ChoiceButton::set_index(unsigned idx)
{  // index wechsel, Bild darstellen
   image->set(images[idx]);
   label->set_text(texts[idx]);
   actual_index=idx;
}

void ManuProC::ChoiceButton::set_style(bool _image, bool _text)
{  if (_image) image->show(); else image->hide();
   if (_text) label->show(); else label->hide();
}
