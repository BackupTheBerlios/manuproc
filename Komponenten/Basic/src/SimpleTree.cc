// $Id: SimpleTree.cc,v 1.38 2003/12/17 16:37:39 christof Exp $
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
#include <gtkmm/menu.h>
#include <bvector_item_CheckMenuItem.hh>
#include <bool_CheckMenuItem.hh>

void SimpleTree_Basic::detach()
{  set_model(Glib::RefPtr<Gtk::TreeModel>());
}

static void aufklappen(SimpleTree_Basic *tv,Gtk::TreeModel::Path path,
			const Gtk::TreeModel::Children &ch, int depth)
{  path.down();
   for (Gtk::TreeModel::iterator i=ch.begin();i!=ch.end();++i,path.next())
   {  if (!i->children().empty() && (*i)[tv->getStore()->m_columns.childrens_deep]<=depth)
      {  assert(tv->expand_row(path,false));
         if ((*i)[tv->getStore()->m_columns.childrens_deep]<depth) aufklappen(tv,path,i->children(),depth);
      }
   }
}

void SimpleTree_Basic::attach()
{  set_model(sts);
   // aufklappen
   if (sts->expandieren_bool && sts->ShowDeep().Value())
   {  aufklappen(this,Gtk::TreeModel::Path(),get_model()->children(),sts->ShowDeep().Value());
   }
}

void SimpleTree_Basic::on_redisplay()
{  detach(); attach();
}

SimpleTree_Basic::SimpleTree_Basic(unsigned maxcol)
	: SimpleTreeStore_Proxy(maxcol), menu()
{  on_spaltenzahl_geaendert();
   
   getStore()->signal_title_changed().connect(SigC::slot(*this,&SimpleTree_Basic::on_title_changed));
   get_selection()->signal_changed().connect(SigC::slot(*this,&SimpleTree_Basic::on_selection_changed));
   getStore()->signal_redisplay().connect(SigC::slot(*this,&SimpleTree_Basic::on_redisplay));
   fillMenu();
   signal_button_press_event().connect(SigC::slot(*this,&SimpleTree_Basic::MouseButton),false);
   getStore()->signal_spaltenzahl_geaendert().connect(SigC::slot(*this,&SimpleTree_Basic::on_spaltenzahl_geaendert));
}

SimpleTree_Basic::~SimpleTree_Basic()
{  if (menu) delete menu;
}

void SimpleTree_Basic::on_spaltenzahl_geaendert()
{  remove_all_columns();
   for (unsigned int i=0;i<Cols();++i)
   {  Gtk::CellRendererText *crt = Gtk::manage(new Gtk::CellRendererText());
      append_column(getColTitle(i), *crt);
      Gtk::TreeViewColumn* pColumn = get_column(i);
      if (pColumn)
      {  pColumn->signal_clicked().connect(SigC::bind(SigC::slot(*this,&SimpleTree_Basic::on_title_clicked),i));
         pColumn->add_attribute(crt->property_text(),sts->m_columns.cols[i]);
         if (getStore()->OptionColor().Value())
            pColumn->add_attribute(crt->property_background_gdk(),sts->m_columns.background);
      }
   }
   on_redisplay();
   set_headers_clickable();
}

void SimpleTree_Basic::on_title_clicked(unsigned nr)
{  unsigned idx=getStore()->currseq[nr];
   sequence_t::iterator i=std::find(clicked_seq.begin(),clicked_seq.end(),idx);
   if (i==clicked_seq.end())
   {  clicked_seq.push_back(idx);
      // if alles voll -> umsortieren
      if (clicked_seq.size()==Cols()) on_neuordnen_clicked();
      else get_column(nr)->set_title(itos(clicked_seq.size()));
   }
   else if (i==--clicked_seq.end())
   {  // umsortieren
      on_neuordnen_clicked();
   }
   else
   {  // abbrechen
      on_abbrechen_clicked();
   }
}

void SimpleTree_Basic::on_abbrechen_clicked()
{  
      clicked_seq.clear();
      // Titel wiederherstellen
      for (unsigned i=0;i<Cols();++i) 
         get_column(i)->set_title(getColTitle(i));
}

void SimpleTree_Basic::on_zuruecksetzen_clicked()
{  // bool alte_reihenfolge;
   clicked_seq.clear();
   on_neuordnen_clicked();
}

