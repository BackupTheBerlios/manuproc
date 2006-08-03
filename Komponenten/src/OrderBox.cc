// $Id: OrderBox.cc,v 1.6 2006/08/03 11:57:22 christof Exp $
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

#include <OrderBox.hh>
#include <Auftrag/AuftragBase.h>
#include <Misc/Date.h>
#include <Misc/ExtraColumns.h>

OrderBox::OrderBox(int _instanz, what_t w)
  : customer(), what(w), query(), tr("",false), 
instanz(_instanz), extra_string(), search_th(0)
{ get_entry()->set_width_chars(10);
//  signal_activate().connect(SigC::slot(*this, &auftrag_bearbeiten_glade::on_aufnrscombo_activate));
//  signal_search().connect(sigc::mem_fun(*this, &OrderBox::on_search), false);
  extra_string=ExtraColumns("auftrag","instanz","auftragid").hasColumn("label");
}

void OrderBox::Instanz(int inst, what_t w)
{ if (inst==instanz && what==w) return;
  reset();
  instanz=inst;
  what=w;
  reset(); // just to make sure
}

void OrderBox::on_search(int *_continue, GtkSCContext newsearch) throw()
{
 switch(newsearch)
   {case GTK_SEARCH_REOPEN:
    case GTK_SEARCH_OPEN :
#if 0    
      // HACK
//      if (query)
      { delete query;
         tr.commit();
         query=0;
      }
#endif
//      if (!query) // HACK
      {std::string extracol,join;

       if(get_text().size()<search_th) 
	{*_continue=false;
	 return;}
      
       if (extra_string)
       { extracol=",kurz";
         join=" left join auftrag_label on (alid=label)";
       }
       tr.open();
       assert(!query);
       query = new Query("Order"+itos(long(this)),
          "select auftragid, ltrim(to_char(auftragid,'000000')),"
               "cast(datum as date)"+extracol+" "
               "from auftrag"+join+" "
               "where ltrim(to_char(auftragid,'000000')) like ? "
               "and (?<1 or kundennr=?) and instanz=? "
               "and auftragid>=? order by datum desc");
       (*query) << ((newsearch==GTK_SEARCH_OPEN?get_text():Glib::ustring())+"%") 
          << customer << customer
          << instanz << AuftragBase::handplan_id;
      }
      // fall through
  case GTK_SEARCH_FETCH :
       {Query::Row rw;
        if (((*query)>>rw).good())
        { int aufid; std::string text, label;
          ManuProC::Date d;
          
          rw >> aufid >> text >> d;
          if (extra_string) rw >> Query::Row::MapNull(label);
	  add_item(text+" "+(label.empty()?"          ":label)+" "+d.to_locale(),aufid);
  	  *_continue=true;
        }
        else *_continue=false;
        break;
       }
 
  case GTK_SEARCH_CLOSE :
//       if (!query)
       { delete query;
         tr.commit();
         query=0;
       }
       break;
  }
}

