// $Id: gtkhacks.cc,v 1.2 2004/04/29 14:24:40 christof Exp $
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 2003  Adolf Petig GmbH. & Co. KG
    Developed by Christof Petig <christof@petig-baender.de>
 
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

// HACKERY !
// there seems to be no decent way to get the model column 
// of a treeview column's text attribute

#include <gtk/gtktreeviewcolumn.h>
#include "gtkhacks.h"

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
int get_ModelColumn(const Gtk::TreeViewColumn *tvc,const std::string &property_name)
{  const GtkTreeViewColumn *tree_column= tvc->gobj();
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
	  if (name && name==property_name) return number;
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