void SimpleTree_Basic::on_neuordnen_clicked()
{  if (!clicked_seq.empty()) getStore()->ShowDeep()=clicked_seq.size()-1;
   getStore()->fillSequence(clicked_seq);
   getStore()->setSequence(clicked_seq);
   clicked_seq.clear();
   getStore()->save_remembered();
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


Handle<const TreeRow> SimpleTree::getSelectedNode() const 
 	throw(noNodeSelected,multipleNodesSelected,notNodeSelected)
{
  Gtk::TreeModel::iterator sel=const_cast<SimpleTree*>(this)
		->get_selection()->get_selected();
   if (sel)
   { const Gtk::TreeRow &row=*sel;  
     if (row[getStore()->m_columns.childrens_deep])
       return static_cast<Handle<TreeRow> >(row[getStore()->m_columns.row]);
     else
       throw notNodeSelected();
   }
   else throw noRowSelected(); // oder multipleRowsSelected()
}

void SimpleTree_Basic::Expand_recursively()
{  expand_all();
}

void SimpleTree_Basic::Collapse()
{  collapse_all();
}

// Pivot element might use expand_to_path

// expand_row(path,false)/collapse_row(path)

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


static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text,const Model_ref<bvector_item> &model)
{  bvector_item_CheckMenuItem *it = 
	manage(new bvector_item_CheckMenuItem(model,text));
   m->append(*it);
   it->show();
   return it;
}

static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text,const Model_ref<bool> &model)
{  bool_CheckMenuItem *it = manage(new bool_CheckMenuItem(model,text));
   m->append(*it);
   it->show();
   return it;
}

static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text)
{  Gtk::MenuItem *it=manage(new class Gtk::MenuItem(text));
   m->append(*it);
   it->show();
   return it;
}

static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text,const SigC::Slot0<void> &callback)
{  Gtk::MenuItem *it=add_mitem(m,text);
   it->signal_activate().connect(callback);
   return it;
}

void SimpleTree_Basic::fillMenu()
{  assert(menu==0); 
  menu=new Gtk::Menu();
  // Hauptmenü
  add_mitem(menu,"Zurücksetzen",SigC::slot(*this,&SimpleTree_Basic::on_zuruecksetzen_clicked));
  add_mitem(menu,"Abbrechen",SigC::slot(*this,&SimpleTree_Basic::on_abbrechen_clicked));
  add_mitem(menu,"Neuordnen",SigC::slot(*this,&SimpleTree_Basic::on_neuordnen_clicked));

  Gtk::MenuItem *spalten=add_mitem(menu,"Sichtbare Spalten");
  Gtk::Menu *spalten_menu=manage(new Gtk::Menu);
  spalten->set_submenu(*spalten_menu);
  Gtk::TearoffMenuItem *tomi=manage(new Gtk::TearoffMenuItem());
  spalten_menu->append(*tomi);
  tomi->show();
   for (guint i=0;i<Cols();++i)
      add_mitem(spalten_menu,getColTitle(i),getStore()->ShowColumn(i));

  Gtk::MenuItem *optionen=add_mitem(menu,"Optionen");
  Gtk::Menu *optionen_menu=manage(new Gtk::Menu);
  add_mitem(optionen_menu,"Spaltenüberschriften anzeigen")->set_sensitive(false); // Model
  add_mitem(optionen_menu,"Auffüllen mit Standardreihenfolge\n"
	  	"(statt der aktuellen)",getStore()->OptionAuffullen());
  add_mitem(optionen_menu,"Ausgewählte Spalten aufklappen",getStore()->OptionExpandieren());
  add_mitem(optionen_menu,"Tiefe farblich markieren",getStore()->OptionColor());

  optionen->set_submenu(*optionen_menu);
  add_mitem(menu,"Alles aufklappen",SigC::slot(*this,&SimpleTree_Basic::Expand_recursively));
  add_mitem(menu,"Alles zuklappen",SigC::slot(*this,&SimpleTree_Basic::Collapse));
}

bool SimpleTree_Basic::MouseButton(GdkEventButton *event)
{  
   if (event->type == GDK_BUTTON_PRESS && event->button==3  && menu)
   {  menu->popup(event->button,event->time);
      return true;
   }
   return false;
}

void SimpleTree_Basic::setTitles(const std::vector<std::string>& T)
{  SimpleTreeStore_Proxy::setTitles(T);
   delete menu; menu=0;
   fillMenu();
}

void SimpleTree::ScrollToSelection()
{  Glib::RefPtr<Gtk::TreeSelection> sel=get_selection();
   if (!sel->get_selected())
      scroll_to_cell(
      	get_model()->get_path(sel->get_selected()),
      	*get_column(0),0.5,0.0);
}
