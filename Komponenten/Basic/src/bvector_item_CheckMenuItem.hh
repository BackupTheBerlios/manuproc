// $Id: bvector_item_CheckMenuItem.hh,v 1.1 2003/10/22 12:04:50 christof Exp $
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

#ifndef MANUPROC_WIDGETS_MVC_BVECTOR_CHECKMENU_H
#define MANUPROC_WIDGETS_MVC_BVECTOR_CHECKMENU_H

#include <gtkmm/checkmenuitem.h>
#include <gtkmm/table.h>
#include <ModelWidgetConnection.h>
#include <BaseObjects/Model_ref_bvector.h>

class bvector_item_CheckMenuItem : public Gtk::CheckMenuItem
{	typedef bvector_item T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::CheckMenuItem,bvector_iterator>
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
	bvector_item_CheckMenuItem(const Model_ref<T> &model, const std::string &text);
	bvector_item_CheckMenuItem(const Model_ref<T> &model, Gtk::Widget &widget);
};

#endif
