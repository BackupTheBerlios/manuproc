// $Id: bool_ImageButton.hh,v 1.3 2003/03/07 08:27:17 christof Exp $
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

#ifndef MANUPROC_WIDGETS_MVC_BOOLIMAGEBUTTON_H
#define MANUPROC_WIDGETS_MVC_BOOLIMAGEBUTTON_H

#include <gtkmm/checkbutton.h>
#include <BaseObjects/Model.h>
#include <gtkmm/image.h>

class bool_ImageButton : public Gtk::CheckButton
{	typedef bool T;
	Glib::RefPtr<Gdk::Pixmap> off,on;
	Glib::RefPtr<Gdk::Bitmap> offm,onm;
	SigC::Connection ch_con, my_ch_con;
	Model_ref<T> model;
	Gtk::Image *imag;

	void init();
	void refresh(gpointer);
	void on_toggled();
	// explicitely forbid to use these (make them private)
	bool get_active() const;
	void set_active(bool);
public:
	bool_ImageButton(const Model_ref<T> &model, 
		const Glib::RefPtr<Gdk::Pixmap> &off, const Glib::RefPtr<Gdk::Bitmap> &offm,
		const Glib::RefPtr<Gdk::Pixmap> &on, const Glib::RefPtr<Gdk::Bitmap> &onm);
};

#endif
