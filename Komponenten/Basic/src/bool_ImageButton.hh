// $Id: bool_ImageButton.hh,v 1.7 2004/07/15 09:25:57 christof Exp $
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

#include <gtkmm/eventbox.h>
#include <ModelWidgetConnection.h>
#include <gtkmm/image.h>
namespace Gtk { class Tooltips; }

class bool_ImageButton : public Gtk::EventBox
{	typedef bool T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::Image>
	{	Glib::RefPtr<Gdk::Pixbuf> off,on;
		Gtk::Widget *eventbox;
		SigC::Connection toggleconn;
		// tooltips
		Gtk::Tooltips *tips;
		std::string tip_off,tip_on;

		void model2widget();
		void widget2model() {}
		SigC::Connection connect();
		void disconnect();
		
		bool toggle(GdkEventButton *ev);
	public:
		Connection(const Model_ref<T> &m) 
			: this_t(m), eventbox(), tips() { }
		void set_widget(widget_t *w, Widget *eventbox=0);
		void set_images(const Glib::RefPtr<Gdk::Pixbuf> &off,
				const Glib::RefPtr<Gdk::Pixbuf> &on);
		void set_tooltips(Gtk::Tooltips *_tips,
			const std::string &off,const std::string &on)
		{  tips=_tips;
		   set_tooltips(off,on);
		}
		// perhaps not overly useful, unless you want to _change_ them ...
		void set_tooltips(const std::string &off,const std::string &on);
	};
private:
	Connection conn;
	Gtk::Image *imag;

public:
	bool_ImageButton(const Model_ref<T> &model, 
		const Glib::RefPtr<Gdk::Pixbuf> &off,
		const Glib::RefPtr<Gdk::Pixbuf> &on);
	void set_tooltips(Gtk::Tooltips *_tips,
			const std::string &off,const std::string &on)
	{  conn.set_tooltips(_tips,off,on);
	}
};

#endif
