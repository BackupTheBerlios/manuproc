// $Id: ArtikelBox.cc,v 1.15 2002/04/11 11:57:59 christof Exp $
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
#include <Artikel/ArtikelBezeichnung.h>
#include <Aux/Global_Settings.h>

void ArtikelBox::selectFunc(unsigned int sp,unsigned int l) throw(SQLerror)
{
 if(automatisch_anlegen_bool)
  {
    if(sp+1==combos[l].size()) Neuer_Eintrag() ;
    else combos[l][sp+1]->grab_focus();
  }
 else
  {
    // Signifikanz testen
    if(sp == (schema->sigsize(signifikanz[l])-1) || kombiniertbool)
      {try
        {
         loadArtikel(l);
         activate();
        }
      catch(SQLerror &e)
        {
         std::cerr << e.Code() << e.Message() << e.Context() << "\n";
         pixmap_setzen(false);
         artikel=ArtikelBase();
        }
      return;
    }
    pixmap_setzen(false);
    artikel=ArtikelBase();

//std::cout << l<<"\t"<<sp<<combos[l][sp+1]->get_text(); 
    combos[l][sp+1]->reset();
//std::cout << l<<"\t"<<sp<< combos[l][sp+1]->get_text(); 
    combos[l][sp+1]->grab_focus();
   }
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

 pixmap_setzen(true);

 for (unsigned int j=0;j<signifikanz.size();++j)
 {  
    assert(j<combos.size());
    ArtikelBezeichnung::const_sigiterator ci = artbez->sigbegin(signifikanz[j]);
    if (!kombiniertbool)   
       for (unsigned int i=0; ci!=artbez->sigend(signifikanz[j]) 
       		&& i<combos[j].size(); ++ci, ++i)
         { combos[j][i]->set_text((*ci)->getStrVal());
         }
    else
      { std::string text;
        cH_ExtBezSchema schema = artbez->getExtBezSchema();
        ExtBezSchema::const_sigiterator is=schema->sigbegin(signifikanz[j]);
        for(;ci!=artbez->sigend(signifikanz[j])
            && is!=schema->sigend(signifikanz[j]) ;++ci,++is)
          {  
           std::string sep = is->separator;
           if (sep==" ") sep = "_";
           text += (*ci)->getStrVal() + sep;
          }
        combos[j][0]->set_text(text);
      }
 }
}

vector<cH_EntryValue> ArtikelBox::expand_kombi(unsigned int l,enum_art_label eal)
{
 std::string text;
 std::vector<cH_EntryValue> v;
 if (eal==ARTIKEL) 
   { text = combos[l][0]->get_text(); 
     v=expand_kombi_Artikel(l,text);
   }
 if (eal==LABEL)
    for (ExtBezSchema::const_sigiterator i=schema->sigbegin(signifikanz[l]);i!=schema->sigend(signifikanz[l]);++i)
       v.push_back(cH_EntryValueIntString(i->spaltenname));
 return v;
}

vector<cH_EntryValue> ArtikelBox::expand_kombi_Artikel(unsigned int l,std::string text)
{
     std::vector<cH_EntryValue> v;
     std::string::size_type s1=0;
     for (ExtBezSchema::const_sigiterator i=schema->sigbegin(signifikanz[l]);i!=schema->sigend(signifikanz[l]);)
     { 
      std::string sep = i->separator;
      ExtBezSchema::const_sigiterator j=i;
      if (sep==" "&& ++j!=schema->sigend(signifikanz[l]) ) sep="_";
      std::string::size_type s2=0;
      if (sep.size()==0) s2 = std::string::npos;
      else s2 = text.find(sep,s1);
      std::string sx(text,s1,s2-s1);
      s1=s2 + sep.size();
//std::cout << s1 <<'\t'<<s2<<"\t->"<<sx<<"<-\n";
      v.push_back(cH_EntryValueIntString(sx));
      if (s2==std::string::npos) break;
     }
 return v;
}

void ArtikelBox::loadArtikel(unsigned int l) throw(SQLerror)
{std::vector<cH_EntryValue> v;
 if(!kombiniertbool)
  { for (unsigned int i=0;i<combos[l].size();++i)
     v.push_back(cH_EntryValueIntString(combos[l][i]->get_text()));
  }
 else v = expand_kombi(l,ARTIKEL);
 try {
  while (v.size()<schema->size(signifikanz[l])) 
     v.push_back(cH_EntryValueIntString(""));
  
  cH_ArtikelBezeichnung bez(signifikanz[l],v,schema);
  artikel=*bez;
  set_value(artikel);
 } catch (SQLerror &e)
 {  std::cerr << "ArtikelBox::loadArtikel: setArtikel threw "<< e<< "\n";
 } catch (std::exception &e)
 {  std::cerr << "ArtikelBox::loadArtikel: setArtikel threw "<< e.what()<< "\n";
 }
}


ArtikelBox::~ArtikelBox()
{  if (menu) delete menu;
}  

