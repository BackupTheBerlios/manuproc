// $Id: WTelefon.cc,v 1.15 2006/10/31 16:10:11 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig
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
#include <Misc/i18n.h>
#include <gtkmm/adjustment.h>
#include "WTelefon.hh"
#include <Gtk_OStream.h>
#include <gtkmm/viewport.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/button.h>
#include <Aux/itos.h>

#include <Aux/EntryValueIntString.h>
#include <SelectMatching.h>
#include <sigc++/slot.h>
#include <sigc++/compatibility.h>
#include <gtk/gtksignal.h>

#include "MyMessage.h"
#include <sys/types.h> 
#include <regex.h>

class Data_Tel :  public RowDataBase
{
      cH_Telefon T;
   public:
      Data_Tel(cH_Telefon t) : T(t){}
      enum Spalten {NUMMER,ART};
      virtual cH_EntryValue Value(guint seqnr,gpointer gp) const
       {
         switch((Spalten)seqnr)
          {
            case NUMMER :  return cH_EntryValueIntString(T->Text());
            case ART    :  
               std::string s=T->ArtString();
               if(T->isPrivat()) s+=_(" (privat)");
               return cH_EntryValueIntString(s);
          }
         return cH_EntryValueIntString();
       }
   cH_Telefon getTelefon() const {return T;}
};

class cH_Data_Tel : public Handle<const Data_Tel>
{
 public:
   cH_Data_Tel(const Data_Tel *r) : Handle<const Data_Tel>(r) {}
};

bool operator==(void *a, TelArt b)
{  return a==(void*)b;
}

// ich vermute, dies wird nicht verwendet ... 
void WTelefon::set_value(const cH_Telefon &t)
{
  _land->set_text(itos(t->Nummer().land));
  _vorwahl->set_text(itos(t->Nummer().vorwahl));
  _nummer->set_text(itos(t->Nummer().nummer));
  _durchwahl->set_text(t->Nummer().durchwahl==-1 ? "" : itos(t->Nummer().durchwahl));
  textfeld->set_text(t->getText());
  Gtk::Menu_Helpers::SelectMatching(*option_menu,t->Nummer().art);
  felder_anpassen(t->Nummer().art);
}

void WTelefon::felder_anpassen(TelArt art)
{ if (art==TEL_E_MAIL || art==TEL_HOMEPAGE)
  { //text_box->show();
    telnummer_box->hide();
  }
  else
  { //text_box->hide();
    telnummer_box->show();
  }
}

void WTelefon::anderer_typ()
{ TelArt typ=TelArt(reinterpret_cast<long>(const_cast<Gtk::MenuItem*>(option_menu->get_menu()->get_active())->get_data("user_data")));
  felder_anpassen(typ);
std::cout << "andere_typ()\n";
}

gint WTelefon::try_grab_focus(GtkWidget *w,gpointer gp)
{ 
   WTelefon *this2((WTelefon*)gp);
   assert(dynamic_cast<Gtk::Table*>(this2)); // very weak check
 if (this2->_land->is_visible()) 
 { this2->_land->grab_focus();
   this2->_land->select_region(0,this2->_land->get_text_length());
 }
 else 
 { this2->textfeld->grab_focus();
   this2->textfeld->select_region(0,this2->textfeld->get_text_length());
 }
   return true;
}

