/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include"treebase.h"
#include"rowdata.h"
#include"tclistnode.h"
#include"tclistleaf.h"
#include<Aux/itos.h>
#include<typeinfo>
#include <algorithm>

// these macros are a lot faster than x.size() because that needs a division
#define KeinKind(x) ((x).begin()==(x).end())
#define NurEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) != (x).end())

void TreeBase::clear()
{
  datavec.clear();
  vec_hide_cols.clear();
  TCList::clear();
}

const std::string TreeBase::getColTitle(guint seq) const
{
 return "-";
}

void TreeBase::setSequence()
{  currseq.clear();
   for(guint i=0; i<attrcount; ++i) currseq.push_back(i);
}

void TreeBase::on_click_column(int col)
{
//std::cout << col<<"\t"<<currseq[col]<<"\t"<<Attrs()<<"\t"<<col_count<<"\n";
 if (col<(int)Attrs()) 
   {
    if (col_schon_ausgewaehlt(col)) on_neuordnen_clicked();
    else 
      {
       clicked_seq.push_back(currseq[col]);
       set_column_title(col, itos(clicked_seq.size())); 
       if (NurEinKind(clicked_seq)) summen_knoepfe();
       if (expandieren_bool) showdeep = clicked_seq.size();
         else showdeep = 0;
       if (clicked_seq.size()==Attrs()) on_neuordnen_clicked(); 
      }
   }

 else // kann nur neu/abbrechen sein
 {  if (col==(int)Cols()-1) on_neuordnen_clicked();
    if (col==(int)Cols()-2) on_abbrechen_clicked();
 }
}

bool TreeBase::col_schon_ausgewaehlt(int col)
{
 std::deque<guint>::const_iterator i =clicked_seq.begin();
 while (i!=clicked_seq.end() && *i!=currseq[col]) ++i;
 if (i==clicked_seq.end()) return false;
// else { std::cout << "gewählt\n"; return true;}
 else  return true;
}

// der Name ist ja eher irreführend
void TreeBase::summen_knoepfe()
{
  if (Attrs()<Cols())
    set_column_title(Cols()-1, "Neuordnen"); 
  if (Attrs()<Cols()-1)
    set_column_title(Cols()-2, "Abbrechen"); 
}

TreeBase::TreeBase(guint cols, guint attr) : 
	TCList(cols), showdeep(0), attrcount(attr ? attr : cols),
	gp(0), menu(0),
	auffuellen_bool(false), expandieren_bool(true),stutzen_bool(true)
{
  this->button_press_event.connect(SigC::slot(this,&TreeBase::MouseButton));
  click_column.connect(SigC::slot(this,&TreeBase::on_click_column));
  select_row.connect(SigC::slot(this, &TreeBase::on_row_select));
  setSequence();
  vec_hide_cols.resize(Cols());
  for (std::vector<bool>::iterator i=vec_hide_cols.begin();i!=vec_hide_cols.end();++i)
    (*i) = true;
}

// We can't call these virtual functions in the ctor because 
// the virtual table of derived classes is not in place. So we have to
// call it afterwards. 
// If you call this function in your ctor and derive from your class 
// all kind of strange things will happen. You have been warned!

void TreeBase::init()
{ setColTitles();
//  fillMenu();
  fillTCL();
}

void TreeBase::setColTitles()
{
 std::deque<guint> seqtitle = currseq;
 int seqlen=seqtitle.size();

 while(!seqtitle.empty()) // Attribute
   { set_column_title(Attrs()-seqtitle.size(),getColTitle(seqtitle.front()));
     seqtitle.pop_front();
   }
 for(size_t i=seqlen;i<Cols();++i) // Summen
   { 
         set_column_title(i, getColTitle(i));
   }
 column_titles_show();
 if (!menu) fillMenu();
}


#define NEW_INSERT

