// $Id: SimpleTree.cc,v 1.21 2003/10/21 07:41:41 christof Exp $
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

void SimpleTree_Basic::detach()
{  set_model(Glib::RefPtr<Gtk::TreeModel>());
}

void SimpleTree_Basic::attach()
{  set_model(sts);
}

void SimpleTree_Basic::on_redisplay()
{  detach(); attach();
}

SimpleTree_Basic::SimpleTree_Basic(unsigned maxcol)
	: SimpleTreeStore_Proxy(maxcol), menu()
{  attach();

   for (unsigned int i=0;i<Cols();++i)
   {  Gtk::CellRendererText *crt = Gtk::manage(new Gtk::CellRendererText());
      append_column("", *crt);
      Gtk::TreeViewColumn* pColumn = get_column(i);
      if (pColumn)
      {  pColumn->signal_clicked().connect(SigC::bind(SigC::slot(*this,&SimpleTree_Basic::on_title_clicked),i));
         pColumn->add_attribute(crt->property_text(),sts->m_columns.cols[i]);
         if (getStore()->ShowColor().Value())
            pColumn->add_attribute(crt->property_background_gdk(),sts->m_columns.background);
      }
   }
   set_headers_clickable();
   
   getStore()->signal_title_changed().connect(SigC::slot(*this,&SimpleTree_Basic::on_title_changed));
   get_selection()->signal_changed().connect(SigC::slot(*this,&SimpleTree_Basic::on_selection_changed));
   getStore()->signal_redisplay().connect(SigC::slot(*this,&SimpleTree_Basic::on_redisplay));
}

void SimpleTree_Basic::on_title_clicked(unsigned nr)
{  unsigned idx=getStore()->currseq[nr];
   sequence_t::iterator i=std::find(clicked_seq.begin(),clicked_seq.end(),idx);
   if (i==clicked_seq.end())
   {  clicked_seq.push_back(idx);
      // if alles voll -> umsortieren
      if (clicked_seq.size()==Cols()) goto resort;
      get_column(nr)->set_title(itos(clicked_seq.size()));
   }
   else if (i==--clicked_seq.end())
   {  // umsortieren
      getStore()->fillSequence(clicked_seq);
     resort:
      getStore()->setSequence(clicked_seq);
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
   if (!sel) // unselect
     _leaf_unselected();
   else
   {  Gtk::TreeRow row=*sel;
      if (!row[getStore()->m_columns.childrens_deep])
         _leaf_selected(row[getStore()->m_columns.leafdata]);
      else
      {  Handle<TreeRow> htr=row[getStore()->m_columns.row];
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
      if (!row[getStore()->m_columns.childrens_deep])
         return row[getStore()->m_columns.leafdata];
      else throw notLeafSelected();
   }
   else throw noRowSelected(); // oder multipleRowsSelected()
}

void SimpleTree::Expand_recursively()
{
#warning 2do expand
}

std::vector<cH_RowDataBase> SimpleTree::getSelectedRowDataBase_vec() const
	throw (SimpleTree::notLeafSelected)
{  std::vector<cH_RowDataBase> result;
   try {
#warning 2do multiple rows
      result.push_back(getSelectedRowDataBase());
   } catch (notLeafSelected &e) { throw; }
     catch (...) {}
   return result;
}

#include <gtkmm/menu.h>
#include <bool_CheckMenuItem.hh>

void SimpleTree_Basic::fillMenu()
{  assert(menu==0); 
  menu=new Gtk::Menu();
  // Hauptmenü        
#if 0
   Gtk::MenuItem *neuordnen = manage(new class Gtk::MenuItem("Neuordnen"));
   Gtk::MenuItem *zuruecksetzen = manage(new class Gtk::MenuItem("Zurücksetzen"));
   Gtk::MenuItem *abbrechen = manage(new class Gtk::MenuItem("Abbrechen"));
   Gtk::Menu *spalten_menu = manage(new class Gtk::Menu());
   Gtk::MenuItem *spalten = manage(new class Gtk::MenuItem("Sichtbare Spalten"));
   Gtk::Menu *optionen_menu = manage(new class Gtk::Menu());
   Gtk::MenuItem *optionen = manage(new class Gtk::MenuItem("Optionen"));

   menu->append(*zuruecksetzen);     
   menu->append(*abbrechen);   
   menu->append(*spalten);
   spalten->set_submenu(*spalten_menu);

   for (guint i=0;i<Cols();++i)
    {
      Gtk::CheckMenuItem *sp = manage(new class Gtk::CheckMenuItem(getColTitle(i)));
      spalten_menu->append(*sp);
      if (vec_hide_cols[i]) sp->set_active(true);
      sp->show();
      sp->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::welche_Spalten),i,sp));
    }
   menu->append(*optionen);
   optionen->set_submenu(*optionen_menu);
   Gtk::CheckMenuItem *titles = manage(new class Gtk::CheckMenuItem("Spaltenüberschriften anzeigen"));
   Gtk::CheckMenuItem *auffuellen = manage(new class Gtk::CheckMenuItem("Auffüllen mit Standardreihenfolge\n(statt der aktuellen)"));
   Gtk::CheckMenuItem *expandieren = manage(new class Gtk::CheckMenuItem("Gewählte Knoten expandieren"));
   Gtk::CheckMenuItem *colorize = manage(new class Gtk::CheckMenuItem("farblich markieren"));
   Gtk::MenuItem *exp_all = manage(new class Gtk::MenuItem("Alle Knoten expandieren"));
   Gtk::MenuItem *col_all = manage(new class Gtk::MenuItem("Alle Knoten kollabieren"));
   optionen_menu->append(*titles);
   optionen_menu->append(*auffuellen);
   optionen_menu->append(*expandieren);
   optionen_menu->append(*colorize);
   optionen_menu->append(*exp_all);
   optionen_menu->append(*col_all);
   titles->show();
   auffuellen->show();
   expandieren->show();
   colorize->show();
   
   neuordnen->activate.connect(SigC::slot(this,&TreeBase::on_neuordnen_clicked));
   zuruecksetzen->activate.connect(SigC::slot(this,&TreeBase::on_zuruecksetzen_clicked));
   abbrechen->activate.connect(SigC::slot(this,&TreeBase::on_abbrechen_clicked));

   titles_menu=titles;
   titles->set_active(titles_bool);
   titles->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::Titles),titles));

   auffuellen->set_active(auffuellen_bool);
   auffuellen->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::Auffuellen),auffuellen));

   expandieren->set_active(expandieren_bool);
   expandieren->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::Expandieren),expandieren));

   colorize->set_active(color_bool);
   colorize->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::on_Color),colorize));

   exp_all->activate.connect(SigC::slot(this,&TreeBase::Expand_recursively));
   col_all->activate.connect(SigC::slot(this,&TreeBase::Collapse_recursively));

   // Menu anzeigen
//   neuordnen->show();
//   zuruecksetzen->show();
//   abbrechen->show();
//   spalten->show();
//   optionen->show();
   menu->show_all();
#endif
}
