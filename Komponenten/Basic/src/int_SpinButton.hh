// $Id: int_SpinButton.hh,v 1.3 2002/12/03 09:10:44 christof Exp $
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
#include <BaseObjects/Model.h>

class int_SpinButton : public Gtk::SpinButton
{	typedef int T;
	bool any_change;
	SigC::Connection ch_con, my_ch_con;
	Model_ref<T> model;
	
	bool on_focus_out(GdkEventFocus *ev);
	bool on_focus_in(GdkEventFocus *ev);
	void refresh(gpointer);
	void keypress();
	void on_activate();
	// explicitely forbid to use these (make them private)
	gfloat get_value_as_float() const;
	gint get_value_as_int() const;
	void set_value(gfloat);
public:
	int_SpinButton(const Model_ref<T> &model,T min,T max);
};

#endif
