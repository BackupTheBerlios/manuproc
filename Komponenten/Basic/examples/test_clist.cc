// $Id: test_clist.cc,v 1.2 2004/07/02 14:36:04 christof Exp $
/*  ManuProcWidgets: ManuProC's GUI element library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/treeview.h>
#include <TreeViewUtility.h>

struct testwin : Gtk::Window
{  Gtk::TreeView gtklist;
   TreeViewUtility::CListEmulator emu;

  testwin() : emu("column")
  {  add(gtklist);
     emu.attach_to(gtklist);
     gtklist.show();
     std::vector<Glib::ustring> vec;
     vec.push_back("test");
     emu.append(vec);
  }
};

int main(int argc,char ** argv)
{  Gtk::Main m(argc,argv);
   
   Gtk::Window *w=new testwin;
   m.run(*w);
   return 0;
}
