// $Id: string_Label.hh,v 1.1 2003/04/07 12:33:17 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#ifndef MANUPROC_WIDGETS_STRING_LABEL_H
#define MANUPROC_WIDGETS_STRING_LABEL_H

#include <gtkmm/label.h>
#include <ModelWidgetConnection.h>

class string_Label : public Gtk::Label
{	typedef std::string T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::Label>
	{	void model2widget();
	public:
		Connection(widget_t *w=0) { set_widget(w); }
		Connection(const Model_ref<T> &m, widget_t *w=0) 
		: this_t(m) { set_widget(w); }
	};
private:
	Connection conn;
	
	// explicitely forbid to use these (make them private)
	Glib::ustring get_text() const;
	void set_text(const Glib::ustring& text);
public:
	string_Label(const Model_ref<T> &model);
};

#endif
