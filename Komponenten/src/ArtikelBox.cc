// $Id: ArtikelBox.cc,v 1.6 2001/07/16 14:22:15 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2001 Adolf Petig GmbH & Co. KG
 *                             written by Christof Petig and Malte Thoma
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

#include <cstdio>
#include "ArtikelBox.hh"
#include <gtk--/box.h>
#include <gtk--/paned.h>
#include <algorithm>
#include <Aux/EntryValueIntString.h>
#include "stock_button_apply.xpm"
#include "stock_button_cancel.xpm"

void ArtikelBox::selectFunc(unsigned int sp,unsigned int l) throw(SQLerror)
{
 // Signifikanz testen
 if(sp == (schema->sigsize(signifikanz[l])-1) || kombiniertbool)
   {try
     {
      loadArtikel(l);
      activate();
     }
   catch(SQLerror &e)
     {std::cerr << e.Code() << e.Message() << e.Context() << "\n";
      pixmap->set(stock_button_cancel_xpm);
      artikel=ArtikelBase();
     }
   return;
 }
 pixmap->set(stock_button_cancel_xpm);
 artikel=ArtikelBase();
 
 combos[l][sp+1]->reset();
 combos[l][sp+1]->grab_focus();
}


const std::string ArtikelBoxErr::ErrMsg() const
{
 switch(code)
   {
    case KOMP_ANZ: 
    {  char ptxt[10];
       snprintf(ptxt,sizeof ptxt,"%d",param);
       return std::string("Es fehlen ")+ptxt+" Bez. komponenten für den Artikel";
    }
    break;
    case NO_ART:
       return std::string("kein Artikel zu der Bezeichnung gefunden");
    break;
    default : return std::string("Fehler in den Bez. Komponenten");
   }
}


gint ArtikelBox::try_grab_focus(GtkWidget *w,gpointer gp)
{  ArtikelBox *this2((ArtikelBox*)gp);
   assert(Gtk::EventBox::isA(this2)); // very weak check
   assert(this2->combos.size());
      	 this2->combos[0][0]->grab_focus();
   return true;
}



void ArtikelBox::set_value(const ArtikelBase &art)
throw(SQLerror,ArtikelBoxErr)
{cH_ArtikelBezeichnung artbez(art,schema->Id());
 if (schema!=artbez->getExtBezSchema()) 
    setExtBezSchema(artbez->getExtBezSchema());

 artikel=art;

 pixmap->set(stock_button_apply_xpm);

 for (unsigned int j=0;j<signifikanz.size();++j)
 {  
#if 0 
    if (j>=combos.size())
    {  cerr << __FILELINE__": Keine Ahnung warum aber in ArtikelBox stimmt was nicht\n";
       cerr << combos.size() << '/' << signifikanz.size() << ' '
       	<< schema->Id() << ',' << schema->Typ() << '|'
       	<< artbez->getExtBezSchema()->Id() << ',' 
       	<< artbez->getExtBezSchema()->Typ() << '\n';
       return;
    }
#endif
    assert(j<combos.size());
    ArtikelBezeichnung::const_sigiterator ci = artbez->sigbegin(signifikanz[j]);
    for (unsigned int i=0; ci!=artbez->sigend(signifikanz[j]) 
    		&& i<combos[j].size(); ++ci, ++i)
    { combos[j][i]->set_text((*ci)->getStrVal());
    }
 }

}


vector<cH_EntryValue> ArtikelBox::expand_kombi_Artikel(unsigned int l)
{
     std::vector<cH_EntryValue> v;
     const std::string text=combos[l][0]->get_text();
     std::string::const_iterator i1=text.begin();
     for (ExtBezSchema::const_sigiterator i=schema->sigbegin(signifikanz[l]);i!=schema->sigend(signifikanz[l]);++i)
     { 
      std::string::const_iterator i2;
      if (i->separator.size()==0) i2 = text.end();
      else i2 = search(i1,text.end(),i->separator.begin(),i->separator.end());
      std::string sx(i1,i2);
      i1=i2+i->separator.size();
      v.push_back(cH_EntryValueIntString(sx));
     }
 return v;
}

