// $Id: gtkhacks.h,v 1.2 2004/04/29 14:24:40 christof Exp $
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

#include <gtkmm/treeviewcolumn.h>

// -1 on error
int get_ModelColumn(const Gtk::TreeViewColumn *tvc,const std::string &property_name);
