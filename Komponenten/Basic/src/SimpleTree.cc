// $Id: SimpleTree.cc,v 1.85 2005/12/06 07:18:59 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include "config.h"
#include <libintl.h>
#include <Misc/i18n.h>
#include <SimpleTree.hh>
#include <Misc/itos.h>
#include <gtkmm/menu.h>
#include <bvector_item_CheckMenuItem.hh>
#include <bool_CheckMenuItem.hh>
#include <CellRendererSimpleTree.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#  include <sigc++/bind.h>
#endif
#include <iostream>

#define FIRST_COLUMN 1

void SimpleTree_Basic::detach()
{  set_model(Glib::RefPtr<Gtk::TreeModel>());
}

static void aufklappen(SimpleTree_Basic *tv,Gtk::TreeModel::Path path,
			const Gtk::TreeModel::Children &ch, int depth)
{  path.down();
   for (Gtk::TreeModel::iterator i=ch.begin();i!=ch.end();++i,path.next())
   {  if (!i->children().empty() && (*i)[tv->getStore()->m_columns.childrens_deep]<=depth)
      {  if (!tv->expand_row(path,false) && !tv->row_expanded(path))
            std::cerr << "aufklappen von " << path.to_string() << " schlug fehl\n";
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

//void SimpleTree_Basic::on_redisplay()
//{  detach(); attach();
//}

void SimpleTree_Basic::init()
{  on_spalten_geaendert();

   get_selection()->signal_changed().connect(SigC::slot(*this,&SimpleTree_Basic::on_selection_changed));
   // stammt eigentlich aus Prefs_proxy   
   getStore()->signal_column_changed().connect(SigC::slot(*this,&SimpleTree_Basic::on_column_changed));
   // Store
   getStore()->signal_please_detach().connect(SigC::slot(*this,&SimpleTree_Basic::detach));
   getStore()->signal_please_attach().connect(SigC::slot(*this,&SimpleTree_Basic::attach));
   getStore()->signal_spalten_geaendert().connect(SigC::slot(*this,&SimpleTree_Basic::on_spalten_geaendert));
   fillMenu();
   signal_button_press_event().connect(SigC::slot(*this,&SimpleTree_Basic::MouseButton),false);
}

SimpleTree_Basic::SimpleTree_Basic(unsigned maxcol)
	: SimpleTreeStore_Proxy(maxcol), button_press_vfunc(), menu()
{ init();
}

SimpleTree_Basic::SimpleTree_Basic(SimpleTreeModel_Properties &props)
	: SimpleTreeStore_Proxy(props), button_press_vfunc(), menu()
{ init();
}

SimpleTree_Basic::~SimpleTree_Basic()
{  if (menu) delete menu;
}

void SimpleTree_Basic::on_column_edited(const Glib::ustring &path,const Glib::ustring&new_text,unsigned idx)
{  const Gtk::TreeRow row=*getTreeModel()->get_iter(Gtk::TreeModel::Path(path));
   if (!row) return;
   cH_RowDataBase rdb=row[getStore()->m_columns.leafdata];
   // 2do: think about optimizing if nothing changes (unlikely)
   getModel().about_to_change(rdb);
   bool changed=false;
   getModel().signal_value_changed()(rdb,idx,new_text,changed);
   getModel().has_changed(rdb);
}

void SimpleTree_Basic::on_spalten_geaendert()
{  remove_all_columns();
#if FIRST_COLUMN==1 // hide normal tree expanders
   append_column(*Gtk::manage(new Gtk::TreeViewColumn("tree")));
//   get_column(0)->set_visible(false);
   get_column(0)->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
   get_column(0)->set_fixed_width(1);
#endif
   for (unsigned int i=0;i<Cols();++i)
   {
#if 1
      CellRendererSimpleTree *crst = Gtk::manage(new CellRendererSimpleTree(i));
      Gtk::TreeView::Column* pColumn = Gtk::manage(new Gtk::TreeView::Column(getColTitle(i),*crst));
      pColumn->signal_clicked().connect(SigC::bind(SigC::slot(*this,&SimpleTree_Basic::on_title_clicked),i));
      pColumn->add_attribute(crst->property_text(),sts->m_columns.cols[i]);
      if (getStore()->OptionColor().Value())
         pColumn->add_attribute(crst->property_background_gdk(),sts->m_columns.background);
      pColumn->add_attribute(crst->property_childrens_deep(),sts->m_columns.childrens_deep);

      unsigned idx(IndexFromColumn(i));
      pColumn->set_alignment(Properties().Alignment(idx));
      crst->property_xalign()=Properties().Alignment(idx);
      pColumn->set_sizing(Properties().get_sizing(idx));
      if (Properties().get_sizing(idx)==Gtk::TREE_VIEW_COLUMN_FIXED)
        pColumn->set_fixed_width(Properties().get_fixed_width(idx));
      pColumn->set_resizable(Properties().resizeable(idx));
      
      if (Properties().editable(idx))
      {  crst->property_editable()=true;
         crst->signal_edited().connect(SigC::bind(SigC::slot(*this,&SimpleTree_Basic::on_column_edited),idx));
      }
      append_column(*pColumn);
#else
      append_column(getColTitle(i),sts->m_columns.cols[i]);
#endif      
   }
// (gtk_tree_view_set_headers_clickable): assertion `tree_view->priv->model != NULL'
   set_headers_clickable(); 
}

void SimpleTree_Basic::on_title_clicked(unsigned nr)
{  unsigned idx=getStore()->currseq[nr];
   sequence_t::iterator i=std::find(clicked_seq.begin(),clicked_seq.end(),idx);
   if (i==clicked_seq.end())
   {  clicked_seq.push_back(idx);
      // if alles voll -> umsortieren
      if (clicked_seq.size()==Cols()) on_neuordnen_clicked();
      else 
	{
	 std::string tmptit=std::string("(")+itos(clicked_seq.size())+")";
	 if(get_column(nr+FIRST_COLUMN)->get_title().size()>=3)
	   tmptit+=get_column(nr+FIRST_COLUMN)->get_title().substr(3);
	 get_column(nr+FIRST_COLUMN)->set_title(tmptit);
	
	}
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
         get_column(i+FIRST_COLUMN)->set_title(getColTitle(i));
}

void SimpleTree_Basic::on_zuruecksetzen_clicked()
{  // bool alte_reihenfolge;
   clicked_seq.clear();
   on_neuordnen_clicked();
}

void SimpleTree_Basic::on_neuordnen_clicked()
{  if (!clicked_seq.empty()) getStore()->ShowDeep()=clicked_seq.size()-1;
   getStore()->fillSequence(clicked_seq);
   std::vector<cH_RowDataBase> 
       selection=dynamic_cast<SimpleTree*>(this)->getSelectedRowDataBase_vec();
   if (selection.empty()) 
       selection=dynamic_cast<SimpleTree*>(this)->getSelectedRowDataBase_vec(true);
   getStore()->setSequence(clicked_seq);
   for (std::vector<cH_RowDataBase>::const_iterator i=selection.begin();
         i!=selection.end();++i)
   { Gtk::TreeModel::Path path=getStore()->getPath(*i);
     if (!path.empty())
     { if (i==selection.begin()) scroll_to_row(path);
       expand_to_path(path);
       get_selection()->select(path);
     }
   }
   clicked_seq.clear();
   getStore()->save_remembered();
}

void SimpleTree_Basic::on_column_changed(guint nr)
{  delete menu; 
   menu=0;
   fillMenu();
   on_spalten_geaendert();
}

void SimpleTree_Basic::sel_change_cb(const Gtk::TreeModel::iterator&it,
		std::vector<cH_RowDataBase> *l,std::vector<Handle<const TreeRow> > *n)
{  Gtk::TreeRow row=*it;
   if (!row[getStore()->m_columns.childrens_deep])
      l->push_back(row[getStore()->m_columns.leafdata]);
   else
   {  Handle<TreeRow> htr=row[getStore()->m_columns.row];
      if (htr) n->push_back(Handle<const TreeRow>(htr));
//std::cerr << &*htr << '\n';
   }
}

void SimpleTree_Basic::on_selection_changed()
{
   if (get_selection()->get_selected_rows().empty()) // !sel) // unselect
     _leaf_unselected();
   else
   {  std::vector<cH_RowDataBase> leaves;
      std::vector<Handle<const TreeRow> > nodes;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
      get_selection()->selected_foreach_iter(sigc::bind(sigc::mem_fun(*this,
      		&SimpleTree_Basic::sel_change_cb),&leaves,&nodes));
#else
      get_selection()->selected_foreach(SigC::bind(SigC::slot(*this,
      		&SimpleTree_Basic::sel_change_cb),&leaves,&nodes));
#endif
      for (std::vector<cH_RowDataBase>::const_iterator i=leaves.begin();
      		i!=leaves.end();++i)
      	 _leaf_selected(*i);
      for (std::vector<Handle<const TreeRow> >::const_iterator i=nodes.begin();
      		i!=nodes.end();++i)
      	 _node_selected(*i);
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

cH_RowDataBase SimpleTree::getFirstSelection() const throw()
{  if (!get_selection()->count_selected_rows()) return cH_RowDataBase();
   Gtk::TreeModel::Path path=*(get_selection()->get_selected_rows().begin());
   Gtk::TreeModel::iterator i=const_cast<SimpleTree*>(this)->getStore()->get_iter(path);
   return (*i)[getStore()->m_columns.leafdata];
}


cH_RowDataBase SimpleTree::getCursorRowDataBase() const
// actually it does not throw multipleRowsSelected
	throw(noRowSelected,multipleRowsSelected,notLeafSelected)
{  Gtk::TreeModel::Path path;
   Gtk::TreeViewColumn *col=0;
   const_cast<SimpleTree*>(this)->get_cursor(path,col);
   if (!path.empty())
   {  const Gtk::TreeRow row=*const_cast<SimpleTree*>(this)->getTreeModel()->get_iter(path);
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
       return Handle<const TreeRow>(static_cast<Handle<TreeRow> >(row[getStore()->m_columns.row]));
     else
       throw notNodeSelected();
   }
   else throw noNodeSelected(); // oder multipleRowsSelected()
}

void SimpleTree_Basic::Expand_recursively()
{  expand_all();
}

void SimpleTree_Basic::Collapse()
{  collapse_all();
}

// Pivot element might use expand_to_path

// expand_row(path,false)/collapse_row(path)

void SimpleTree::getSelectedRowDataBase_vec_cb(const Gtk::TreeModel::iterator&it, 
		std::vector<cH_RowDataBase> *res, bool include_nodes)
{ Gtk::TreeRow row=*it;
  if (!row[getStore()->m_columns.childrens_deep])
      res->push_back(row[getStore()->m_columns.leafdata]);
  else if (include_nodes)
      res->push_back(row[getStore()->m_columns.leafdata]);
}

std::vector<cH_RowDataBase> SimpleTree::getSelectedRowDataBase_vec(bool include_nodes) const throw()
{  std::vector<cH_RowDataBase> result;
   SimpleTree *non_const_this=const_cast<SimpleTree*>(this);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   get_selection()->selected_foreach_iter(sigc::bind(
   		sigc::mem_fun(*non_const_this,
      		&SimpleTree::getSelectedRowDataBase_vec_cb),&result,include_nodes));
#else
   non_const_this->get_selection()->selected_foreach(SigC::bind(
   		SigC::slot(*non_const_this,
      		&SimpleTree::getSelectedRowDataBase_vec_cb),&result,include_nodes));
#endif      		
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
  add_mitem(menu,_("Zurücksetzen"),SigC::slot(*this,&SimpleTree_Basic::on_zuruecksetzen_clicked));
  add_mitem(menu,_("Abbrechen"),SigC::slot(*this,&SimpleTree_Basic::on_abbrechen_clicked));
  add_mitem(menu,_("Neuordnen"),SigC::slot(*this,&SimpleTree_Basic::on_neuordnen_clicked));

  Gtk::MenuItem *spalten=add_mitem(menu,_("Sichtbare Spalten"));
  Gtk::Menu *spalten_menu=manage(new Gtk::Menu);
  spalten->set_submenu(*spalten_menu);
  Gtk::TearoffMenuItem *tomi=manage(new Gtk::TearoffMenuItem());
  spalten_menu->append(*tomi);
  tomi->show();
   for (guint i=0;i<getStore()->MaxCol();++i)
      add_mitem(spalten_menu,Properties().Title(i),getStore()->ShowColumn(i));

  Gtk::MenuItem *optionen=add_mitem(menu,_("Optionen"));
  Gtk::Menu *optionen_menu=manage(new Gtk::Menu);
//  add_mitem(optionen_menu,_("Spaltenüberschriften anzeigen"))->set_sensitive(false); // Model
  add_mitem(optionen_menu,_("Auffüllen mit Standardreihenfolge\n"
	  	"(statt der aktuellen)"),getStore()->OptionAuffullen());
  add_mitem(optionen_menu,_("Ausgewählte Spalten aufklappen"),getStore()->OptionExpandieren());
  add_mitem(optionen_menu,_("Tiefe farblich markieren"),getStore()->OptionColor());

  optionen->set_submenu(*optionen_menu);
  add_mitem(menu,_("Alles aufklappen"),SigC::slot(*this,&SimpleTree_Basic::Expand_recursively));
  add_mitem(menu,_("Alles zuklappen"),SigC::slot(*this,&SimpleTree_Basic::Collapse));
  
  // separator
  for (std::list<std::pair<sigc::signal0<void>,Glib::ustring> >::iterator i=user_menuitems.begin();
        i!=user_menuitems.end();++i)
  { add_mitem(menu,i->second,i->first.make_slot());
  }
}

bool SimpleTree_Basic::MouseButton(GdkEventButton *event)
{  Gtk::TreeModel::Path path;
   Gtk::TreeViewColumn *col(0);
   int cell_x(0),cell_y(0);
   bool res=get_path_at_pos(int(event->x),int(event->y),path,col,cell_x,cell_y);

   if (event->type == GDK_BUTTON_PRESS && event->button==1 && button_press_vfunc
       && event->window ==get_bin_window()->gobj())
   {  if (!res || !col) return false;
      Gtk::TreeModel::iterator it=getTreeModel()->get_iter(path);
      if (!it) return false;
      if ((*it)[getStore()->m_columns.childrens_deep]) return false; // node
      int idx=-1;
      for (unsigned colno=0;colno<=Cols();++colno)
         if (col==get_column(colno)) 
         {  idx=getStore()->currseq[colno];
            break;
         }
      return (*button_press_vfunc)(this,(*it)[getStore()->m_columns.leafdata],idx);
   }
   if (event->type == GDK_BUTTON_PRESS && event->button==3  && menu)
   {  if (res)
      { Gtk::TreeModel::iterator it=getTreeModel()->get_iter(path);
        if (!!it) 
          menuContext=(*it)[getStore()->m_columns.leafdata];
        else menuContext=cH_RowDataBase();
      }
      else menuContext=cH_RowDataBase();
      menu->popup(event->button,event->time);
      return true;
   }
   return false;
}

#ifdef ST_DEPRECATED
void SimpleTree_Basic::setTitles(const std::vector<std::string>& T)
{  SimpleTreeStore_Proxy::setTitles(T);
   delete menu; menu=0;
   fillMenu();
}
#endif

void SimpleTree::ScrollToSelection()
{ Glib::RefPtr<Gtk::TreeSelection> sel=get_selection();
  if (sel->count_selected_rows())
  { Gtk::TreeModel::Path path=*(sel->get_selected_rows().begin());
    expand_to_path(path);
    scroll_to_row(path,0.5);
  }
}

void SimpleTree::scroll_to(const cH_RowDataBase &data, gfloat where)
{ Gtk::TreeModel::Path path=getStore()->getPath(data);
  if (!path.empty())
  { expand_to_path(path);
    if (where<0) scroll_to_row(path);
    else scroll_to_row(path,where);
  }
}

void SimpleTree_Basic::setAlignment(const std::vector<gfloat> &A)
{  sts->setAlignment(A);
}

void SimpleTree_Basic::setResizeable(const std::vector<bool> &R)
{ sts->setResizeable(R);
}

void SimpleTree_Basic::setResizeable(const bool b)
{
 SimpleTree_Basic::setResizeable(std::vector<bool>(sts->MaxCol(),b));
}

static void sig0(const char * const x)
{ std::cerr << x << "()\n";
}

static void sig1i(int i, const char * const x)
{ std::cerr << x << "(" << i << ")\n";
}

static void sig1r(cH_RowDataBase r, const char * const x)
{ std::cerr << x << "(" << &r << ")\n";
}

static void sig2pi(const Gtk::TreeModel::Path&p,const Gtk::TreeModel::iterator&i, const char * const x)
{ std::cerr << x << "(" << p.to_string() << "," << i.get_stamp() << "," << i.gobj()->user_data << ")\n";
}

static void sig1p(const Gtk::TreeModel::Path&p, const char * const x)
{ std::cerr << x << "(" << p.to_string() << ")\n";
}

static void sig3piI(const Gtk::TreeModel::Path&p,const Gtk::TreeModel::iterator&i, int *I, const char * const x)
{ std::cerr << x << "(" << p.to_string() << "," << i.get_stamp() << "," << i.gobj()->user_data << "," << I << ")\n";
}

void SimpleTree_Basic::debug()
{ getStore()->signal_please_detach().connect(SigC::bind(&sig0,"please_detach"));
  getStore()->signal_please_attach().connect(SigC::bind(&sig0,"please_attach"));
  getStore()->signal_spalten_geaendert().connect(SigC::bind(&sig0,"spalten_geaendert"));
  getStore()->signal_column_changed().connect(SigC::bind(&sig1i,"column_changed"));
  getModel().signal_line_appended().connect(SigC::bind(&sig1r,"line_appended"));
  getModel().signal_line_to_remove().connect(SigC::bind(&sig1r,"line_to_remove"));
  getModel().signal_please_detach().connect(SigC::bind(&sig0,"Model::please_detach"));
  getModel().signal_please_attach().connect(SigC::bind(&sig0,"Model::please_attach"));
  getStore()->signal_row_changed().connect(SigC::bind(&sig2pi,"row_changed"));
  getStore()->signal_row_inserted().connect(SigC::bind(&sig2pi,"row_inserted"));
  getStore()->signal_row_has_child_toggled().connect(SigC::bind(&sig2pi,"row_has_child_toggled"));
  getStore()->signal_row_deleted().connect(SigC::bind(&sig1p,"row_deleted"));
  getStore()->signal_rows_reordered().connect(SigC::bind(&sig3piI,"rows_reordered"));
}

// deprecated proxies
void SimpleTreeStore_Proxy::set_remember(const std::string &program, const std::string &instance) {  sts->set_remember(program,instance); }
void SimpleTreeStore_Proxy::setTitles(const std::vector<std::string> &T) {  sts->setTitles(T); }
void SimpleTreeStore_Proxy::setTitleAt(unsigned idx, const std::string &s) {  sts->setTitleAt(idx,s); }
void SimpleTreeStore_Proxy::set_NewNode(SimpleTreeStore::NewNode_fp n) { sts->set_NewNode(n); }
void SimpleTreeStore_Proxy::set_value_data(gpointer _p) { sts->set_value_data(_p); }

bool SimpleTree_Basic::clicked_impl(SimpleTree_Basic *_this, const cH_RowDataBase &row, int col_idx)
{  bool handled=false;
   _this->clicked_sig(row,col_idx,handled);
   return handled;
}

SigC::Signal3<void,const cH_RowDataBase &,int,bool&> 
	  &SimpleTree_Basic::signal_clicked()
{ button_press_vfunc=&SimpleTree_Basic::clicked_impl;
  return clicked_sig;
}

sigc::signal0<void> &SimpleTree_Basic::addMenuItem(Glib::ustring const& text)
{ std::list<int>::size_type idx=user_menuitems.size();
  user_menuitems.resize(idx+1);
  user_menuitems.back().second=text;
  if (menu)
  { add_mitem(menu,text,user_menuitems.back().first.make_slot());
  }
  return user_menuitems.back().first;
}

cH_RowDataBase SimpleTree::getMenuContext(bool prefer_selection) const
{ if (prefer_selection && get_selection()->count_selected_rows())
    return getFirstSelection();
  return menuContext;
}

void SimpleTree::select_leaves(Gtk::TreeModel::Path const& p)
{ Gtk::TreeModel::iterator it=get_model()->get_iter(p);
  Gtk::TreeModel::Children::size_type csz=it->children().size();
  if (csz)
  { Gtk::TreeModel::Path p2=p;
    p2.down();
    while (csz)
    { get_selection()->select(p2);
      select_leaves(p2);
      p2.next();
    }
  }
}

void SimpleTree::nodes_select_leaves()
{ Gtk::TreeSelection::ListHandle_Path ld=get_selection()->get_selected_rows();
  for (Gtk::TreeSelection::ListHandle_Path::const_iterator i=ld.begin();i!=ld.end();++i)
  { select_leaves(*i);
  }
}
