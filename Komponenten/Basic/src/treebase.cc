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

const string TreeBase::getColTitle(guint seq) const
{
 return "-";
}

void TreeBase::setSequence()
{  currseq.clear();
   for(guint i=0; i<attrcount; ++i) currseq.push_back(i);
}

void TreeBase::on_click_column(int col)
{
//cout << col<<"\t"<<currseq[col]<<"\t"<<Attrs()<<"\t"<<col_count<<"\n";
 if (col_schon_ausgewaehlt(col)) return;
 if (col<(int)Attrs()) 
   {
    clicked_seq.push_back(currseq[col]);
    set_column_title(col, itos(clicked_seq.size())); 
    if (clicked_seq.size()==1) summen_knoepfe();
    if (expandieren_bool) showdeep = clicked_seq.size();
      else showdeep = 0;
    if (clicked_seq.size()==Attrs()) on_neuordnen_clicked(); 
   }

 else // kann nur neu/abbrechen sein
 {  if (col==(int)Cols()-1) on_neuordnen_clicked();
    if (col==(int)Cols()-2) on_abbrechen_clicked();
 }
}

bool TreeBase::col_schon_ausgewaehlt(int col)
{
 deque<guint>::const_iterator i =clicked_seq.begin();
 while (i!=clicked_seq.end() && *i!=currseq[col]) ++i;
 if (i==clicked_seq.end()) return false;
 else return true;
}

void TreeBase::summen_knoepfe()
{
  if (Attrs()<Cols())
    set_column_title(Cols()-1, "Neuordnen"); 
  if (Attrs()<Cols()-1)
    set_column_title(Cols()-2, "Abbrechen"); 
}

