// $Id: Gtk_OStream_TreeView.cc,v 1.3 2003/11/03 07:26:49 christof Exp $
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 2002  Adolf Petig GmbH. & Co. KG
    Developed by Christof Petig <christof.petig@wtal.de>
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "Gtk_OStream.h"
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeviewcolumn.h>

static int get_ModelColumn(Gtk::TreeViewColumn *tvc);

void Gtk::OStream::erase_TreeView(openmode mode)
{  if (mode&std::ios::trunc) 
   {  Glib::RefPtr<TreeModel> r_model=handler_data.treeview.view->get_model();
      Glib::RefPtr<Gtk::ListStore> store=Glib::RefPtr<Gtk::ListStore>::cast_dynamic(r_model);
      if (!store) 
      {  std::cerr << "Model is not a ListStore ...\n";
         return;
      }
      store->clear();
   }
}

void Gtk::OStream::line_TreeView(const std::string &line)
{  Glib::RefPtr<TreeModel> r_model=handler_data.treeview.view->get_model();
   Glib::RefPtr<Gtk::ListStore> store=Glib::RefPtr<Gtk::ListStore>::cast_dynamic(r_model);
   if (!store) 
   {  std::cerr << "Model is not a ListStore ...\n";
      return;
   }
   Glib::ListHandle<TreeViewColumn*> cols=handler_data.treeview.view->get_columns();
   
   std::string::size_type b=0,end=std::string::npos;
   const unsigned linesize=line.size();
   Glib::ListHandle<TreeViewColumn*>::const_iterator coli=cols.begin();
   Gtk::TreeRow row = *store->append();
   do
   {  if (coli==cols.end()) break;
      if (b>=linesize) break;
      end=line.find('\t',b);
      
      int column=get_ModelColumn(*coli);
      if (column!=-1)
         row.set_value(column, Glib::ustring(line.substr(b,end)));
           
      ++coli;
      b=end+1;
   } while(end!=std::string::npos);
}

// HACKERY !
// there seems to be no decent way to get the model column 
// of a treeview column's text attribute

#include <gtk/gtktreeviewcolumn.h>

namespace {
// copied from gtktreeviewcolumn.c ...
struct my_GtkTreeViewColumnCellInfo
{
  GtkCellRenderer *cell;
  GSList *attributes;
  // and more I don't care for
};
}

// copied from gtk_tree_view_column_cell_set_cell_data
static int get_ModelColumn(Gtk::TreeViewColumn *tvc)
{  GtkTreeViewColumn *tree_column= tvc->gobj();
   GList *cell_list;
   GSList *list;

   g_return_val_if_fail (GTK_IS_TREE_VIEW_COLUMN (tree_column), -1);
   g_return_val_if_fail (tree_column->cell_list != NULL, -1);
     
  for (cell_list = tree_column->cell_list; cell_list; cell_list = cell_list->next)
    {
      struct my_GtkTreeViewColumnCellInfo *info = (my_GtkTreeViewColumnCellInfo *) cell_list->data;
      list = info->attributes;
      
      while (list && list->next)
	{ gint number=GPOINTER_TO_INT (list->next->data);
	  const gchar *name=(const gchar *)list->data;
	  if (name && !strcmp(name,"text")) return number;
	  list = list->next->next;
	}
    }
  return -1;
}


// gtk_cell_renderer_text_get_property
//      ->_property_renderable();
      
//      GtkCellRenderer *cr=coli->get_first_cell_renderer()->_property_renderable
// gobj();
//      Gtk::CellRendererText *crt=dynamic_cast<Gtk::CellRendererText*>(coli->get_first_cell_renderer());
      
//      Glib::Object *obj=cr->property_text().get_object();