void ArtikelBox::loadArtikel(unsigned int l) throw(SQLerror)
{std::vector<cH_EntryValue> v;
 if(!kombiniertbool)
  { for (unsigned int i=0;i<combos[l].size();++i)
     v.push_back(cH_EntryValueIntString(combos[l][i]->get_text()));
  }
 else v = expand_kombi_Artikel(l);
 try {
  cH_ArtikelBezeichnung bez(signifikanz[l],v,schema);
  artikel=bez->Id();
  pixmap->set(stock_button_apply_xpm);

  for (unsigned int j=0;j<signifikanz.size();++j)
   {  ExtBezSchema::const_sigiterator ci = schema->sigbegin(signifikanz[j]);
      for (unsigned int i=0; ci!=schema->sigend(signifikanz[j]) 
      		&& i<combos[j].size(); ++ci, ++i)
         combos[j][i]->set_text((*bez)[ci->bezkomptype]->getStrVal());
   }
 } catch (SQLerror &e)
 {  std::cerr << "ArtikelBox::loadArtikel: setArtikel threw "<< e<< "\n";
 } catch (std::exception &e)
 {  std::cerr << "ArtikelBox::loadArtikel: setArtikel threw "<< e.what()<< "\n";
 }
}


ArtikelBox::~ArtikelBox()
{
}  

void ArtikelBox::init()
{  
 assert(!combos.size());
 assert(!labels.size());
 ArtikelBox::Benutzerprofil_laden();

reloop:
 for (std::vector<int>::iterator i=signifikanz.begin();i!=signifikanz.end();++i)
   if (schema->sigsize(*i) == 0) {signifikanz.erase(i);goto reloop;}
 if (signifikanz.size()==0)  signifikanz.push_back(1); 

 combos.resize(signifikanz.size());
 labels.resize(signifikanz.size());

 int l=signifikanz.size()-1 ; // Anzahl der Signifikanzen
 oberstes = init_table(l);

 for (std::vector<int>::reverse_iterator i=++(signifikanz.rbegin());i!=signifikanz.rend();++i)
  {
    --l;
    Gtk::Container* table = init_table(l);
    if (vertikalbool) 
      {Gtk::VPaned* hp=manage(new Gtk::VPaned);
       hp->set_handle_size(10);
       hp->set_gutter_size(10);
//       hp->set_position(50*schema->sigsize(*i));
       hp->pack1(*table, true, true);
       hp->pack2(*oberstes, true, true);
       hp->show();
       oberstes = hp;
      }
    else 
      {Gtk::HPaned* hp=manage(new Gtk::HPaned);
       hp->set_handle_size(10);
       hp->set_gutter_size(10);
//       hp->set_position(50*schema->sigsize(*i));
       hp->pack1(*table, true, true);
       hp->pack2(*oberstes, true, true);
       hp->show();
       oberstes = hp;
      }
  }
 add(*oberstes);
}

Gtk::Container* ArtikelBox::init_table(int l)
{
 unsigned int cls=schema->sigsize(signifikanz[l]); // Anzahl der Spalten
 Gtk::Table* table = manage(new Gtk::Table(2,cls));
 Gtk::EventBox *ev = manage(new Gtk::EventBox());
 ev->add(*table);

 assert(!combos[l].size());
 assert(!labels[l].size());
 unsigned int i=0; 
 for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz[l]);j!=schema->sigend(signifikanz[l]);++j)
   {Gtk::SearchCombo *sc;
    combos[l].push_back(sc=manage (new Gtk::SearchCombo(true)));
    sc->set_usize(50,0);
    sc->set_autoexpand(false);
    sc->set_enable_tab(true);
    sc->search.connect(SigC::bind(SigC::slot(this,&ArtikelBox::searchFunc),i,l));
    sc->activate.connect(SigC::bind(SigC::slot(this,&ArtikelBox::selectFunc),i,l));

    Gtk::Label *lb;

    std::string text;
    if(kombiniertbool) text = kombinierteAnzeige(signifikanz[l],schema->Typ(),schema->Id());
    else text = j->bezkomptext;
    labels[l].push_back(lb=manage(new Gtk::Label(text)));

    if (i==0) 
     {
       Gtk::HBox *hb= manage(new Gtk::HBox());
       pixmap= manage(new class Gtk::Pixmap(stock_button_cancel_xpm));
       hb->pack_start(*pixmap,false,false);   pixmap->show();
       hb->pack_start(*lb);                   hb->show();
       table->attach(*hb,i,i+1,0,1);
     }
    else  table->attach(*lb,i,i+1,0,1);
    table->attach(*sc,i,i+1,1,2);
    sc->show();
    lb->show();
    ++i;
    if(kombiniertbool) break;
   }
 ev->button_press_event.connect(SigC::slot(this,&ArtikelBox::MouseButton));
 table->show();
 ev->show();
 return ev;
}


