// $Id: Gtk_OStream_optionmenu.cc,v 1.4 2004/04/29 14:24:40 christof Exp $
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

#include "Gtk_OStream.h"
#include <gtkmm/optionmenu.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/menu.h>

// because of some strance gtk-1.2 bug we can't set the menu at open ??
void Gtk::OStream::erase_OptionMenu(openmode mode)
{  if (mode&std::ios::trunc)  handler_data.optionmenu.menu=manage(new Gtk::Menu());
   else 
   {  handler_data.optionmenu.menu=handler_data.optionmenu.widget->get_menu();
      if (!handler_data.optionmenu.menu) 
      {  mode=(mode&~std::ios::app)|std::ios::trunc;
         handler_data.optionmenu.menu=manage(new Gtk::Menu());
      }
   }
}

void Gtk::OStream::close_OptionMenu()
{  if (handler_data.optionmenu.widget->get_menu()!=handler_data.optionmenu.menu) 
      handler_data.optionmenu.widget->set_menu(*handler_data.optionmenu.menu);
}

void Gtk::OStream::line_OptionMenu(const std::string &line)
{   Gtk::MenuItem *item=manage(new Gtk::MenuItem(line));
    handler_data.optionmenu.menu->append(*item);
    item->show();
    if (user_data) item->set_data("user_data",user_data,notify);
}
