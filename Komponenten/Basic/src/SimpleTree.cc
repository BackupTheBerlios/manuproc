// $Id: SimpleTree.cc,v 1.14 2002/12/05 14:20:22 christof Exp $
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
#include <Misc/itos.h>

SimpleTree_Basic::SimpleTree_Basic(unsigned maxcol)
	: SimpleTreeStore_Proxy(maxcol)
{  set_model(sts.m_refTreeStore);

   for (unsigned int i=0;i<Cols();++i)
   {  append_column("",sts.m_columns.cols[i]);
      get_column(i)->signal_clicked().connect(SigC::bind(SigC::slot(*this,&SimpleTree_Basic::on_title_clicked),i));
   }
   set_headers_clickable();
   
   getStore().signal_title_changed().connect(SigC::slot(*this,&SimpleTree_Basic::on_title_changed));
   get_selection()->signal_changed().connect(SigC::slot(*this,&SimpleTree_Basic::on_selection_changed));
}

void SimpleTree_Basic::on_title_clicked(unsigned nr)
{  unsigned idx=getStore().currseq[nr];
   sequence_t::iterator i=std::find(clicked_seq.begin(),clicked_seq.end(),idx);
   if (i==clicked_seq.end())
   {  clicked_seq.push_back(idx);
      // if alles voll -> umsortieren
      if (clicked_seq.size()==Cols()) goto resort;
      get_column(nr)->set_title(itos(clicked_seq.size()));
   }
   else if (i==--clicked_seq.end())
   {  // umsortieren
      getStore().fillSequence(clicked_seq);
     resort:
      getStore().setSequence(clicked_seq);
      clicked_seq.clear();
   }
   else
   {  // abbrechen
      clicked_seq.clear();
      // Titel wiederherstellen
      for (unsigned i=0;i<Cols();++i) 
         get_column(i)->set_title(getColTitle(i));
   }
}

void SimpleTree_Basic::on_title_changed(guint nr)
{  get_column(nr)->set_title(getColTitle(nr));
}

void SimpleTree_Basic::on_selection_changed()
{  Gtk::TreeModel::iterator sel=get_selection()->get_selected();
   if (!sel) ; // unselect
   else
   {  Gtk::TreeRow row=*sel;
      if (!row[getStore().m_columns.childrens_deep])
         _leaf_selected(row[getStore().m_columns.leafdata]);
      else
      {  Handle<TreeRow> htr=row[getStore().m_columns.row];
         if (htr) _node_selected(*htr);
      }
   }
}

cH_RowDataBase SimpleTree::getSelectedRowDataBase() const
	throw(noRowSelected,multipleRowsSelected,notLeafSelected)
{  Gtk::TreeModel::iterator sel=const_cast<SimpleTree*>(this)
		->get_selection()->get_selected();
   if (sel)
   {  const Gtk::TreeRow row=*sel;
      if (!row[getStore().m_columns.childrens_deep])
         return row[getStore().m_columns.leafdata];
      else throw notLeafSelected();
   }
   else throw noRowSelected(); // oder multipleRowsSelected()
}
