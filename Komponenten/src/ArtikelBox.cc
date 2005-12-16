// $Id: ArtikelBox.cc,v 1.35 2005/12/16 07:52:44 christof Exp $
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
#include <stdexcept>
#include "ArtikelBox.hh"
#include <gtkmm/box.h>
#include <gtkmm/paned.h>
#include <algorithm>
#include <Artikel/ArtikelBezeichnung.h>
#include <Aux/Global_Settings.h>
#include <Misc/relops.h>

#include <unistd.h>
#include <sys/types.h>
#include <gtkmm/main.h>
#include <gtk/gtksignal.h>
#include <gtkmm/stock.h>
#include <gdkmm/pixbufloader.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/bind.h>
#include <sigc++/compatibility.h>
#endif
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Artikel/ArtikelBaum.h>

#include <Misc/TagStream.h>
#include <Misc/create_parse.h>
#include <Misc/mystream.h>

void ArtikelBox::selectFunc(unsigned sp,unsigned l) throw(SQLerror)
{
 if(automatisch_anlegen_bool)
  { 
   if(sp+1==combos[l].size()) 
    {  
       // erst gucken ob schon da ...
       bool ok=loadArtikel(l);
       if(ok) activate();
       else 
        {  
         Neuer_Eintrag() ;
        }
    }
    else 
    {  combos[l][sp+1]->grab_focus(); // kein reset? CP
       pixmap_setzen(false);
    }
  }
 else
  {
    // Signifikanz testen
    if(kombiniertbool || sp == (schema->sigsize(signifikanz[l])-1))
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
//    combos[l][sp+1]->reset();
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
{  ArtikelBox *this2=dynamic_cast<ArtikelBox*>((Glib::Object*)gp);
   assert(this2);
   assert(this2->combos.size());
      	 this2->combos[0][0]->grab_focus();
   return true;
}

bool ArtikelBox::set_value_idle(Handle<const ArtikelBezeichnung> artbez)
{  setExtBezSchema(artbez->getExtBezSchema());
   
 artikel=*artbez;

 pixmap_setzen(true);
 for (unsigned j=0;j<signifikanz.size();++j)
 {  
    assert(j<combos.size());
    std::vector<cH_EntryValue> v;
    for (ArtikelBezeichnung::const_sigiterator ci = artbez->sigbegin(signifikanz[j]);
    		ci!=artbez->sigend(signifikanz[j]); ++ci)
    	v.push_back((*ci));
    set_content(v,j);
 }
 return false;
}

void ArtikelBox::set_value(const ArtikelBase &art)
throw(SQLerror,ArtikelBoxErr)
{cH_ArtikelBezeichnung artbez(art,schema->Id());
 artikel=art;

 pixmap_setzen(true);
 
 if (schema!=artbez->getExtBezSchema()) 
 {  Glib::signal_idle().connect(SigC::bind(SigC::slot
 		(*this,&ArtikelBox::set_value_idle),
 		artbez));
 }

 for (unsigned j=0;j<signifikanz.size();++j)
 {  
    assert(j<combos.size());
    std::vector<cH_EntryValue> v;
    for (ArtikelBezeichnung::const_sigiterator ci = artbez->sigbegin(signifikanz[j]);
    		ci!=artbez->sigend(signifikanz[j]); ++ci)
    	v.push_back((*ci));
    set_content(v,j);
 }
}

std::string ArtikelBox::kombinierterName(cH_ExtBezSchema schema,int signifikanz)
{  std::vector<std::string> v;
   for (ExtBezSchema::const_sigiterator i=schema->sigbegin(signifikanz);
   	i!=schema->sigend(signifikanz);++i)
       v.push_back(i->spaltenname);
   return Kombinieren(schema,signifikanz,v);
}

std::vector<cH_EntryValue> ArtikelBox::expand_kombi_Artikel(unsigned l,std::string text) const
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

std::vector<cH_EntryValue> ArtikelBox::get_content(unsigned l,unsigned spmax) const
{std::vector<cH_EntryValue> v;
 if(!kombiniertbool)
  { spmax=std::min<size_t>(combos[l].size(),spmax+1);
    for (unsigned i=0;i<spmax;++i)
     v.push_back(cH_EntryValueIntString(combos[l][i]->get_text()));
  }
 else 
  { v = expand_kombi_Artikel(l, combos[l][0]->get_text());
  }
  return v;
}

std::string ArtikelBox::Kombinieren(cH_ExtBezSchema schema, unsigned sig,const std::vector<std::string> &v)
{std::string text;
 std::vector<std::string>::const_iterator vi=v.begin();
 for (ExtBezSchema::const_sigiterator i=schema->sigbegin(sig);
   			std_neq(vi,v.end()) && i!=schema->sigend(sig);
   			++i,++vi)
    {text += *vi;
     if (i->separator==" ") text += "_";
     else text +=i->separator;
    }
  return text;
}

std::string ArtikelBox::Kombinieren(cH_ExtBezSchema schema, unsigned sig,const std::vector<cH_EntryValue> &v)
{  std::vector<std::string> u;
   u.reserve(v.size());
   for (std::vector<cH_EntryValue>::const_iterator i=v.begin();std_neq(i,v.end());++i)
      u.push_back((*i)->getStrVal());
   return Kombinieren(schema,sig,u);
}

void ArtikelBox::set_content(const std::vector<cH_EntryValue> &v,unsigned l)
{   unsigned grenze=v.size();
    if (!kombiniertbool)
    {  grenze=std::min<size_t>(grenze,combos[l].size());
       for (unsigned i=0; i<grenze; ++i)
         { combos[l][i]->set_text(v[i]->getStrVal());
         }
    }
         else combos[l][0]->set_text(Kombinieren(schema,signifikanz[l],v));
}

bool ArtikelBox::loadArtikel(unsigned l) throw(SQLerror)
{
 try {
  std::vector<cH_EntryValue> v=get_content(l);
  while (v.size()<schema->size(signifikanz[l])) v.push_back(cH_EntryValue());
  cH_ArtikelBezeichnung bez(signifikanz[l],v,schema);
  artikel=*bez;
  set_value(artikel);
  return true;
 } catch (SQLerror &e)
 {  std::cerr << "ArtikelBox::loadArtikel: setArtikel threw "<< e<< "\n"; return false;
 } catch (std::exception &e)
 {  std::cerr << "ArtikelBox::loadArtikel: setArtikel threw "<< e.what()<< "\n"; return false;
 }
}


ArtikelBox::~ArtikelBox()
{  if (menu) delete menu;
}  

void ArtikelBox::init()
{
 Benutzerprofil_laden();   // setzt signifikanz !!! 
}

void ArtikelBox::setzeAnzeige(const ArtikelBoxAnzeige &anz)
{  if (anz.schema!=schema) setExtBezSchema(anz.schema);
   // Altes Widget löschen (Gtk::Container::)
   remove();
   combos.clear();
   labels.clear();

  signifikanz.clear();
  for (int i=-int(sizeof(int))*8;i<=int(sizeof(int))*8;++i)
     if (anz.Sichtbar(i)) signifikanz.push_back(i);
  autocompletebool=anz.AutoComplete();
  set_autoexpand(autocompletebool);
  kombiniertbool=anz.Kombiniert();
  reset_on_focus=anz.reset_on_focus;

  init2();
}

void ArtikelBox::init2()  
{
  {
   reloop:
    for (std::vector<int>::iterator i=signifikanz.begin();std_neq(i,signifikanz.end());++i)
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
       Gtk::Paned* hp=0;
       if (vertikalbool) hp=manage(new Gtk::VPaned);
       else hp=manage(new Gtk::HPaned);

//          hp->set_handle_size(10);
//          hp->set_gutter_size(10);
   //       hp->set_position(50*schema->sigsize(*i));
          hp->pack1(*table, true, true);
          hp->pack2(*oberstes, true, true);
          hp->show();
          oberstes = hp;
      }
    add(*oberstes);
   }
 std::string labtext = ArtikelTyp::get_string(schema->Typ())
      +": "+ cH_Kunde(schema->Id())->firma();
 tooltips.set_tip(*this,labtext);
 fuelleMenu();
 adjust_combo_behaviour();
}

Gtk::Container* ArtikelBox::init_table(int l)
{
 unsigned cls=schema->sigsize(signifikanz[l]); // Anzahl der Spalten
 Gtk::Table* table = manage(new Gtk::Table(2,cls));
 Gtk::EventBox *ev = manage(new Gtk::EventBox());
 ev->add(*table);

 assert(!combos[l].size());
 assert(!labels[l].size());
 unsigned i=0; 
 for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz[l]);j!=schema->sigend(signifikanz[l]);++j)
   {
    Gtk::SearchCombo *sc;
    combos[l].push_back(sc=manage (new Gtk::SearchCombo(true,autocompletebool)));
    sc->set_size_request(50,-1);
//    sc->set_autoexpand(autocompletebool);
    sc->set_enable_tab(true);
    sc->signal_search().connect(SigC::bind(SigC::slot(*this,&ArtikelBox::searchFunc),i,l));
    sc->signal_activate().connect(SigC::bind(SigC::slot(*this,&ArtikelBox::selectFunc),i,l));
    sc->get_entry()->signal_focus_in_event().connect(SigC::bind(SigC::slot(*this,&ArtikelBox::FocusInFunc),i,l));

    Gtk::Label *lb;

    std::string text;
    if(kombiniertbool) text = kombinierterName(schema,signifikanz[l]);
    else text = j->bezkomptext;
    labels[l].push_back(lb=manage(new Gtk::Label(text)));

    if (i==0&&l==0) 
     {
       Gtk::HBox *hb= manage(new Gtk::HBox());
       pixmap= manage(new class Gtk::Image());
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
 ev->signal_button_press_event().connect(SigC::slot(*this,&ArtikelBox::MouseButton));
 std::string labtext = ArtikelTyp::get_string(schema->Typ())
      +": "+ cH_Kunde(schema->Id())->firma();
 label_typ = manage(new class Gtk::Label(labtext));
 table->attach(*label_typ,0,cls,2,3);

// EIBox = createEIBox();
// table->attach(*EIBox,0,cls,3,4);

 table->show();
 ev->show();
 active_sc=0;
 return ev;
}

#ifdef MABELLA_EXTENSIONS
#define RESET_ON_FOCUS_DEF true
#else
#define RESET_ON_FOCUS_DEF false
#endif

#define USUAL_INIT(sch) vertikalbool(), autocompletebool(), \
   kombiniertbool(),labelbool(), \
   automatisch_anlegen_bool(),eingeschraenkt(),\
   artikel_automatisch_finden(), reset_on_focus(RESET_ON_FOCUS_DEF), \
   block_selection(), \
  schema(sch), gewaehltesSchema(sch->Id()), tr("",false), tr2("",false),\
  oberstes(), menu(),  pixmap(), label_typ(), label(),  \
  active_sc(), artikel_anlegen_funcptr(&ArtikelBox::Neuer_Eintrag_ext), \
  artikel_anlegen_gptr()

ArtikelBox::ArtikelBox(const cH_ExtBezSchema &_schema)  throw(SQLerror)
: USUAL_INIT(_schema)
{ 
 artbox_start();
}

ArtikelBox::ArtikelBox(const std::string& _program,const std::string& _position)  throw(SQLerror)
: USUAL_INIT(cH_ExtBezSchema(ExtBezSchema::default_ID))
{ 
  sprogram=_program;
  sposition=_position;
  artbox_start();
  std::string gs = Global_Settings(0,sprogram,sposition).get_Wert();
  if (gs!="")
   {
     std::string sep=":";
     std::string::size_type  p=gs.find(sep);
     if(p==std::string::npos) return;
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
     {  std::cout <<"Global_Settings konnte nicht korrekt ausgewert: "<<e << '\n';
     }
   }
}

void ArtikelBox::artbox_start()
{
#ifdef PETIG_EXTENSIONS
 artikel_automatisch_finden=true;
#endif
 init();
 this->signal_button_press_event().connect(SigC::slot(*this,&ArtikelBox::MouseButton));

 // redirect our grab_focus
 gtk_signal_connect_after(GTK_OBJECT(gobj()), "grab_focus",
                 GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
}

void ArtikelBox::setExtBezSchema(const cH_ExtBezSchema &_schema)
{
   schema=_schema;
   init();
}

void ArtikelBox::setExtBezSchemaID(ExtBezSchema::ID id)
{  gewaehltesSchema=id;
   if (id==schema->Id()) return; // vielleicht gewähltesSchema setzen?

   // replace this by a decent database driven last selected solution
   // the API is already there
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
   ArtikelBox::setExtBezSchema(cH_ExtBezSchema(id,ArtikelTypID::Band));
#else
   ArtikelBox::setExtBezSchema(cH_ExtBezSchema(id));
#endif
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

bool ArtikelBox::MouseButton(GdkEventButton *event)
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
   std::vector<cH_EntryValue> v;
   guint l,sp;
   if (determineFocus(l,sp)) v=get_content(l,sp);
   try {
      setExtBezSchema(cH_ExtBezSchema(schema->Id(),t));
   } catch (SQLerror &e)
   {  setExtBezSchema(cH_ExtBezSchema(ExtBezSchema::default_ID,t));
   }
   set_content(v,0); // perhaps 0 is not the best choice ...
   if (sprogram!="")
      Global_Settings::create(getuid(),sprogram,sposition,itos(schema->Id())+":"+itos(t));
}

void ArtikelBox::setzeSchemaTyp(int t2)
{  
   Benutzerprofil_speichern();
   std::vector<cH_EntryValue> v;
   guint l,sp;
   if (determineFocus(l,sp)) v=get_content(l,sp);
   setExtBezSchema(cH_ExtBezSchema(t2,schema->Typ()));
   set_content(v,0);
   if (sprogram!="")
      Global_Settings::create(getuid(),sprogram,sposition,itos(t2)+":"+itos(schema->Typ()));
}


void ArtikelBox::setzeSignifikanz(int t)
{  
  bool add = true;
  for (std::vector<int>::iterator i=signifikanz.begin();std_neq(i,signifikanz.end());++i)
    if ( (*i)==t ) { signifikanz.erase(i); add=false; break;}
  if (add) signifikanz.push_back(t);
  Benutzerprofil_speichern();
  setExtBezSchema(schema);
}


void ArtikelBox::Autocomplete(Gtk::CheckMenuItem *autocomplete)
{ 
  autocompletebool=autocomplete->get_active();
  set_autoexpand(autocompletebool);
  Benutzerprofil_speichern();
}  

void ArtikelBox::ResetOnFocusCMI(Gtk::CheckMenuItem *rof)
{ 
  reset_on_focus=rof->get_active();
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

static const unsigned char E_png_data[] = 
{       137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,
        0,0,0,14,0,0,0,14,8,3,0,0,0,40,150,221,
        227,0,0,0,180,80,76,84,69,0,0,0,248,0,0,94,
        0,0,74,0,0,54,0,0,51,0,0,182,0,0,105,0,
        0,162,0,0,219,0,0,85,0,0,159,0,0,25,0,0,
        216,0,0,196,0,0,253,0,0,119,0,0,156,0,0,2,
        0,0,56,0,0,113,0,0,16,0,0,187,0,0,244,0,
        0,167,0,0,33,0,0,224,0,0,144,0,0,201,0,0,
        238,0,0,27,0,0,255,0,0,44,0,0,4,0,0,118,
        0,0,21,0,0,1,0,0,229,0,0,18,0,0,189,0,
        0,226,0,0,92,0,0,206,0,0,72,0,0,146,0,0,
        29,0,0,200,0,0,46,0,0,160,0,0,83,0,0,6,
        0,0,254,0,0,177,0,0,214,0,0,137,0,0,117,0,
        0,40,0,0,20,0,0,211,0,0,77,0,0,131,40,239,
        112,0,0,0,130,73,68,65,84,120,156,101,206,215,18,130,
        48,20,4,208,181,98,84,20,236,125,85,80,131,93,209,216,
        254,255,191,76,8,60,113,103,118,118,206,203,206,5,114,231,
        52,164,148,74,231,102,189,38,187,8,156,66,209,178,198,185,
        169,199,201,82,210,7,154,248,133,137,202,49,35,108,119,217,
        148,199,248,122,220,251,25,91,156,245,15,155,40,213,167,77,
        165,231,198,41,235,100,9,168,96,113,73,56,226,48,208,181,
        170,190,141,238,19,186,186,150,231,111,199,240,37,132,59,85,
        131,167,232,229,223,255,3,59,60,10,90,253,185,43,150,0,
        0,0,0,73,69,78,68,174,66,96,130,
};
static const unsigned E_png_size = 379;
static const unsigned char plus_png_data[] = 
{       137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,
        0,0,0,14,0,0,0,14,4,3,0,0,0,237,102,48,
        226,0,0,0,36,80,76,84,69,0,0,0,28,0,0,85,
        0,0,99,0,0,56,0,0,170,0,0,141,0,0,198,0,
        0,255,0,0,152,0,0,226,0,0,109,0,0,80,192,190,
        145,0,0,0,46,73,68,65,84,120,156,99,96,96,144,158,
        194,0,6,26,173,248,104,214,208,138,174,208,16,6,6,142,
        14,32,104,102,96,96,82,202,104,87,82,196,167,111,6,148,
        70,1,0,8,224,12,142,117,200,35,17,0,0,0,0,73,
        69,78,68,174,66,96,130,
};
static const unsigned plus_png_size = 151;

void ArtikelBox::pixmap_setzen(bool valid)
{  if (valid) pixmap->set(Gtk::Stock::APPLY,Gtk::ICON_SIZE_SMALL_TOOLBAR);
   else if (automatisch_anlegen_bool) 
   {  Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
      loader->write(plus_png_data, plus_png_size);
      loader->close();
      pixmap->set(loader->get_pixbuf());
   }
   else if (eingeschraenkt) 
   {  Glib::RefPtr<Gdk::PixbufLoader> loader=Gdk::PixbufLoader::create();
      loader->write(E_png_data, E_png_size);
      loader->close();
      pixmap->set(loader->get_pixbuf());
   }
   else pixmap->set(Gtk::Stock::CANCEL,Gtk::ICON_SIZE_SMALL_TOOLBAR);
}

void ArtikelBox::Neuer_Eintrag_automatisch(Gtk::CheckMenuItem *cmi)
{
  automatisch_anlegen_bool=cmi->get_active();
  adjust_combo_behaviour();
}

bool ArtikelBox::determineFocus(guint &sigidx_out, guint &entryidx_out) const
{
  for (guint l=0;l<combos.size();++l)
      for (guint sp=0;sp<combos[l].size();++sp)
#if 0 // this simply does not work - don't know why ....
         if (combos[l][sp]->has_focus())
#else
	 if (combos[l][sp]==active_sc)
#endif
         {  sigidx_out=l; entryidx_out=sp;
//std::cerr << "fokus " << l << "," << sp << '\n';
            return true;
         }
   return false;
}

void ArtikelBox::reset()
   { for (t_combos2::iterator j=combos.begin();std_neq(j,combos.end());++j)  
      for (t_combos::iterator i=j->begin();std_neq(i,j->end());++i)
        (*i)->reset(); 
//     eingeschraenkt=false;
//     einschraenkung="";
//     joinstring="";
   }  

void ArtikelBox::set_editable(bool edit)
   { for (t_combos2::iterator j=combos.begin();std_neq(j,combos.end());++j)  
      for (t_combos::iterator i=j->begin();std_neq(i,j->end());++i)
        (*i)->set_editable(edit); }  

void ArtikelBox::set_autoexpand(bool exp)
   { for (t_combos2::iterator j=combos.begin();std_neq(j,combos.end());++j)  
      for (t_combos::iterator i=j->begin();std_neq(i,j->end());++i)
        (*i)->set_autoexpand(exp); }  

void ArtikelBox::set_always_fill(bool fill)
   { for (t_combos2::iterator j=combos.begin();std_neq(j,combos.end());++j)  
      for (t_combos::iterator i=j->begin();std_neq(i,j->end());++i)
        (*i)->set_always_fill(fill); }  

void ArtikelBox::set_focus(int sig, int field)
   {
    assert(combos.size()>sig);
    assert(combos[sig].size()>field);
//    combos[sig][field]->reset();
    combos[sig][field]->grab_focus();
   }
  

void ArtikelBox::set_automatisch_anlegen(bool b)
{ if(b && !neuanlegen()) return;
  automatisch_anlegen_bool=b;
  fuelleMenu();
  adjust_combo_behaviour();
}

void ArtikelBox::adjust_combo_behaviour()
{  // z.B. search
 for (t_combos2::iterator j=combos.begin();std_neq(j,combos.end());++j)
  for (t_combos::iterator i=j->begin(); std_neq(i,j->end()); ++i)
   {  (*i)->set_value_in_list(!automatisch_anlegen_bool,automatisch_anlegen_bool);
   }
}

static bool select_all_text(Gtk::Editable *e)
{  e->select_region(0,-1);
   return false;
}

bool ArtikelBox::FocusInFunc(GdkEventFocus *ev, guint sp, guint l)
{  if (block_selection) return true;
   if (reset_on_focus) combos[l][sp]->reset();
   else 
   {  // HACK: show available options (even nonmatching and enable overwrite)
      combos[l][sp]->get_entry()->select_region(0,-1);
      combos[l][sp]->gobj()->value_selected=true;
      combos[l][sp]->trigger_search();
      // wenn es durch Mausclick passiert ist
      Glib::signal_idle().connect(SigC::bind(SigC::slot(&select_all_text),combos[l][sp]->get_entry()));
   }
   return true;
}

void ArtikelBox::Benutzerprofil_laden()
{ ArtikelBoxAnzeige anz=ArtikelBoxAnzeige::Laden(schema);
  setzeAnzeige(anz);
}

bool ArtikelBox::neuanlegen() const 
{  return Query("select exists(select true from artbez_insert where tabelle='"+
		Tabellenname(schema)+"')").FetchOne<bool>();
}

void ArtikelBox::Benutzerprofil_speichern()
{ 
  ArtikelBoxAnzeige anz(schema);
  for(std::vector<int>::const_iterator i=signifikanz.begin();i!=signifikanz.end();++i)
     anz.Sichtbar(*i,true);
  anz.AutoComplete(autocompletebool);
  anz.Kombiniert(kombiniertbool);
  anz.reset_on_focus=reset_on_focus;
  anz.Speichern();
}

void ArtikelBox::Neuer_Eintrag()
{ /* testen, ob Artikel schon existiert, warum nicht anzeigen? */
  if(artikel_exist(false)!=ArtikelBase::none_id) 
    {
      std::cerr<<"FEHLER: Artikel existiert schon\n";
      return; 
    }
  std::map<int,std::vector<cH_EntryValue> > felder;
  
  for (unsigned int j=0;j<combos.size();++j)
  { for (unsigned int i=0; i<combos[j].size(); ++i)
      felder[signifikanz[j]].push_back(cH_EntryValueIntString(combos[j][i]->get_text()));
  }
  ArtikelBase newart=(*artikel_anlegen_funcptr)(artikel_anlegen_gptr,schema,felder);
  if (!!newart) set_value(newart);
}

ArtikelBase ArtikelBox::Neuer_Eintrag_ext(gpointer ud,cH_ExtBezSchema const&s,
	    std::map<int,std::vector<cH_EntryValue> > const& felder)
{
  TagStream ts;
  Tag &neu_anlegen=ts.push_back(Tag("neu_anlegen"));
  neu_anlegen.setAttr("warengruppe",ManuProC::create<int>(s->Typ().Id()));
  neu_anlegen.setAttr("schema",ManuProC::create<int>(s->Id()));

  for (std::map<int,std::vector<cH_EntryValue> >::const_iterator j=felder.begin()
        ;j!=felder.end();++j)
  { Tag &part=neu_anlegen.push_back(Tag("part"));
    part.setAttr("signifikanz",ManuProC::create<int>(j->first));
    for (std::vector<cH_EntryValue>::const_iterator i=j->second.begin()
        ; i!=j->second.end(); ++i)
    { part.push_back(Tag("content",(*i)->getStrVal()));
    }
  }
  mystream ms;
  ts.write(ms,true);
  std::string arg=ms.str();
  if (!fork())
  { execlp("artikeleingabe","artikeleingabe",arg.c_str(),0);
    perror("artikeleingabe");
    exit(errno);
  }
  return ArtikelBase();
}

void ArtikelBox::where_what(std::string& where, std::string& what, bool jumbo)
{ unsigned l=0; 
  for (std::vector<int>::const_iterator i=signifikanz.begin();
  		i!=signifikanz.end(); ++i,++l)
   { std::vector<cH_EntryValue> v=get_content(l);
     unsigned spalte=0;   
     for(ExtBezSchema::const_sigiterator j=schema->sigbegin(signifikanz[l]);
     	   j!=schema->sigend(signifikanz[l]); ++spalte,++j)
      {  if (j->spaltenname!="id")
         {  where += j->spaltenname+",";
#ifdef PETIG_EXTENSIONS
            if (jumbo && j->spaltenname==schema->JumboSpalte())
               what += "'"+schema->JumboBez()+"'," ;
            else 
#endif
               what += "'"+v[spalte]->getStrVal()+"',";
         }
      }
   }
   if (!where.empty() && where[where.size()-1]==',') where[where.size()-1]=')';
   if (!what.empty() && what[what.size()-1]==',') what[what.size()-1]=')';
}

ArtikelBase::ID ArtikelBox::artikel_exist(bool jumbo) 
{ std::string squery = "select id from "+Tabellenname(schema)+" where ";
  std::string where = "(";
  std::string what = "(";
  where_what(where,what,jumbo);
  squery += where + '=' + what;
 
  try{
     return Query(squery).FetchOne<int>();
  }catch(SQLerror &e) {return ArtikelBase::none_id; }
}

ArtikelBoxAnzeige ArtikelBoxAnzeige::Laden(cH_ExtBezSchema schema)
{  ArtikelBoxAnzeige res=ArtikelBoxAnzeige(schema);

   std::string qual=itos(schema->Typ().Id())+"_"+itos(schema->Id());
   std::string val=Global_Settings(getuid(),"ArtikelBox",qual).get_Wert();
   if (!val.empty())
   {  res.autocomplete=val.find('a')!=std::string::npos;
         res.kombiniert=val.find('k')!=std::string::npos;
#ifdef MABELLA_EXTENSIONS         
         res.reset_on_focus=val.find('R')==std::string::npos;
#else
	 res.reset_on_focus=false;         
#endif         
         std::string::size_type komma1=val.find(','),komma2=std::string::npos;
         if (komma1!=std::string::npos) komma2=val.find(',',komma1+1);
         if (komma2!=std::string::npos)
         {  res.anzeige_pos=strtol(val.substr(0,komma1).c_str(),0,10);
            res.anzeige_neg=strtol(val.substr(komma1+1,komma2-(komma1+1)).c_str(),0,10);
         }
   }
   else
   {  res.anzeige_pos=1;
   }
   return res;
}

void ArtikelBoxAnzeige::Speichern() const
{  std::string qual=itos(schema->Typ().Id())+"_"+itos(schema->Id());
   std::string val=itos(anzeige_pos)+','+itos(anzeige_neg)+','
   	+(autocomplete?"a":"")+(kombiniert?"k":"")+(!reset_on_focus?"R":"");
   Global_Settings::create(getuid(),"ArtikelBox",qual,val);
}

std::string ArtikelBox::Tabellenname(cH_ExtBezSchema s)
{  return "artbez_"+itos(s->Typ().Id())+"_"+itos(s->Id());
}

unsigned ArtikelBox::getSignifikanzPos(int sig)
{ for (unsigned i=0;i<signifikanz.size();++i)
    if (signifikanz[i]==sig) return i;
  throw std::domain_error("signifikanz nicht sichtbar");
}
