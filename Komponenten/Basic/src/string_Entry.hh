// $Id: string_Entry.hh,v 1.2 2002/12/03 09:09:05 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#ifndef MANUPROC_WIDGETS_MVC_STRING_H
#define MANUPROC_WIDGETS_MVC_STRING_H

#include <gtkmm/entry.h>
#include <BaseObjects/Model.h>

class MVC_string_Widget : public Gtk::Entry
{	typedef std::string T;
	bool any_change;
	SigC::Connection ch_con, my_ch_con;
	Model_ref<T> model;
	
	bool on_focus_out(GdkEventFocus *ev);
	bool on_focus_in(GdkEventFocus *ev);
	void refresh(gpointer);
	void keypress();
	void on_activate();
	// explicitely forbid to use these (make them private)
	Glib::ustring get_text() const;
	unsigned int get_text_length() const;
	void set_text(const Glib::ustring& text);
public:
	MVC_string_Widget(const Model_ref<T> &model);
};

#endif
