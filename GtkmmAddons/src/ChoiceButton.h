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


#ifndef CHOICEBUTTON_H
#define CHOICEBUTTON_H

#include "DoubleButton.h"
#include <vector>
#include <string>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/menu.h>
#include <gtkmm/tooltips.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/slot.h>
#endif

namespace ManuProC {

class ChoiceButton : public DoubleButton
{	unsigned actual_index;
	Gtk::Image *image;
	Gtk::Label *label;
	Gtk::Menu *menu;
	Gtk::Tooltips *tips;
	
	std::vector<Glib::RefPtr<Gdk::Pixbuf> > images;
	std::vector<Glib::ustring> texts;
	std::vector<Glib::ustring> tooltips;
	std::vector<SigC::Slot0<void> > callbacks;
	SigC::Signal0<void> changed;
//	std::vector<Gtk::MenuItem *> menuitems;
	bool activate_on_change;

	void on_button_pressed();
	void on_sbutton_pressed(int mbutton);
	void on_menuitem_selected(unsigned idx);
	
	void rebuild_button(bool horizontal);
 
public:
	ChoiceButton(bool tearoff=true);
	~ChoiceButton();
	void add(const Glib::RefPtr<Gdk::Pixbuf> &image, 
		const Glib::ustring &text, const Glib::ustring &tooltip, 
		const SigC::Slot0<void> &callback=SigC::Slot0<void>());
	void add(const Glib::RefPtr<Gdk::Pixbuf> &image, 
		const Glib::ustring &text, const SigC::Slot0<void> &callback=SigC::Slot0<void>());
	void add(const Glib::ustring &text, const SigC::Slot0<void> &callback=SigC::Slot0<void>());

	unsigned get_index() const { return actual_index; }
	void set_index(unsigned idx);

	void set_style(bool image=true, bool text=false, bool horizontal=false);
	void set_tearoff_title(const Glib::ustring &title);
	void set_tooltips(Gtk::Tooltips *_tips);

	void set_activate_on_change(bool x) { activate_on_change=x; }
	SigC::Signal0<void> &signal_changed()
	{  return changed; }

	void set_index_sensitive(unsigned idx,bool sensitive);
};

}
#endif
