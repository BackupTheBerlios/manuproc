// $Id: Optionmenu_AuftragLabel.cc,v 1.2 2005/09/07 13:27:37 christof Exp $
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

#include "Optionmenu_AuftragLabel.hh"
#include <Gtk_OStream.h>
#include <SelectMatching.h>
#include <Misc/FetchIStream.h>

Optionmenu_AuftragLabel::Optionmenu_AuftragLabel()
{ fuelle_menu();
  get_menu()->signal_deactivate().connect(activate.slot());
}

int Optionmenu_AuftragLabel::get_value() const
{
 return (int)(get_menu()->get_active()->get_user_data());
}

void Optionmenu_AuftragLabel::fuelle_menu()
{

 int defaultidx=0;
 Gtk::OStream t_(this);
try
 { 
 Query q("select alid,bezeichnung,kurz from auftrag_label order by bezeichnung");

 FetchIStream fi=q.Fetch();

 int count=0;

 while(fi.good())
   {
    std::string bez;
    std::string kurz;
    unsigned int id;    
    fi >> id >> bez >> kurz;
    t_ << "("+(id==0?"  ":kurz)+") "+bez;
    t_.flush((gpointer)id);
    if(id==0)
      set_history(count);
    count++;
    
    fi=q.Fetch();
   }
 }
 catch (SQLerror &e)
 {  t_ << e.Message();
 }
}

/*static bool operator==(gpointer data, int alid)
{ 
  return (int)(data)==alid;
}
*/

void Optionmenu_AuftragLabel::set_value(int alid)
{  Gtk::Menu_Helpers::SelectMatching(*this,(gpointer)alid);
}

