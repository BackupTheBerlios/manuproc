// $Id: TreeModelDebug.cc,v 1.2 2004/05/05 12:00:44 christof Exp $
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

#include "TreeModelDebug.h"
#include <iostream>
#include <gtkmm/treepath.h>

static void TreeModelDebug_sub(const Glib::RefPtr<Gtk::TreeModel> &tm, unsigned indent,
				Gtk::TreeModel::Children c)
{  for (Gtk::TreeModel::iterator i=c.begin();i!=c.end();++i)
   {  std::cout << std::string(indent,' ') 
   		<< "Node " << tm->get_path(i).to_string()
   		<< " children=" << i->children().size() << '\n';
      TreeModelDebug_sub(tm,indent+1,i->children());
   }
}

void TreeModelDebug(const Glib::RefPtr<Gtk::TreeModel> &tm)
{  std::cout << "TreeModel @" << tm->gobj() << " flags=" << tm->get_flags()
	<< " columns=" << tm->get_n_columns() << '\n';
   std::cout << " column types=(";
   for (unsigned i=0;i<tm->get_n_columns();++i)
      std::cout << tm->get_column_type(i) << ',';
   std::cout << ")\n";
   TreeModelDebug_sub(tm,1,tm->children());
}
