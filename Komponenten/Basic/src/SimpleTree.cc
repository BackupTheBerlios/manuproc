// $Id: SimpleTree.cc,v 1.8 2002/12/03 08:44:30 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <SimpleTree.hh>

SimpleTree::SimpleTree(unsigned int cols,int attrs)
	: SimpleTreeStore_Proxy(cols,attrs)
{  set_model(sts.m_refTreeStore);

   for (unsigned int i=0;i<cols;++i)
      append_column("",sts.m_columns.cols[i]);
   getStore().signal_title_changed().connect(SigC::slot(*this,&SimpleTree::on_title_changed));
   get_selection().signal_changed().connect(SigC::slot(*this,&SimpleTree::on_selection_changed));
}

void SimpleTree::on_title_changed(guint nr)
{  get_column(nr)->set_title(getColTitle(nr));
}

void SimpleTree::on_selection_changed()
{  Gtk::TreeRow row=*(get_selection().get_selected());
   if (!row[getStore().m_columns.childrens_deep])
      _leaf_selected(row[getStore().m_columns.leafdata]);
   else
      _node_selected(*row[getStore().m_columns.row]);
}
