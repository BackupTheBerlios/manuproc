// $Id: SimpleTreeStore_test.cc,v 1.2 2004/05/06 10:26:25 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <SimpleTreeStore.h>
#include <iostream>
#include <gtkmm/treepath.h> 

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION<2
void SimpleTreeStore::test_sub(unsigned indent,const GtkTreeIter *i,const GtkTreeIter *parent)
{  GtkTreeIter i2=*i;
loop:
   GtkTreeIter it;
   std::cout << std::string(indent,' ') << "Node @" << i2.user_data
	<< " has_child=" << iter_has_child_vfunc(&i2);
   if (!iter_parent_vfunc(&it,&i2))
   {  if (parent) std::cerr << " E:no_parent";
   }
   else
   {  if (parent->user_data != it.user_data) std::cerr << " E:parent_mism";
   }
//   if (
   if (iter_has_child_vfunc(&i2))
   {  std::cout << " n_children=" << iter_n_children_vfunc(&i2);
      if (!iter_children_vfunc(&it,&i2)) std::cerr << " E:children\n";
      else 
      {  std::cout << '\n'; 
         test_sub(indent+1,&it,&i2);
      }
   }
   else std::cout << '\n';
   if (iter_next_vfunc(&i2)) goto loop;
}

void SimpleTreeStore::test()
{  std::cout << "flags=" << get_flags() << " n_columns=" << get_n_columns() << '\n';
   for (int i=0;i<get_n_columns();++i) std::cout << "[" << i << "]=" << get_column_type(i) << ' ';
   std::cout << '\n';
   GtkTreeIter it;
   if (get_iter_vfunc(&it,Gtk::TreeModel::Path("0")))
      test_sub(1,&it,0);
}
#endif
