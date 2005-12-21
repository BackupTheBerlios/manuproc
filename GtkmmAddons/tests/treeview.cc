// $Id: treeview.cc,v 1.1 2005/12/21 07:27:30 christof Exp $
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 1998  Adolf Petig GmbH. & Co. KG
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

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <Gtk_OStream.h>

class myWindow : public Gtk::Window
{	Gtk::TreeView tv;
        Glib::RefPtr<Gtk::ListStore> store;
        struct Columns : Gtk::TreeModelColumnRecord
        { Gtk::TreeModelColumn<Glib::ustring> a,b,c;
    
          Columns();
        };
        Columns cols;
public:
	myWindow()
	{	add(tv);
		tv.show();
		store=Gtk::ListStore::create(cols);
		tv.set_model(store);
		tv.append_column("A",cols.a);
		tv.append_column("B",cols.b);
		tv.append_column("C",cols.c);
	}
	Gtk::TreeView &TreeView()
	{  return tv; }
};

myWindow::Columns::Columns()
{ add(a); add(b); add(c);
}

int main(int argc,char **argv)
{  Gtk::Main m(argc, argv );
   myWindow w;
   w.show();
   {  Gtk::OStream os(&w.TreeView());
      os << "a\tb\tc\n";
      os << "d\te\tf\n"
        "g\th\ti\n"; 
//      os.flush();
   }
   m.run(w);
   return 0;
}
