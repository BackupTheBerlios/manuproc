// $Id: Gtk_OStream_TreeView.cc,v 1.7 2004/04/29 14:24:40 christof Exp $
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
#include "gtkhacks.h"

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
      
      int column=get_ModelColumn(*coli,"text");
      if (column!=-1)
         row.set_value(column, Glib::ustring(line.substr(b,end)));
           
      ++coli;
      b=end+1;
   } while(end!=std::string::npos);
}

