// $Id: bool_CheckMenuItem.hh,v 1.5 2003/04/07 13:26:32 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002-2003 Adolf Petig GmbH & Co. KG
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

#ifndef MANUPROC_WIDGETS_MVC_BOOL_CHECKMENU_H
#define MANUPROC_WIDGETS_MVC_BOOL_CHECKMENU_H

#include <gtkmm/checkmenuitem.h>
#include <gtkmm/table.h>
#include <ModelWidgetConnection.h>

class bool_CheckMenuItem : public Gtk::CheckMenuItem
{	typedef bool T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::CheckMenuItem>
	{	void model2widget();
		void widget2model();
		SigC::Connection connect();
	public:
		Connection(widget_t *w=0) { set_widget(w); }
		Connection(const Model_ref<T> &m, widget_t *w=0) 
		: this_t(m) { set_widget(w); }
	};
private:
	Connection conn;
	
	// explicitely forbid to use these (make them private)
	bool get_active() const;
	void set_active(bool);
public:
	bool_CheckMenuItem(const Model_ref<T> &model, const std::string &text);
	bool_CheckMenuItem(const Model_ref<T> &model, Gtk::Widget &widget);

	// you might as well use bool_properties.hh directly for this
	void setSensitive(const Model_ref<bool> &s,const bool sensitive_invert=false);
};

#endif
