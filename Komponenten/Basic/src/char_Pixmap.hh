// $Id: char_Pixmap.hh,v 1.6 2003/04/07 14:28:21 christof Exp $
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

#ifndef MANUPROC_WIDGETS_MVC_CHAR_PIX_H
#define MANUPROC_WIDGETS_MVC_CHAR_PIX_H

#include <gtkmm/image.h>
#include <ModelWidgetConnection.h>

// an Image which's content might be changed via the xpm data model

class char_Pixmap : public Gtk::Image
{	typedef const char * const * T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::Image>
	{	void model2widget();
	public:
		Connection(widget_t *w=0) { set_widget(w); }
		Connection(const Model_ref<T> &m, widget_t *w=0) 
		: this_t(m) { set_widget(w); }
	};
private:
	Connection conn;
	
public:
	char_Pixmap(const Model_ref<T> &model);
};

#endif
