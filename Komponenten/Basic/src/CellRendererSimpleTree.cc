// $Id: CellRendererSimpleTree.cc,v 1.6 2005/11/03 21:17:54 christof Exp $
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

#include <CellRendererSimpleTree.h>
#include <gdkmm/pixbufloader.h>
#include <gtkmm/treeview.h>

#include <iostream>

static const unsigned char plus_png_data[] = 
{	137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,
	0,0,0,9,0,0,0,9,1,3,0,0,0,218,95,148,
	83,0,0,0,6,80,76,84,69,0,0,0,248,0,0,30,
	176,155,167,0,0,0,24,73,68,65,84,120,156,99,96,96,
	96,168,103,96,40,7,35,71,24,3,40,194,192,0,0,43,
	149,3,28,93,38,150,240,0,0,0,0,73,69,78,68,174,
	66,96,130,
};
static const unsigned plus_png_size = 99;

static const unsigned char minus_png_data[] = 
{	137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,
	0,0,0,9,0,0,0,9,1,3,0,0,0,218,95,148,
	83,0,0,0,6,80,76,84,69,0,0,0,0,252,0,29,
	110,23,126,0,0,0,19,73,68,65,84,120,156,99,96,96,
	96,168,135,33,71,36,54,3,3,0,45,85,3,60,193,182,
	28,23,0,0,0,0,73,69,78,68,174,66,96,130,
};
static const unsigned minus_png_size = 94;

static Glib::RefPtr<Gdk::Pixbuf> plus,minus;

static void create_plus_minus()
{  if (!plus)
   {  Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
      loader->write(plus_png_data, plus_png_size);
      loader->close();
      plus=loader->get_pixbuf();
   }
   if (!minus)
   {  Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
      loader->write(minus_png_data, minus_png_size);
      loader->close();
      minus=loader->get_pixbuf();
   }
}

#define IMGSIZE 9
#define XSIZE IMGSIZE+2

CellRendererSimpleTree::CellRendererSimpleTree(guint col)
	: Glib::ObjectBase(typeid(CellRendererSimpleTree)),
	  column(col), childrens_deep(*this,"childrens_deep",0)
{  property_mode() = Gtk::CELL_RENDERER_MODE_ACTIVATABLE;
}

void CellRendererSimpleTree::get_size_vfunc(Gtk::Widget& widget, const Gdk::Rectangle* cell_area, 
			int* x_offset, int* y_offset, int* width, int* height) const
{  Parent::get_size_vfunc(widget,cell_area,x_offset,y_offset,width,height);
//std::cerr << column << "," << guint(childrens_deep) << '\n';
   if (column && guint(childrens_deep)==column)
   {  if (width) 
        *width+=property_xpad() * 2 + XSIZE;
      if (height && *height<(2*property_ypad() + IMGSIZE)) 
        *height=2*property_ypad() + IMGSIZE;
   }
}

static void adjust(Gdk::Rectangle &r,unsigned xp)
{ r.set_x(r.get_x()+ 2*xp + XSIZE);
  r.set_width(r.get_width() - (2*xp + XSIZE));
}

static void clamp(Gdk::Rectangle &r,Gdk::Rectangle const &mask)
{ if (r.get_x()<mask.get_x())
  { unsigned shift=mask.get_x()-r.get_x();
    r.set_x(r.get_x()+shift);
    r.set_width(r.get_width()-shift);
  }
}

void CellRendererSimpleTree::render_vfunc(const Glib::RefPtr<Gdk::Drawable>& window,
			Gtk::Widget& widget, 
			const Gdk::Rectangle& _background_area, 
			const Gdk::Rectangle& _cell_area, 
			const Gdk::Rectangle& _expose_area,
			Gtk::CellRendererState flags)
{  if (column && guint(childrens_deep)==column)
   {  Gdk::Rectangle cell_area=_cell_area;
      Gdk::Rectangle background_area=_background_area;
      Gdk::Rectangle expose_area=_expose_area;
      const unsigned int cell_xpad = property_xpad();
      const unsigned int cell_ypad = property_ypad();
      
      int y_offset=cell_area.get_height()/2-IMGSIZE/2;
      
      create_plus_minus();
//std::cerr << "render " << cell_area.get_y() << "," << y_offset << '\n';
      // background fehlt !
      window->draw_pixbuf(Gdk::GC::create(window),
      		property_is_expanded()?minus:plus,0,0,
      		cell_area.get_x() + cell_xpad,
      		cell_area.get_y() + y_offset + cell_ypad,
      		IMGSIZE,IMGSIZE,Gdk::RGB_DITHER_NORMAL,0,0);
      adjust(cell_area,property_xpad());
      clamp(expose_area,cell_area);
      clamp(background_area,cell_area);
      Parent::render_vfunc(window,widget,background_area,cell_area,expose_area,flags);
   }
   else Parent::render_vfunc(window,widget,_background_area,_cell_area,_expose_area,flags);
}

bool CellRendererSimpleTree::activate_vfunc(GdkEvent* event, Gtk::Widget& widget,
			const Glib::ustring& path, const Gdk::Rectangle& background_area,
			const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags)
{  if (column && guint(childrens_deep)==column) 
   {  Gtk::TreeView &tv=dynamic_cast<Gtk::TreeView &>(widget);
      Gtk::TreeModel::Path p=Gtk::TreeModel::Path(path);
      if (tv.row_expanded(p)) tv.collapse_row(p);
      else tv.expand_row(p,false);
      return true;
   }
   return false;
}

Glib::PropertyProxy<guint> CellRendererSimpleTree::property_childrens_deep()
{  return childrens_deep.get_proxy();
}

#if 0
enum { PROP_CHILDRENS_DEEP };

namespace {
class CellRendererSimpleTree_Class : public Glib::Class
{	typedef Gtk::CellRenderer_Class CppClassParent;
	typedef GtkCellRendererClass BaseClassParent;
public:
        const Glib::Class& init();
        static void class_init_function(void* g_class, void* class_data);
};
}

static CellRendererSimpleTree_Class myclass;

void CellRendererSimpleTree_Class::class_init_function(void* g_class, void* class_data)
{ BaseClassType *const klass = static_cast<BaseClassType*>(g_class);
  CppClassParent::class_init_function(klass, class_data);

  G_OBJECT_CLASS(klass)->get_property = gtk_cell_renderer_toggle_get_property;
  G_OBJECT_CLASS(klass)->set_property = gtk_cell_renderer_toggle_set_property;
  klass->plug_added = &plug_added_callback;
  klass->plug_removed = &plug_removed_callback;
}

static void gtk_cell_renderer_SimpleTree_init(GtkCellRenderer *cell)
{  GTK_CELL_RENDERER(cell)->mode = GTK_CELL_RENDERER_MODE_ACTIVATABLE;
}

const Glib::Class& CellRendererSimpleTree_Class::init()
{
    if (!gtype_)
    {
      static const GTypeInfo cell_SimpleTree_info =
      {
        sizeof (CellRendererClass),
        NULL,           /* base_init */
        NULL,           /* base_finalize */
        (GClassInitFunc) gtk_cell_renderer_SimpleTree_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GtkCellRenderer),
        0,              /* n_preallocs */
        (GInstanceInitFunc) gtk_cell_renderer_SimpleTree_init,
      };
      type = g_type_register_static (GTK_TYPE_CELL_RENDERER_TEXT,
		"CellRendererSimpleTree",&cell_SimpleTree_info, GTypeFlags(0));
    }
   return *this;
}
#endif