bool TreeBase::stutzen(TCListRow_API *parent, TCListRow_API *we,
						TCList &tclist)
{
#ifndef NEW_INSERT
 if (NurEinKind(*we)==1)
 {  while (NurEinKind(*we)==1)
    {  TCListRow *child_and_brother_to_be= &* (we->begin());
       TCListRow *we_as_a_row=static_cast<TCListRow*>(we);

       we->reparent_children(*parent);
       // copy non-empty attribute cells
       for (int i=0;i<we->get_expanding_column();++i)
       {  const std::string t(we_as_a_row->get_text(i));
          if (t.size())
             child_and_brother_to_be->set_text(i,t);
       }
       tclist.erase(TCListRow_iterator(parent,static_cast<TCListRow*>(we)));
       we=child_and_brother_to_be;
    }
    return true;
 }

 if(MehrAlsEinKind(*we))
 {  for (TCListRow_API::iterator i = we->begin();i!=we->end();++i)
       if (stutzen(we,&*i,tclist)) 
          return true;
 }
#endif
  return false;
}

void TreeBase::fillTCL()
{
 fillDataVec();
 refillTCL();
}

TreeBase::~TreeBase()
{  if (menu) delete menu;
}

// true = gefunden
bool TreeBase::redisplay_recurse(TCListRow_API *a, const RowDataBase *r, guint col)
{  for (TCListRow_API::iterator i=a->begin();i!=a->end();++i)
   {  if ((*i).begin()!=(*i).end())
      {  if (redisplay_recurse(&*i,r,col)) return true;
      }
      const TreeRow *tlr=reinterpret_cast<const TreeRow *>((*i).get_user_data());
      if (tlr->Leaf())
      {  if (&*(tlr->LeafData()) == r)
         {  dynamic_cast<TCListRow&>(*i).relabel(col,r->Value(col,ValueData())->getStrVal());
            return true;
         }
      }
   }
   return false;
}

void TreeBase::redisplay(cH_RowDataBase row, guint index)
{  if (index<attrcount)
   {  int newindex=0;
      for (std::deque<guint>::const_iterator i=currseq.begin();
   		i!=currseq.end();++i,++newindex)
      {  if (*i==index) 
         { index=newindex; break; }
      }
   }
   redisplay_recurse(this,&*row,index);
}


void TreeBase::refillTCL(bool clear_me)
{
// liste loeschen
 collapse();
 if(clear_me) TCList::clear();

 std::vector<cH_RowDataBase>::const_iterator i=datavec.begin();
 std::vector<cH_RowDataBase>::const_iterator j=datavec.end();

// neu einordnen, Summen berechnen
 for(; i!=j; ++i)
    insertIntoTCL((TCListRow_API*)this,*this,*i,currseq,0);

// Aeste mit einem Blatt kuerzen 
#ifndef NEW_INSERT
 if (stutzen_bool)
    for(TCListRow_API::iterator i = begin(); i!=end();)
    {  if (stutzen((TCListRow_API*)this,(TCListRow_API*)(&(*i)),*this))
          i=begin(); // reloop
       else ++i;
    }
#endif

// Summen anzeigen
 for(TCListRow_API::iterator i = begin(); i!=end(); ++i)
 {  if (!((TreeRow*)(*i).get_user_data())->Leaf())
   	((TreeRow*)(*i).get_user_data())->refreshSum(*this);
 }
 show_or_hide_Spalten();
 expand();

//CList Breite anpassen
 for (unsigned int i=0;i<Cols();++i)
        set_column_auto_resize(i,true);

 // callback breitstellen:
 reorder();
}


#ifdef NEW_INSERT

bool operator < (const TCListRow_API &a, const cH_EntryValue &b)
{  return *(reinterpret_cast<TreeRow*>(a.get_user_data())->Value()) < *b;
}

bool operator < (const cH_EntryValue &a, const TCListRow_API &b)
{  return *a < *(reinterpret_cast<TreeRow*>(b.get_user_data())->Value());
}

//#define DEBUG_NEW