void ArtikelBox::init()
{
   // Altes Widget löschen (Gtk::Container::)
#if 1   
   remove();
   combos.clear();
   labels.clear();
#else
   assert(!combos.size());
   assert(!labels.size());
#endif   

 Benutzerprofil_laden();   // setzt signifikanz !!! 

 if(!alle_artikel_anzeigen_bool)
  {
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
 else
   {
     int s=1;
     if (alle_artikel_anzeigen_mit_id_bool) s=2;
     combos.resize(s);
     labels.resize(s);
     oberstes = init_table_alle_artikel(s);
     add(*oberstes);
   }
 fuelleMenu();
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
   {
    Gtk::SearchCombo *sc;
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

    if (i==0&&l==0) 
     {
       Gtk::HBox *hb= manage(new Gtk::HBox());
       pixmap= manage(new class Gtk::Pixmap()); // stock_button_cancel_xpm));
       pixmap_setzen(false);
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
 std::string labtext = ArtikelTyp::get_string(schema->Typ())
      +": "+ cH_Kunde(schema->Id())->firma();
 label_typ = manage(new class Gtk::Label(labtext));
 table->attach(*label_typ,0,cls,2,3);
 table->show();
 ev->show();
 return ev;
}


Gtk::Container* ArtikelBox::init_table_alle_artikel(int s)
{
 Gtk::Table* table = manage(new Gtk::Table(2,s));
 Gtk::EventBox *ev = manage(new Gtk::EventBox());
 ev->add(*table);

 assert(!combos[s-1].size());
 assert(!labels[s-1].size());
 for(int i=0;i<s;++i)
   {
    Gtk::SearchCombo *sc;
    combos[s].push_back(sc=manage (new Gtk::SearchCombo(true)));
    sc->set_usize(50,0);
    sc->set_autoexpand(false);
    sc->set_enable_tab(true);
    sc->search.connect(SigC::bind(SigC::slot(this,&ArtikelBox::searchFunc_alle_artikel),i));
    sc->activate.connect(SigC::bind(SigC::slot(this,&ArtikelBox::selectFunc_alle_artikel),i));

    Gtk::Label *lb;
    std::string text;
    if     (i==0) text="Artikel";
    else if(i==1) text="ID";
    labels[i].push_back(lb=manage(new Gtk::Label(text)));

    if (i==0) 
     {
       Gtk::HBox *hb= manage(new Gtk::HBox());
       pixmap= manage(new class Gtk::Pixmap()); // stock_button_cancel_xpm));
       pixmap_setzen(false);
       hb->pack_start(*pixmap,false,false);   pixmap->show();
       hb->pack_start(*lb);                   hb->show();
       table->attach(*hb,i,i+1,0,1);
     }
    else  table->attach(*lb,i,i+1,0,1);
    table->attach(*sc,i,i+1,1,2);
    sc->show();
    lb->show();
   }
 ev->button_press_event.connect(SigC::slot(this,&ArtikelBox::MouseButton));
 std::string labtext = "Artikelsuche in allen Schemata und für alle Kunden";
 label_typ = manage(new class Gtk::Label(labtext));
 table->attach(*label_typ,0,1,2,3);
 table->show();
 ev->show();
 return ev;
}


ArtikelBox::ArtikelBox(const cH_ExtBezSchema &_schema)  throw(SQLerror)
: vertikalbool(false), autocompletebool(false), 
   kombiniertbool(false),labelbool(true), 
   automatisch_anlegen_bool(false),eingeschraenkt(false),
   alle_artikel_anzeigen_bool(false),alle_artikel_anzeigen_mit_id_bool(false),
  schema(_schema), tr("",false), tr2("",false),
  oberstes(0), menu(0),  pixmap(0), label_typ(0), label(0)
{ 
 artbox_start();
}

ArtikelBox::ArtikelBox(const std::string& _program,const std::string& _position)  throw(SQLerror)
: vertikalbool(false), autocompletebool(false), 
   kombiniertbool(false),labelbool(true), 
   automatisch_anlegen_bool(false),eingeschraenkt(false),
   alle_artikel_anzeigen_bool(false),alle_artikel_anzeigen_mit_id_bool(false),
   sprogram(_program),sposition(_position),
   schema(cH_ExtBezSchema(ExtBezSchema::default_ID)),
   tr("",false), tr2("",false),
   oberstes(0), menu(0),  pixmap(0), label_typ(0), label(0)
{ 
  artbox_start();
  std::string gs = Global_Settings(0,sprogram,sposition).get_Wert();
//std::cout << "GS="<<gs<<'\n';
  if (gs!="")
   {
     std::string sep=":";
     std::string::size_type  p=gs.find(sep);
     std::string s(gs,0,p);
     std::string t(gs,p+sep.size(),std::string::npos);
//std::cout << s <<'-'<<t<<'\n';
     ExtBezSchema::ID eid = atoi(s.c_str());
     ArtikelTyp atyp      = atoi(t.c_str());
//std::cout << eid<<'-'<<atyp<<'\n';
     try
     {  setExtBezSchema(cH_ExtBezSchema(eid,atyp)); 
     }
     catch (SQLerror &e)
     {  std::cout << e << '\n';
     }
   }
}

void ArtikelBox::artbox_start()
{
 init();
 this->button_press_event.connect(SigC::slot(this,&ArtikelBox::MouseButton));

 // redirect our grab_focus
 gtk_signal_connect(GTK_OBJECT(gtkobj()), "grab_focus",
                 GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
}

void ArtikelBox::setExtBezSchema(const cH_ExtBezSchema &_schema)
{
   schema=_schema;
   init();
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

void ArtikelBox::setzeSchemaId(int t)
{  
   Benutzerprofil_speichern();
//cout << "ArtikelBox::setzeSchemaId: "<<' '<<t<<'\n';
   try {
      setExtBezSchema(cH_ExtBezSchema(schema->Id(),t));
   } catch (SQLerror &e)
   {  setExtBezSchema(cH_ExtBezSchema(ExtBezSchema::default_ID,t));
   }
   if (sprogram!="")
      Global_Settings(0,sprogram,sposition,itos(schema->Id())+":"+itos(t));
}

void ArtikelBox::setzeSchemaTyp(int t2)
{  
   Benutzerprofil_speichern();
   setExtBezSchema(cH_ExtBezSchema(t2,schema->Typ()));
   if (sprogram!="")
      Global_Settings(0,sprogram,sposition,itos(t2)+":"+itos(schema->Typ()));
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
{ 
  autocompletebool=autocomplete->get_active();
  set_autoexpand(autocompletebool);
  Benutzerprofil_speichern();
}  

void ArtikelBox::set_Vertikal(Gtk::CheckMenuItem *verti)
{
  vertikalbool=verti->get_active();
  setExtBezSchema(schema);
}

void ArtikelBox::kombiniert(Gtk::CheckMenuItem *kombi)
{
  kombiniertbool=kombi->get_active();
  Benutzerprofil_speichern();
  setExtBezSchema(schema);
}

void ArtikelBox::set_Label(Gtk::CheckMenuItem *label)
{
  labelbool=label->get_active();
  show_label(labelbool);
}

void ArtikelBox::show_label(bool b)
{
  labelbool=b;
  if(labelbool) label_typ->show();
  else          label_typ->hide();
  // vielleich einfach Menu neu aufbauen, dann kann label dort lokal bleiben
  label->set_active(labelbool);
}

double ArtikelBox::get_menge_from_artikelbox()
{
  std::string aufmachung;
  for (unsigned int j=0;j<combos.size();++j)
   for (unsigned int i=0; i<combos[j].size(); ++i)
    if (labels[j][i]->get_text()==schema->JumboTitel())
       aufmachung=combos[j][i]->get_text();
  std::string smenge;
  for (std::string::const_iterator i=aufmachung.begin();i!=aufmachung.end();++i)
   {
     char c=*i;
#warning KOMMA ist erlaubt MAT
     if (c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='.'||c==',')
       { if(c==',') c='.'; smenge += c; }
     else break;     
   }
  double menge=atof(smenge.c_str());
  return menge;
}

void ArtikelBox::Einschraenken(const std::string &e, bool an)
{  einschraenkung=e;
   Einschraenken_b(an);
}

void ArtikelBox::Einschraenken_b(bool an)
{  eingeschraenkt=an;
   fuelleMenu();
   pixmap_setzen(artikel.Id()!=0);
}

void ArtikelBox::ClearUserMenus()
{  user_menus.clear();
   fuelleMenu();
}

void ArtikelBox::AddUserMenu(const std::string &text, gpointer data)
{  user_menus.push_back(user_menu_t(text,data));
   fuelleMenu();
}

void ArtikelBox::einschraenken_cb(Gtk::CheckMenuItem *einschr_mi)
{  Einschraenken_b(einschr_mi->get_active());
}

#include "stock_button_apply.xpm"
#include "stock_button_cancel.xpm"
#include "E.xpm"

void ArtikelBox::pixmap_setzen(bool valid)
{  if (valid) pixmap->set(stock_button_apply_xpm);
   else if (eingeschraenkt) pixmap->set(E_xpm);
   else pixmap->set(stock_button_cancel_xpm);
}

void ArtikelBox::Neuer_Eintrag_automatisch(Gtk::CheckMenuItem *cmi)
{
  automatisch_anlegen_bool=cmi->get_active();
  for (unsigned int j=0;j<combos.size();++j)
   for (unsigned int i=0; i<combos[j].size(); ++i)
    {
     if(automatisch_anlegen_bool)
         combos[j][i]->set_value_in_list(false,true);
     else
         combos[j][i]->set_value_in_list(true,true);
    }  
}

void ArtikelBox::AlleArtikelAnzeigen(Gtk::CheckMenuItem *cmi)
{
  alle_artikel_anzeigen_bool=cmi->get_active();  
  if(alle_artikel_anzeigen_bool)
   {
    kombiniertbool=true;    
   }
  init();
}

void ArtikelBox::AlleArtikelAnzeigenId(Gtk::CheckMenuItem *cmi)
{
  alle_artikel_anzeigen_mit_id_bool=cmi->get_active();
  init();
}
