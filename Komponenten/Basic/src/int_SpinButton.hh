// $Id: int_SpinButton.hh,v 1.5 2003/04/07 06:38:04 christof Exp $
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

#ifndef MANUPROC_WIDGETS_MVC_INT_H
#define MANUPROC_WIDGETS_MVC_INT_H

#include <gtkmm/spinbutton.h>
#include <ModelWidgetConnection.h>

class int_SpinButton : public Gtk::SpinButton
{	typedef int T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::SpinButton>
	{	bool any_change;
		SigC::Connection cm_con2[3];

		bool on_focus_out(GdkEventFocus *ev);
		bool on_focus_in(GdkEventFocus *ev);
		void keypress();
		void on_activate();
		
		void model2widget();
		void widget2model();
		SigC::Connection connect();
		void disconnect();
	public:
		Connection(widget_t *w=0) : any_change(false) 
		{ set_widget(w); }
		Connection(const Model_ref<T> &m, widget_t *w=0) 
		: this_t(m), any_change(false) { set_widget(w); }
	};
private:
	Connection conn;
	
	// explicitely forbid to use these (make them private)
	gfloat get_value_as_float() const;
	gint get_value_as_int() const;
	void set_value(gfloat);
public:
	int_SpinButton(const Model_ref<T> &model,T min,T max);
};

#endif