void TreeBase::insertIntoTCL(TCListRow_API *parent, const TreeBase &tree,
            const cH_RowDataBase &v, std::deque<guint> selseq, guint deep)
{
recurse:
 TCListRow_API::iterator current_iter=parent->begin();
 TCListRow_API::iterator apiend = parent->end();
 TCListRow_API::iterator upper_b=apiend;
 guint seqnr=selseq.front();	
 cH_EntryValue ev=v->Value(seqnr,ValueData());

// node/leaf mit Wert<=ev suchen
// optimization: we expect to need upper_bound if this is the last attribute
 if (!MehrAlsEinKind(selseq))
 {  pair<TCListRow_API::iterator,TCListRow_API::iterator> range 
 		= std::equal_range(current_iter,apiend,ev);
    current_iter=range.first;	// lower_bound
    upper_b=range.second;	// upper_bound
 }
 else
    current_iter=std::lower_bound(current_iter,apiend,ev);

#ifdef DEBUG_NEW 
cout << "inserting @"<< deep << " " << ev->getStrVal() << " <= ";
if (current_iter!=apiend) 
 std::cout << reinterpret_cast<TreeRow*>((*current_iter).get_user_data())->Value()->getStrVal();
if (!MehrAlsEinKind(selseq) && upper_b!=apiend) 
 std::cout << " <= " << reinterpret_cast<TreeRow*>((*upper_b).get_user_data())->Value()->getStrVal();
cout << '\n';
#endif

 if(current_iter!=apiend) // dann einfuegen
   {// eigentlich nur ein gecastetes current_iter
    TreeRow *current_tclr=reinterpret_cast<TreeRow*>((*current_iter).get_user_data());
    //----------------- gleicher Wert ------------------
    if((ev) == current_tclr->Value())
     { 
      if (MehrAlsEinKind(selseq)) // wenn Blatt noch nicht erreicht
      // eine neue Node erzeugen(?)
      {  cH_RowDataBase v2=current_tclr->LeafData();
         guint child_s_deep=deep;

	do 
	{selseq.pop_front();
	 ++child_s_deep;
	 
	 // darum muss sich eine andere Node kümmern
         if (child_s_deep==current_tclr->Children_s_Deep())
         {  current_tclr->cumulate(v);
            // insertIntoTCL((&*current_iter),tree,v,selseq,child_s_deep);
            // return;
            // goto ist schneller als (end?)rekursion !!!
            parent=&*current_iter;
            deep=child_s_deep;
            goto recurse;
         }
         
        } while (MehrAlsEinKind(selseq) 
			&& v->Value(selseq.front(),ValueData())
				==v2->Value(selseq.front(),ValueData()));
         
	 // vor current_iter einfügen
         TreeRow *newnode=NewNode(deep, ev, child_s_deep, v, child_s_deep < showdeep);
	 newnode->initTCL(parent,current_iter,tree);
	 
	 if (current_tclr->Leaf()) newnode->cumulate(v2);
	 current_tclr->getTCL_API()->reparent(*parent,*newnode->getTCL_API());
//std::cout << child_s_deep << ':' << selseq.front() << '\n';
	 current_tclr->ValueDeep(v2->Value(selseq.front(),ValueData()),child_s_deep);

	 // das neue Blatt einsortieren
	 newnode->cumulate(v);
         // insertIntoTCL(newnode->getTCL_API(),tree,v,selseq,child_s_deep);
         parent=newnode->getTCL_API();
         deep=child_s_deep;
         goto recurse;
      }
      else // Blatt erreicht
      {  // als letztes der Gleichen an parent anhängen
         // upper_b steht schon richtig (s.o.)
         TreeRow *newleaf=NewLeaf(deep,ev,v);
	 newleaf->initTCL(parent, upper_b, tree);
      }
      return;
     }
     else // --------------- kleinerer Wert (davor Einfügen) ----------
	{
#ifdef DEBUG_NEW
cout << "davor\n";
#endif
	 TreeRow *newleaf=NewLeaf(deep,ev,v);
	 newleaf->initTCL(parent,current_iter,tree);
	}
   }
 else //----------------- am Ende der Liste: anhängen ---------------------
   {
#ifdef DEBUG_NEW   
cout << "am Ende\n";
#endif
	    TreeRow *newleaf=NewLeaf(deep,ev,v);
	    newleaf->initTCL(parent,tree); 
    }
}                                

