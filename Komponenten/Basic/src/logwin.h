// $Id: logwin.h,v 1.1 2001/06/21 09:05:27 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <gtk--/list.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/viewport.h>

class logwin : public Gtk::ScrolledWindow
{  	Gtk::Viewport vp;
 	Gtk::List list;
   public:
	logwin(guint minimum_size=200);
	Gtk::List *get_list()
	{ return &list; } 
	void scroll() throw();
}; 
