// $Id: optionmenu.cc,v 1.4 2004/04/29 14:24:40 christof Exp $
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
#include <Gtk_OStream.h>
#include <SelectMatching.h>

class myWindow : public Gtk::Window
{	Gtk::OptionMenu l;
public:
	myWindow()
	{	add(l);
		l.show();
		// l.set_usize(100,100);
	}
	Gtk::OptionMenu &OptionMenu()
	{  return l; }
};

int main(int argc,char **argv)
{  Gtk::Main m(argc, argv );
   myWindow w;
   w.show();
   {  Gtk::OStream os(&w.OptionMenu());
      os << "test"; os.flush();
      os << "test 17"; os.flush((void*)17);
      os << "test 23"; os.flush((void*)23);
   }
   Gtk::Menu_Helpers::SelectMatching(w.OptionMenu(),(void*)17);
   m.run(w);
   return 0;
}