#else // alter Code
// einordnen, Summen berechnen
void TreeBase::insertIntoTCL(TCListRow_API *parent,const TreeBase &tree,
				const cH_RowDataBase &v,std::deque<guint> selseq,guint deep)
{
 TCListRow_API::iterator current_iter = parent->begin();
 TCListRow_API::iterator apiend = parent->end();

 guint seqnr=selseq.front();	

 cH_EntryValue ev=v->Value(seqnr,ValueData());
 TreeRow *current_tclr=0;

 while((current_iter!=apiend) &&
   *( (current_tclr=reinterpret_cast<TreeRow*>((*current_iter).get_user_data()))
   	->Value()) < (*ev))
   current_iter++;

 if(current_iter!=apiend) // einfuegen
   { // gleicher Wert
    if((*ev) == *(current_tclr->Value()))
     { 
       if(!current_tclr->Leaf())
         {
          current_tclr->cumulate(v);
          selseq.pop_front();
	  ++deep;
          insertIntoTCL((&*current_iter),tree,v,selseq,deep);
	 }
      else
      {  // als letztes der Gleichen an parent anhängen
         ++current_iter;
         while ((current_iter!=apiend) &&
            *( (current_tclr=reinterpret_cast<TreeRow*>((*current_iter).get_user_data()))
            			->Value()) == (*ev))
            current_iter++;
         TreeRow *newleaf=NewLeaf(deep,ev,v);
	 newleaf->initTCL(parent,apiend,tree);
      }
      return;
     }
     // kleinerer Wert
  if(MehrAlsEinKind(selseq)) // noch nicht am Blatt
	{
	 TreeRow *newnode=NewNode(deep,ev,deep+1,v,deep < showdeep);
	 newnode->initTCL(parent,current_iter,tree);
	 newnode->cumulate(v);
         selseq.pop_front();
         ++deep;
         // koennte abgekuerzt werden, ist einzelne Node ohne Kinder
	 insertIntoTCL(newnode->getTCL_API(),tree,v,selseq,deep);
	}
     else
	{
	 TreeRow *newleaf=NewLeaf(deep,ev,v);
	 newleaf->initTCL(parent,current_iter,tree);
	}
   }
 else // anhaengen
   {
    if(MehrAlsEinKind(selseq)) // noch nicht am Blatt
	  {
   	 TreeRow *newnode=NewNode(deep,ev,deep+1,v,deep < showdeep);
   	 newnode->initTCL(parent,tree); 
	    newnode->cumulate(v);
       selseq.pop_front();
       ++deep;
	    insertIntoTCL(newnode->getTCL_API(),tree,v,selseq,deep);
	   }
     else
	   {
	    TreeRow *newleaf=NewLeaf(deep,ev,v);
	    newleaf->initTCL(parent,tree); 
	   }
    }
}
#endif

void TreeBase::Expand_recursively(TCListRow_API &api)
{  api.expand();
   for (TCListRow_API::iterator i=api.begin();i!=api.end();++i)
   { (*i).expand();
     if (!KeinKind(*i)) Expand_recursively(*i);
   }
}

void TreeBase::Expand_recursively()
{  detach_from_clist();
   for (TCListRow_API::iterator i=begin();i!=end();++i)
   {  Expand_recursively(*i);
   }
   attach_to_clist();
}         

void TreeBase::fillMenu()
{ assert(menu==0); 
  menu=new Gtk::Menu();
  // Hauptmenü        
   Gtk::MenuItem *neuordnen = manage(new class Gtk::MenuItem("Neuordnen"));
   Gtk::MenuItem *zuruecksetzen = manage(new class Gtk::MenuItem("Zurücksetzen"));
   Gtk::MenuItem *abbrechen = manage(new class Gtk::MenuItem("Abbrechen"));
   Gtk::Menu *spalten_menu = manage(new class Gtk::Menu());
   Gtk::MenuItem *spalten = manage(new class Gtk::MenuItem("Sichtbare Spalten"));
   Gtk::Menu *optionen_menu = manage(new class Gtk::Menu());
   Gtk::MenuItem *optionen = manage(new class Gtk::MenuItem("Optionen"));
   menu->append(*neuordnen);   
   menu->append(*zuruecksetzen);     
   menu->append(*abbrechen);   
   menu->append(*spalten);
   spalten->set_submenu(*spalten_menu);

//   for (std::deque<guint>::const_iterator i=currseq.begin();i!=currseq.end();++i)
   for (guint i=0;i<Cols();++i)
    {
      Gtk::CheckMenuItem *sp = manage(new class Gtk::CheckMenuItem(getColTitle(i)));
      spalten_menu->append(*sp);
      sp->set_active(true);
//      sp->show();
      sp->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::welche_Spalten),i,sp));
    }
   menu->append(*optionen);
   optionen->set_submenu(*optionen_menu);
   Gtk::CheckMenuItem *titles = manage(new class Gtk::CheckMenuItem("Spaltenüberschriften anzeigen"));
   Gtk::CheckMenuItem *auffuellen = manage(new class Gtk::CheckMenuItem("Auffüllen mit aktueller Reinfolge\n(statt Anfangsreinfolge)"));
   Gtk::CheckMenuItem *expandieren = manage(new class Gtk::CheckMenuItem("Gewählte Knoten expandieren"));
   Gtk::MenuItem *exp_all = manage(new class Gtk::MenuItem("Alle Knoten expandieren"));
   Gtk::MenuItem *col_all = manage(new class Gtk::MenuItem("Alle Knoten kollabieren"));
   optionen_menu->append(*titles);
   optionen_menu->append(*auffuellen);
   optionen_menu->append(*expandieren);
   optionen_menu->append(*exp_all);
   optionen_menu->append(*col_all);