ArtikelBox::ArtikelBox(const cH_ExtBezSchema &_schema)  throw(SQLerror)
: oberstes(0), vertikalbool(false), autocompletebool(false), kombiniertbool(false),
  schema(_schema), menu(0), tr("",false), tr2("",false)
{ 
 init();
 menu=manage(new Gtk::Menu());
 fuelleMenu();
 this->button_press_event.connect(SigC::slot(this,&ArtikelBox::MouseButton));

 // redirect our grab_focus
 gtk_signal_connect(GTK_OBJECT(gtkobj()), "grab_focus",
                 GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
}

void ArtikelBox::setExtBezSchema(const cH_ExtBezSchema &_schema)
{
   if (menu)
   {  menu->destroy();
      menu=0;
   }
   reset();

   // Altes Widget löschen
   remove();
   combos.clear();
   labels.clear();
   schema=_schema;
//   Benutzerprofil_laden(); // wird in init gemacht
   init();
   menu=manage(new Gtk::Menu());
   fuelleMenu();
/*
cout << signifikanz.size()<<"\t";
for (int i=0;i<signifikanz.size();++i)std::cout << signifikanz[i]<<"\t";
cout << "\n";
*/
}

void ArtikelBox::TypSelected(int typ)
{  cH_ExtBezSchema alt(schema);
   try
   {  cH_ExtBezSchema sch=cH_ExtBezSchema(schema->Id(),ArtikelTyp(typ));
      setExtBezSchema(sch);
   }
   catch (...)
   {  setExtBezSchema(alt);
   }
}

gint ArtikelBox::MouseButton(GdkEventButton *event)
{  // std::cout << "MB\n";
   if ((event->type == GDK_BUTTON_PRESS) && menu)
   {  menu->popup(event->button,event->time);
      return true;
   }
   return false;
}

void ArtikelBox::setzeTyp(int t)
{  
   ArtikelBox::Benutzerprofil_speichern();
   setExtBezSchema(cH_ExtBezSchema(schema->Id(),t));
}
void ArtikelBox::setzeTyp2(int t2)
{  
   ArtikelBox::Benutzerprofil_speichern();
   setExtBezSchema(cH_ExtBezSchema(t2,schema->Typ()));
}


void ArtikelBox::setzeSignifikanz(int t)
{  
 bool add = true;
 for (std::vector<int>::iterator i=signifikanz.begin();i!=signifikanz.end();++i)
   if ( (*i)==t ) { signifikanz.erase(i); add=false; break;}
 if (add) signifikanz.push_back(t);
   {  ArtikelBox::Benutzerprofil_speichern();
      setExtBezSchema(schema);
   }
}


void ArtikelBox::Autocomplete(Gtk::CheckMenuItem *autocomplete)
{ for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->set_autoexpand(autocomplete->get_active()); 
  autocompletebool=autocomplete->get_active();
  Benutzerprofil_speichern();
}  

void ArtikelBox::set_Vertikal(Gtk::CheckMenuItem *verti)
{
  if (verti->get_active()) vertikalbool=true;
  else vertikalbool=false;
  setExtBezSchema(schema);
}

void ArtikelBox::kombiniert(Gtk::CheckMenuItem *kombi)
{
  if (kombi->get_active()) kombiniertbool=true;
  else kombiniertbool=false;
  Benutzerprofil_speichern();
  setExtBezSchema(schema);
}