WTelefon::WTelefon()
{
  {Gtk::OStream oms(option_menu);
  std::vector<std::pair<std::string,TelArt> > VT=Telefon::getTelArtVec();
  for(std::vector<std::pair<std::string,TelArt> >::const_iterator i=VT.begin();i!=VT.end();++i)
   {
     oms<<i->first;
     oms.flush(gpointer(i->second));
   }
  }
  option_menu->get_menu()->signal_deactivate().
      connect(SigC::slot(*this, &WTelefon::anderer_typ));

  setTitels();

  clear();
  gtk_signal_connect_after(GTK_OBJECT(gobj()), "grab_focus",GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
}

void WTelefon::on_buttonNumLoeschen_clicked()
{
  cH_Telefon T(Telefon::none_id);
  if(getSelectedTel(T));
   {
     Telefon::delTelefon(T->Id());
     TelList.remove(T);
     showTel();
     _remove(T);
   }
std::cout << "loschen\n";   
}


void WTelefon::setTitels()
{
  std::vector<std::string> s;
  s.push_back(_("Nummer"));
  s.push_back(_("Art"));
  tree->setTitles(s);
}

void WTelefon::showTel(std::list<cH_Telefon> VT)
{
 TelList=VT;
 showTel();
}

void WTelefon::showTel()
{
 clear(false);
 std::vector<cH_RowDataBase> datavec;
 for(std::list<cH_Telefon>::const_iterator i=TelList.begin();
 	i!=TelList.end();++i)
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


cH_Telefon WTelefon::get_value() const
{ 
  int durchwahl=-1;
  TelArt typ=TelArt(reinterpret_cast<long>(const_cast<Gtk::MenuItem*>(option_menu->get_menu()->get_active())->get_data("user_data")));

  if (!_durchwahl->get_text().empty()) 
  	durchwahl=atoi(_durchwahl->get_text().c_str());
  return cH_Telefon(typ,kundennr,persnr,
  		atoi(_land->get_text().c_str()),
		atoi(_vorwahl->get_text().c_str()),
		atoi(_nummer->get_text().c_str()),
		durchwahl,
		textfeld->get_text());
}

void WTelefon::land_activate()
{
  _vorwahl->grab_focus();
  _vorwahl->select_region(0,_vorwahl->get_text_length());
}

void WTelefon::vorwahl_activate()
{
  _nummer->grab_focus();
  _nummer->select_region(0,_nummer->get_text_length());
}

void WTelefon::nummer_activate()
{
  _durchwahl->grab_focus();
  _durchwahl->select_region(0,_durchwahl->get_text_length());
}

void WTelefon::durchwahl_activate()
{
 text_activate();
}


void WTelefon::text_activate()
{ 
  try{
  cH_Telefon newt(get_value());

  if(newt->TelefonArt()==TEL_E_MAIL)
    {
     char *mail_regexp(
"^([a-zA-Z0-9_\\.\\-])+@(([a-zA-Z0-9\\-])+\\.)+([a-zA-Z0-9]{2,4})+$"
	);
     regex_t *re=(regex_t *)malloc(sizeof(regex_t));
   int ret=regcomp(re,mail_regexp,REG_ICASE|REG_EXTENDED);

//   Gtk::Window *pw=this->get_parent()->get_window();

   if(ret!=0)
     {
//     MyMessage::show_and_wait(_("ERROR: Überprüfung der Emailaddresse nicht mögllich"),
//			this);
       std::cout << "ERROR: Überprüfung der Emailaddresse nicht mögllich\n";
     }
   if(regexec(re, newt->Text().c_str(),0, NULL, 0)==REG_NOMATCH)
     {//MyMessage::show_and_wait(_("Bitte die Emailaddresse korrigieren"),
//			this);
      std::cout << "Bitte die Emailaddresse korrigieren\n";
      return;
     } 
    }

  cH_Telefon ct=Telefon::create(newt);
  TelList.push_back(ct);
  showTel();
  _add(ct);  
  }catch(SQLerror &e){std::cerr << e<<'\n';}
}

  
  
void WTelefon::clear(bool withtree) const
{
#ifdef MABELLA_EXTENSIONS
 _land->set_text("");
 _vorwahl->set_text("");
#else
 _land->set_text(itos(Telefon::Landeskennzahl));
 _vorwahl->set_text(itos(Telefon::Vorwahl));
#endif
 _nummer->set_text("");
 _durchwahl->set_text("");
 textfeld->set_text("");
 if(withtree)
   tree->clear();
}