//   titles->show();
//   auffuellen->show();
//   expandieren->show();
   
   neuordnen->activate.connect(SigC::slot(this,&TreeBase::on_neuordnen_clicked));
   zuruecksetzen->activate.connect(SigC::slot(this,&TreeBase::on_zuruecksetzen_clicked));
   abbrechen->activate.connect(SigC::slot(this,&TreeBase::on_abbrechen_clicked));

   titles_menu=titles;
   titles_bool=true;
   if(titles_bool) titles->set_active(true);
   else            titles->set_active(false);
   titles->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::Titles),titles));

   auffuellen_bool= true;
   if(auffuellen_bool) auffuellen->set_active(true);
   else                auffuellen->set_active(false);
   auffuellen->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::Auffuellen),auffuellen));

   if(expandieren_bool) expandieren->set_active(true);
   else                 expandieren->set_active(false);
   expandieren->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::Expandieren),expandieren));

   exp_all->activate.connect(SigC::slot(this,&TreeBase::Expand_recursively));
   col_all->activate.connect(SigC::slot(this,&TreeBase::Collapse));

   // Menu anzeigen
//   neuordnen->show();
//   zuruecksetzen->show();
//   abbrechen->show();
//   spalten->show();
//   optionen->show();
   menu->show_all();
}

void TreeBase::welche_Spalten(guint i,const Gtk::CheckMenuItem *sp)
{
  if (sp->get_active()) vec_hide_cols[i] = true;
  else vec_hide_cols[i] = false;
  show_or_hide_Spalten();
}

void TreeBase::set_tree_column_visibility(int column,bool visible)
{
  if(column<Cols())
   {
     vec_hide_cols[column] = visible;
     show_or_hide_Spalten();
   }   
  else std::cerr << "Fehler in 'TreeBase::set_column_visibility': "<<column<<" größer als die Anzahl der Spalten ("<<Cols()<<'\n';
}

void TreeBase::show_or_hide_Spalten()
{
  for (std::deque<guint>::const_iterator i=currseq.begin();i!=currseq.end();++i)
   { if (vec_hide_cols[currseq[*i]]) set_column_visibility(*i,true);
     else set_column_visibility(*i,false);
   }
 for (unsigned int i=currseq.size();i<=Cols();++i)
   { if (vec_hide_cols[i]) set_column_visibility(i,true);
     else set_column_visibility(i,false);
   }
}


gint TreeBase::MouseButton(GdkEventButton *event)
{
//   if ((event->type == GDK_BUTTON_PRESS) && menu)
   if ((event->button==3)  && menu)
   {  menu->popup(event->button,event->time);
      return true;
   }
   return false;
}


void TreeBase::on_neuordnen_clicked()
{
  if (!clicked_seq.size()) return; // nichts tun
  
  // Auffüllen
  if(clicked_seq.size()<Attrs())
   {if (auffuellen_bool) setSequence(); // Standardreihenfolge
   
    for (std::deque<guint>::const_iterator i=currseq.begin();
    		i!=currseq.end();++i)
     { std::deque<guint>::const_iterator j=clicked_seq.begin();
//       for (;j!=clicked_seq.end() && *j!=*i;++j);
         while(j!=clicked_seq.end() && *j!=*i) ++j;
       if (j==clicked_seq.end()) // nicht gefunden
          clicked_seq.push_back(*i);
     }
   }

  currseq=clicked_seq;
  clicked_seq.clear();
  setColTitles();
  refillTCL();
}

