// $Id: EingabeBox.cc,v 1.15 2004/05/06 13:56:16 jacek Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++ Copyright (C)
 *  1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include "EingabeBox.hh"
#include <gtk/gtksignal.h>
#include <cassert>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#  include <sigc++/bind.h>
#endif

gint EingabeBox::try_grab_focus(GtkWidget *w,gpointer gp)
{  assert(dynamic_cast<Gtk::Table*>((Glib::Object*)gp)); // very weak check
   ((EingabeBox*)gp)->grab_focus_tried();
   return true;
}

void EingabeBox::check() const
{  assert(labels.size()==entries.size());
   assert(cons.size()==entries.size());
   assert(visible_size<=entries.size());
}

void EingabeBox::grow(int cols)
{  assert(cols>=0);
   check();
   
   // make invisible columns visible
   {  int already_there(cols>=entries.size()?entries.size():cols);
      if (visible_size<already_there) set_size(already_there);
   }
   // do we have enough columns ?
   if (cols<=labels.size()) return;
   
   last_con.disconnect();
   int newpos(0);
   while ((newpos=labels.size())<cols)
   {  Gtk::Label *l=new Gtk::Label();
      Gtk::Entry *e=new Gtk::Entry();
      l->set_alignment(0.5, 0.5);
      e->set_editable(true);
      //e->set_usize(30,-1); // make width adjustable, strange
      l->show();
      e->show();
      attach(*l,newpos,newpos+1,0,1,Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL,Gtk::AttachOptions(0));
      attach(*e,newpos,newpos+1,1,2,Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL,Gtk::AttachOptions(0));
      labels.push_back(l);
      entries.push_back(e);
      // register our grab_focus
      if (newpos)
	 cons.push_back(entries[newpos-1]->signal_activate().connect(SigC::slot(*e,&Gtk::Widget::grab_focus)));
      else
         cons.push_back(grab_focus_tried.connect(SigC::slot(*e,&Gtk::Widget::grab_focus)));
   }
   // register our activate
   if (newpos)
      last_con=entries[newpos-1]->signal_activate().connect(SigC::slot(*this,&EingabeBox::activate));
   else
      last_con=grab_focus_tried.connect(SigC::slot(*this,&EingabeBox::activate));
   visible_size=cols;
   assert(labels.size()==cols);
   check();
} 

EingabeBox::EingabeBox(int cols)
	: Gtk::Table(cols), visible_size(0)
{  grow(cols);
//   show();
   gtk_signal_connect_after(GTK_OBJECT(gobj()), "grab_focus",
    		GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
//   assert(Gtk::Table::isGtkTable((Gtk::Object *)(gpointer)this));

 int j=0;
 for(t_entries::iterator i=entries.begin();i!=entries.end();++i)
   (*i)->signal_activate().connect(SigC::bind(_activate_entry.slot(),j++));
}



//void EingabeBox::on_entry_activate(int i)
//{
// std::cout << "activated " << i << " \n";
//}


EingabeBox::~EingabeBox()
{  for (t_labels::iterator i=labels.begin();i!=labels.end();++i)
	delete *i;
   for (t_entries::iterator i=entries.begin();i!=entries.end();++i)
	delete *i;
}

const std::string EingabeBox::get_label(int col) const
{  if (col<0 || col>=labels.size()) return "";
   return labels[col]->get_text(); 
}

void EingabeBox::set_label(int col,const std::string &s)
{  grow(col+1); labels[col]->set_text(s); }

const std::string EingabeBox::get_value(int col) const
{  if (col<0 || col>=entries.size()) return "";
   return entries[col]->get_text(); 
}

void EingabeBox::set_value(int col,const std::string &s)
{  grow(col+1); entries[col]->set_text(s); 
   // EAN-COde darf nicht editiert werden:
   if(labels[col]->get_text()=="EAN-Code") entries[col]->set_editable(false);
}

void EingabeBox::set_width(int col,int width)
{  grow(col+1); // entries[col]->set_usize(width,-1);
}

void EingabeBox::set_size(int cols)
{  if (cols>visible_size)
   {  int cols_to_show(cols>=entries.size()?entries.size():cols);
      // restore
      if (visible_size<cols_to_show)
      {  cons[visible_size].disconnect();
         if (visible_size)
	      cons[visible_size]=entries[visible_size-1]->signal_activate()
	      	.connect(SigC::slot(*entries[visible_size],&Gtk::Widget::grab_focus));
         else // visible_size==0
            cons[0]=grab_focus_tried.connect(SigC::slot(*entries[0],&Gtk::Widget::grab_focus));
         // short cut ?
         if (cols_to_show<entries.size())
         {  cons[cols_to_show].disconnect();
            cons[cols_to_show]=entries[cols_to_show-1]->signal_activate()
            	.connect(SigC::slot(*this,&EingabeBox::activate));
         }		
         for (int c=visible_size;c<cols_to_show;c++)
         {  labels[c]->show(); entries[c]->show();
         }
         visible_size=cols_to_show;
      }
      if (cols>cols_to_show) grow(cols);
   }
   else if (cols<visible_size)
   {  // short cut
      cons[cols].disconnect();
      if (cols) 
         cons[cols]=entries[cols-1]->signal_activate().connect(SigC::slot(*this,&EingabeBox::activate));
      else
         cons[cols]=grab_focus_tried.connect(SigC::slot(*this,&EingabeBox::activate));
      for (int c=cols;c<visible_size;c++)
      {  labels[c]->hide(); entries[c]->hide();
      }
      visible_size=cols;
   }
   check();
}

/*
void EingabeBox::activate_entry(int nr)
{
   entries[visible_size]->grab_focus();
   entries[visible_size]->select_region(0,-1);
}
*/

void EingabeBox::reset()
{  for (t_entries::iterator i=entries.begin();i!=entries.end();++i)
      (*i)->set_text("");
}

