// $Id: int_ImageButton.hh,v 1.2 2004/07/16 06:58:44 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG
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

#ifndef _910AED05_87A6_48A8_B996_B969AFCB9F48
#define _910AED05_87A6_48A8_B996_B969AFCB9F48

#include <gtkmm/eventbox.h>
#include <ModelWidgetConnection.h>
#include <gtkmm/image.h>
#include <map>
namespace Gtk { class Tooltips; }

class int_ImageButton : public Gtk::EventBox
{	typedef int T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::Image>
	{	struct properties
		{  Glib::RefPtr<Gdk::Pixbuf> pixbuf;
		   std::string tooltip;
		};
		std::map<int,properties> props;
		Gtk::Widget *eventbox;
		SigC::Connection toggleconn;
		// tooltips
		Gtk::Tooltips *tips;

		void model2widget();
		void widget2model() {}
		SigC::Connection connect();
		void disconnect();
		
		bool toggle(GdkEventButton *ev);
	public:
		Connection(const Model_ref<T> &m) : this_t(m), eventbox(), tips() { }
		Connection(Widget *w) : eventbox(w), tips() { }
		void set_widget(widget_t *w, Widget *eventbox=0);
		void add_entry(int value, Glib::RefPtr<Gdk::Pixbuf> pixbuf, 
				const std::string &tooltip=std::string());
		void set_tooltips(Gtk::Tooltips *_tips) 
		{ tips=_tips; }
	};
private:
	Connection conn;
	Gtk::Image *imag;

public:
	int_ImageButton(const Model_ref<T> &model);
	void add_entry(int value, Glib::RefPtr<Gdk::Pixbuf> pixbuf, 
				const std::string &tooltip=std::string())
	{  conn.add_entry(value,pixbuf,tooltip); }
	void set_tooltips(Gtk::Tooltips *_tips) 
	{ conn.set_tooltips(_tips); }
};

#endif