void TreeBase::on_zuruecksetzen_clicked()
{ setSequence();
  showdeep=0;
  clicked_seq.clear();
  setColTitles();
  refillTCL();
}

void TreeBase::on_abbrechen_clicked()
{
  clicked_seq.clear();
  setColTitles();
}

void TreeBase::show_titles(bool show)
{
 if (show) {column_titles_show(); titles_menu->set_active(true);}
 else { column_titles_hide(); titles_menu->set_active(false);}
}

void TreeBase::Titles(Gtk::CheckMenuItem *titles)
{
  titles_bool=titles->get_active();
  show_titles(titles_bool);
}
void TreeBase::Auffuellen(Gtk::CheckMenuItem *auffuellen)
{
  auffuellen_bool=auffuellen->get_active();
}
void TreeBase::Expandieren(Gtk::CheckMenuItem *expandieren)
{
  expandieren_bool=expandieren->get_active();
}

void TreeBase::on_row_select(int row, int col, GdkEvent* b)
{ TCListRow_API *tclapi=(TCListRow_API*)(get_row_data(row));
  TreeRow *selectedrow=(TreeRow*)(*tclapi).get_user_data();

  try { 
  if(!selectedrow->Leaf()) 
     node_selected(*(dynamic_cast<TreeRow*>(selectedrow)));
  else
     leaf_selected((dynamic_cast<TreeRow*>(selectedrow))->LeafData());
  } catch(...)
   { assert(!"selectedrow is a TreeRow/Leaf"); }
}

TreeRow *TreeBase::NewNode
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data, bool expand)
{  return new TreeRow(deep,v,child_s_deep, child_s_data, expand); }

TreeRow *TreeBase::NewLeaf
 		(guint deep, const cH_EntryValue &v, const cH_RowDataBase &d)
{  return new TreeRow(deep,v,0,d); }


TreeRow *SimpleTree::defaultNewNode
 		(guint deep, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data, bool expand)
{  return new TreeRow(deep,v,child_s_deep, child_s_data, expand); }

cH_RowDataBase TreeBase::getSelectedRowDataBase() const
	throw(noRowSelected,multipleRowsSelected,notLeafSelected)
{  SelectionList::iterator b=selection().begin(),
			e=selection().end(),
			second=b/* +1 */;
   if (b==e) throw noRowSelected();
   ++second;
   if (second!=e) throw multipleRowsSelected();
   // perhaps put this into another function
   TCListRow_API *tclapi=(TCListRow_API*)(b->get_data());
   TreeRow *selectedrow=(TreeRow*)(*tclapi).get_user_data();
   if (!selectedrow->Leaf()) throw notLeafSelected();
   return (dynamic_cast<TreeRow*>(selectedrow))->LeafData();
}

std::vector<cH_RowDataBase> TreeBase::getSelectedRowDataBase_vec() const throw(notLeafSelected)
{  SelectionList::iterator e=selection().end();

   std::vector<cH_RowDataBase> v;
   for (SelectionList::iterator i=selection().begin(); i!=e; ++i)
    {
      TCListRow_API *tclapi=(TCListRow_API*)(i->get_data());
      TreeRow *selectedrow=(TreeRow*)(*tclapi).get_user_data();
      if (!selectedrow->Leaf()) throw notLeafSelected();
      v.push_back((dynamic_cast<TreeRow*>(selectedrow))->LeafData());
    }
   return v;
}

TCListNode &TreeBase::getSelectedNode() const
	throw(noNodeSelected,multipleNodesSelected,notNodeSelected)
{  SelectionList::iterator b=selection().begin(),
			e=selection().end(),
			second=b/* +1 */;
   if (b==e) throw noNodeSelected();
   ++second;
   if (second!=e) throw multipleNodesSelected();
   // perhaps put this into another function

   TCListRow_API *tclapi=(TCListRow_API*)(b->get_data());
   TreeRow *selectedrow=(TreeRow*)(*tclapi).get_user_data();
   if (selectedrow->Leaf()) throw notNodeSelected();
   return *dynamic_cast<TCListNode*>(selectedrow);
}

