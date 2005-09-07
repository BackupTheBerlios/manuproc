// $Id: Optionmenu_Lager.cc,v 1.3 2005/09/07 14:18:06 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#include "Optionmenu_Lager.hh"
#include <Gtk_OStream.h>
#include <SelectMatching.h>


Optionmenu_Lager::Optionmenu_Lager()
{ fuelle_menu();
  get_menu()->signal_deactivate().connect(activate.slot());
}

const FertigWarenLager Optionmenu_Lager::get_value() const
{
 return *(FertigWarenLager*)(const_cast<Gtk::MenuItem*>
       (get_menu()->get_active())->get_data("user_data"));
}

void Optionmenu_Lager::fuelle_menu()
{

 int defaultidx=0;
 Gtk::OStream t_(this);
try
 { 
 Query q("select lagerid,bezeichnung from lager order by lagerid");

 FetchIStream fi=q.Fetch();

 int count=0;

 while(fi.good())
   {
    unsigned int lid;
    std::string lbez;
    fi >> lid >> lbez;
    t_ << lbez;
    t_.flush((gpointer)lid);
    if(lid==FertigWarenLager::default_lagerid)
      defaultidx=count;
    count++;
    
    fi=q.Fetch();
   }
 }
 catch (SQLerror &e)
 {  t_ << e.Message();
 }
 set_history(defaultidx);
}

static bool operator==(gpointer data, const FertigWarenLager &lager)
{ 
  return (long)(data)==lager.Id();
}

void Optionmenu_Lager::set_value(const FertigWarenLager &lager)
{  Gtk::Menu_Helpers::SelectMatching(*this,lager);
}
