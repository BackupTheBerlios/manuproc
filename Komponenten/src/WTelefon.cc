#include <gtk--/adjustment.h>
#include "WTelefon.hh"
#include <Gtk_OStream.h>
#include <gtk--/viewport.h>
#include <gtk--/scrolledwindow.h>
#include <gtk--/button.h>

void WTelefon::setNumber(int land,int vorwahl,int nummer)
{
  spinbutton_land->set_value(land);
  spinbutton_vorwahl->set_value(vorwahl);
//  spinbutton_nummer->set_value(nummer);
  spinbutton_nummer->set_text(itos(nummer));
}

gint WTelefon::try_grab_focus(GtkWidget *w,gpointer gp)
{ 
   WTelefon *this2((WTelefon*)gp);
   assert(Gtk::Table::isA(this2)); // very weak check
   this2->spinbutton_land->grab_focus();
   this2->spinbutton_land->select_region(0,-1);
   return true;
}

WTelefon::WTelefon()
{
  option_menu = manage(new class Gtk::OptionMenu());
  Gtk::OStream oms(option_menu);
  vector<pair<std::string,TelArt> > VT=Telefon::getTelArtVec();
  for(vector<pair<std::string,TelArt> >::const_iterator i=VT.begin();i!=VT.end();++i)
   {
     oms<<i->first;
     oms.flush(gpointer(i->second));
   }

  tree=manage(new class SimpleTree(2,2));
  setTitels();
  Gtk::Viewport *viewport = manage(new class Gtk::Viewport());
  viewport->set_shadow_type(GTK_SHADOW_NONE);
  viewport->add(*tree);
  Gtk::ScrolledWindow *scrolledwindow = manage(new class Gtk::ScrolledWindow());
  scrolledwindow->set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  scrolledwindow->add(*viewport);

  Gtk::Button *buttonNumLoeschen = manage(new class Gtk::Button("alte Nummer löschen"));


  Gtk::Frame *frame_land=manage(new class Gtk::Frame("Landeskennzahl"));
  Gtk::Adjustment *spinbutton_land_adj = manage(new class Gtk::Adjustment(0, 49, 9999, 1, 10, 10));
  spinbutton_land = manage(new class Gtk::SpinButton(*spinbutton_land_adj, 1, 0));
  spinbutton_land->activate.connect_after(SigC::slot(this,&WTelefon::spinbutton_land_activate));
  frame_land->set_label_align(0, 0);
  frame_land->set_shadow_type(GTK_SHADOW_ETCHED_IN);
  frame_land->add(*spinbutton_land);

  Gtk::Frame *frame_vorwahl=manage(new class Gtk::Frame("Vorwahl   "));
  Gtk::Adjustment *spinbutton_vorwahl_adj = manage(new class Gtk::Adjustment(0, 0, 9999999, 1, 10, 10));
  spinbutton_vorwahl = manage(new class Gtk::SpinButton(*spinbutton_vorwahl_adj, 1, 0));
  spinbutton_vorwahl->activate.connect_after(SigC::slot(this,&WTelefon::spinbutton_vorwahl_activate));
  frame_vorwahl->set_label_align(0, 0);
  frame_vorwahl->set_shadow_type(GTK_SHADOW_ETCHED_IN);
  frame_vorwahl->add(*spinbutton_vorwahl);

  Gtk::Frame *frame_nummer=manage(new class Gtk::Frame("Nummer         "));
//  Gtk::Adjustment *spinbutton_nummer_adj = manage(new class Gtk::Adjustment(0, 0, 999999999, 1, 10, 10));
//  spinbutton_nummer = manage(new class Gtk::SpinButton(*spinbutton_nummer_adj, 1, 0));
  spinbutton_nummer = manage(new class Gtk::Entry(8));
  spinbutton_nummer->activate.connect_after(SigC::slot(this,&WTelefon::spinbutton_nummer_activate));
  frame_nummer->set_label_align(0, 0);
  frame_nummer->set_shadow_type(GTK_SHADOW_ETCHED_IN);
  frame_nummer->add(*spinbutton_nummer);

  Gtk::Table *subtab=manage (new class Gtk::Table(3,1));
  Gtk::Frame *frame_neu=manage(new class Gtk::Frame("Neue Nummer eingeben"));
  subtab->attach(*option_menu, 0, 1, 0, 1, 0, 0, 0, 0);
  subtab->attach(*frame_land, 1, 2, 0, 1, GTK_FILL, 0, 0, 0);
  subtab->attach(*frame_vorwahl, 2, 3, 0, 1, GTK_FILL, 0, 0, 0);
  subtab->attach(*frame_nummer, 3, 4, 0, 1, GTK_FILL, 0, 0, 0);
  frame_neu->set_label_align(0, 0);
  frame_neu->set_shadow_type(GTK_SHADOW_ETCHED_IN);
  frame_neu->add(*subtab);
        

  //Horizontal
  attach(*frame_neu, 0, 1, 0, 1, GTK_FILL, 0, 0, 0);
  attach(*scrolledwindow, 0, 1, 1, 2, GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL, 0, 0);
  attach(*buttonNumLoeschen, 0, 1, 2, 3, 0, 0, 0, 0);

  buttonNumLoeschen->clicked.connect(SigC::slot(static_cast<class WTelefon*>(this), &WTelefon::on_buttonNumLoeschen_clicked));

  clear();
  gtk_signal_connect(GTK_OBJECT(gtkobj()), "grab_focus",GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
  show_all();
}

void WTelefon::on_buttonNumLoeschen_clicked()
{
  cH_Telefon T(Telefon::none_id);
  if(getSelectedTel(T));
   {
     Telefon::delTelefon(T->Id());
     TelList.remove(T);
     showTel();
   }
}


void WTelefon::setTitels()
{
  vector<std::string> s;
  s.push_back("Nummer");
  s.push_back("Art");
  tree->setTitles(s);
}

void WTelefon::showTel(std::list<cH_Telefon> VT)
{
 TelList=VT;
 showTel();
}

void WTelefon::showTel()
{
 std::vector<cH_RowDataBase> datavec;
 for(std::list<cH_Telefon>::const_iterator i=TelList.begin();i!=TelList.end();++i)
  {
    datavec.push_back(new Data_Tel(*i));
  }
 tree->setDataVec(datavec);
}
         

bool WTelefon::getSelectedTel(cH_Telefon &T) const
{
  try{
    cH_Data_Tel dt(tree->getSelectedRowDataBase_as<cH_Data_Tel>());
    T=const_cast<cH_Data_Tel&>(dt)->getTelefon();
  }catch(std::exception &e){return false;} 
 return true;
}


Telefon::st_nummer WTelefon::get_value() const
{
  TelArt TA = (TelArt) int(option_menu->get_menu()->get_active()->get_user_data());
  return Telefon::st_nummer(getLand(),getVorwahl(),getNummer(),TA);
}

int WTelefon::getLand() const
{
  gtk_spin_button_update(spinbutton_land->gtkobj());
  return spinbutton_land->get_value_as_int();
}

int WTelefon::getVorwahl() const
{
  gtk_spin_button_update(spinbutton_vorwahl->gtkobj());
  return spinbutton_vorwahl->get_value_as_int();
}

int WTelefon::getNummer() const
{
//  gtk_spin_button_update(spinbutton_nummer->gtkobj());
//  return spinbutton_nummer->get_value_as_int();
  std::string s=spinbutton_nummer->get_text();
  return atoi(s.c_str());
}


void WTelefon::spinbutton_land_activate()
{
  spinbutton_vorwahl->grab_focus();
  spinbutton_vorwahl->select_region(0,-1);
}

void WTelefon::spinbutton_vorwahl_activate()
{
  spinbutton_nummer->grab_focus();
  spinbutton_nummer->select_region(0,-1);
}

void WTelefon::spinbutton_nummer_activate()
{
  activate();
}
  
  
void WTelefon::clear() const
{
 spinbutton_land->set_value(Telefon::Landeskennzahl);
 spinbutton_vorwahl->set_value(Telefon::Vorwahl);
// spinbutton_nummer->set_value(0);
 spinbutton_nummer->set_text("");
 spinbutton_land->grab_focus();
 tree->clear();
}
