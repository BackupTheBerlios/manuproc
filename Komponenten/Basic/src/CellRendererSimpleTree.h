// $Id: CellRendererSimpleTree.h,v 1.1 2004/01/06 09:07:09 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef KOMPONENTEN_CELLREND_SIMPLE_TREE
#define KOMPONENTEN_CELLREND_SIMPLE_TREE

#include <gtkmm/cellrenderer.h>

class CellRendererSimpleTree : public Gtk::CellRenderer
{protected:
	typedef Gtk::CellRenderer Parent;
	
//	SimpleTree &tree;
	const guint column; // which column this is in
	Glib::Property<guint> childrens_deep; // the changing property
	
	virtual void get_size_vfunc(Gtk::Widget& widget, const Gdk::Rectangle* cell_area, 
			int* x_offset, int* y_offset, int* width, int* height);
	virtual void render_vfunc(const Glib::RefPtr<Gdk::Window>& window,
			Gtk::Widget& widget, const Gdk::Rectangle& background_area, 
			const Gdk::Rectangle& cell_area, const Gdk::Rectangle& expose_area,
			Gtk::CellRendererState flags);
	virtual bool activate_vfunc(GdkEvent* event, Gtk::Widget& widget, 
			const Glib::ustring& path, const Gdk::Rectangle& background_area,
			const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags);
public:
//	CellRendererSimpleTree(SimpleTree &t, int col) 
//		: column(col), tree(t) {}
	CellRendererSimpleTree(guint col);
//		: column(col), childrens_deep(guint(-1)) {}
	Glib::PropertyProxy<guint> property_childrens_deep();
};
  
#endif
