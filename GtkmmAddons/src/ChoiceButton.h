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

#include"DoubleButton.h"
#include<vector>
#include<string>

namespace ManuProC {

class ChoiceButton : public DoubleButton
{	int actual_index;
	std::vector<Glib_Refptr<Gdk::Pixbuf> > images;
	std::vector<Gtk::ustring> texts;
	std::vector<SigC::Slot0<void> > callbacks;
 
public:
	ChoiceButton();
	add(const Glib_Refptr<Gdk::Pixbuf> &image, const Gtk::ustring &text, const SigC::Slot0<void> &callback);
	unsigned get_index() const { return actual_index; }
	void set_index();
};

}
#endif