TreeBase::TreeBase(guint cols, guint attr) : 
	TCList(cols), showdeep(0), attrcount(attr ? attr : cols), menu(0),
	auffuellen_bool(false), expandieren_bool(true)
{
  this->button_press_event.connect(SigC::slot(this,&TreeBase::MouseButton));
  click_column.connect(SigC::slot(this,&TreeBase::on_click_column));
  select_row.connect(SigC::slot(this, &TreeBase::on_row_select));
  setSequence();
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
 deque<guint> seqtitle = currseq;
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

#if 0 // this is missing so many things that I will comment it out !!!
TreeBase::TreeBase(const vector<string> &cols) : 
		TCList(cols),showdeep(0),attrcount(cols.size()) 
{
 click_column.connect(SigC::slot(this,&TreeBase::on_click_column));
 for(guint i=1; i<=attrcount; ++i)
   seq.push_back(i);
}
#endif

bool TreeBase::stutzen(TCListRow_API *parent, TCListRow_API *we,
						TCList &tclist,guint deep)
{
 if (we->size()==1)
 {  guint d=deep;
    while (we->size()==1)
    {  TCListRow *child_and_brother_to_be= &* (we->begin());
       TCListRow *we_as_a_row=static_cast<TCListRow*>(we);

       we->reparent_children(*parent);
       // copy non-empty attribute cells
       for (unsigned int i=0;i<=d;++i)
       {  const string t(we_as_a_row->get_text(i));
          if (t.size())
             child_and_brother_to_be->set_text(i,t);
       }
       tclist.erase(TCListRow_iterator(parent,static_cast<TCListRow*>(we)));
       we=child_and_brother_to_be;
       d++;
    }
    return true;
 }

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

void TreeBase::refillTCL()
{
// liste loeschen
 collapse();
 TCList::clear();

 vector<cH_RowDataBase>::const_iterator i=datavec.begin();
 vector<cH_RowDataBase>::const_iterator j=datavec.end();

// neu einordnen
 for(; i!=j; ++i)
    insertIntoTCL((TCListRow_API*)this,*this,*i,currseq,0);

// Aeste mit einem Blatt kuerzen 
 for(TCListRow_API::iterator i = begin(); i!=end();)
 {if (stutzen((TCListRow_API*)this,(TCListRow_API*)(&(*i)),*this,0))
      i=begin(); // reloop
  else ++i;
 }

// Summen errechnen
 for(TCListRow_API::iterator i = begin(); i!=end(); ++i)
   ((TCListRowData*)(*i).get_user_data())->refreshSum(*this);

 expand();

//CList Breite anpassen
 for (unsigned int i=0;i<Cols();++i)
        set_column_auto_resize(i,true);

}


void TreeBase::insertIntoTCL(TCListRow_API *tclapi,const TreeBase &tb,
				const cH_RowDataBase &v,deque<guint> selseq,guint deep)
{
 TCListRow_API::iterator lfind = tclapi->begin();
 TCListRow_API::iterator lend = tclapi->end();

 guint seqnr=selseq.front();	
 guint spaltenr = Attrs() - selseq.size()+1;

// if (show_column_nr(spaltenr)-1) return;

 cH_EntryValue ev=v->Value(seqnr);

 while((lfind!=lend) &&
   *( reinterpret_cast<TCListRowData*>((*lfind).get_user_data())->Value()) < (*ev))
   lfind++;
	
 if(lfind!=lend)
   {
    if((*ev) == *reinterpret_cast<TCListRowData*>((*lfind).get_user_data())->Value())
     {
      reinterpret_cast<TCListNode*>((*lfind).get_user_data())->cumulate(v,seqnr);
      if(!reinterpret_cast<TCListNode*>((*lfind).get_user_data())->Leaf())
	{selseq.pop_front();
         insertIntoTCL((&*lfind),tb,v,selseq,++deep);
	}
      return;
     }

    if(selseq.size()>1) // noch nicht am Blatt
	{
	 TCListNode *newnode=NewNode(seqnr,v,deep);
    if (spaltenr < showdeep) newnode->set_Show(true);
	 newnode->cumulate(v,seqnr);
	 newnode->initTCL(tclapi,lfind,tb,deep);
         selseq.pop_front();
	 insertIntoTCL(newnode->getTCL_API(),tb,v,selseq,++deep);
	}
     else
	{
	 TCListLeaf *newleaf=NewLeaf(seqnr,v,deep); 
	 newleaf->initTCL(tclapi,lfind,tb,deep);
	}
   }
 else
   {
    if(selseq.size()>1) // noch nicht am Blatt
	{
	 TCListNode *newnode=NewNode(seqnr,v,deep);
    if (spaltenr < showdeep) newnode->set_Show(true);
	 newnode->cumulate(v,seqnr);
	 newnode->initTCL(tclapi,tb,deep);
         selseq.pop_front();
	 insertIntoTCL(newnode->getTCL_API(),tb,v,selseq,++deep);
	}
     else
	{
	 TCListLeaf *newleaf=NewLeaf(seqnr,v,deep); 
	 newleaf->initTCL(tclapi,tb,deep);
	}
   }

}


TCListNode *TreeBase::NewNode(guint _seqnr, const cH_RowDataBase &v, 
				guint deep)
{
 return new TCListNode(_seqnr,v,deep); 
}

TCListLeaf *TreeBase::NewLeaf(guint _seqnr, const cH_RowDataBase &v, 
				guint deep)
{
 return new TCListLeaf(_seqnr,v,deep); 
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

//   for (deque<guint>::const_iterator i=currseq.begin();i!=currseq.end();++i)
   for (guint i=0;i<Cols();++i)
    {
      Gtk::CheckMenuItem *sp = manage(new class Gtk::CheckMenuItem(getColTitle(i)));
      spalten_menu->append(*sp);
      sp->set_active(true);
      sp->show();
      sp->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::welche_Spalten),i));
    }
   menu->append(*optionen);
   optionen->set_submenu(*optionen_menu);
   Gtk::CheckMenuItem *auffuellen = manage(new class Gtk::CheckMenuItem("Auffüllen mit aktueller Reinfolge\n(statt Anfangsreinfolge)"));
   Gtk::CheckMenuItem *expandieren = manage(new class Gtk::CheckMenuItem("Gewählte Knoten expandieren"));
   optionen_menu->append(*auffuellen);
   optionen_menu->append(*expandieren);
   auffuellen->set_active(true);
   auffuellen->show();
   expandieren->show();
   

   neuordnen->activate.connect(SigC::slot(this,&TreeBase::on_neuordnen_clicked));
   zuruecksetzen->activate.connect(SigC::slot(this,&TreeBase::on_zuruecksetzen_clicked));
   abbrechen->activate.connect(SigC::slot(this,&TreeBase::on_abbrechen_clicked));

   if(auffuellen_bool) auffuellen->set_active(true);
   else                auffuellen->set_active(false);
   auffuellen->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::Auffuellen),auffuellen));

   if(expandieren_bool) expandieren->set_active(true);
   else                 expandieren->set_active(false);
   expandieren->activate.connect(SigC::bind(SigC::slot(this,&TreeBase::Expandieren),expandieren));

   // Menu anzeigen
   neuordnen->show();
   zuruecksetzen->show();
   abbrechen->show();
   spalten->show();
   optionen->show();
}

void TreeBase::welche_Spalten(guint i)
{
  vector<guint>::iterator k=vec_hide_cols.begin();
  while (k!=vec_hide_cols.end() && *k!=i) ++k;
  if (k==vec_hide_cols.end()) 
   { vec_hide_cols.push_back(i); 
     set_column_visibility(i,false);}
  else
   { vec_hide_cols.erase(k);
     set_column_visibility(i,true);
   }
}

/*
bool TreeBase::show_column_nr(guint i)
{
  for (vector<guint>::const_iterator k=vec_hide_cols.begin();
      k!=vec_hide_cols.end();++k)
    if ((*k)==i) return false;
  return true;
}
*/

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
   
    for (deque<guint>::const_iterator i=currseq.begin();
    		i!=currseq.end();++i)
     { deque<guint>::const_iterator j=clicked_seq.begin();
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
  TCListRowData *selectedrow=(TCListRowData*)(*tclapi).get_user_data();

  if(!selectedrow->Leaf()) return;
  leaf_selected((dynamic_cast<TCListLeaf*>(selectedrow))->LeafData());
}
