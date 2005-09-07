/*  Gtk--addons: a collection of gtk-- addons
 *  Copyright (C) 2001-2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// $Id: Gtk2TeX.cc,v 1.11 2005/09/07 16:22:52 christof Exp $

#include "Gtk2TeX.h"
#include "gtkhacks.h"

static bool is_lines(const std::string &s)
{  if (s.begin()==s.end()) return false;
   for (std::string::const_iterator i=s.begin();i!=s.end();++i)
      if (*i!='-') return false;
   return true;
}

static Glib::ustring get_text(const Gtk::TreeModel::const_iterator &y,const Gtk::TreeViewColumn*col)
{  int column=get_ModelColumn(col,"text");
   if (!column==-1) return std::string();
   Glib::ustring content;
   y->get_value(column,content);
   return content;
}

static void TreeView2Table_sub(std::ostream &os,const Gtk::TreeView *cl,
	const Gtk2TeX::TableFlags &fl,const Gtk::TreeModel::const_iterator &y,
	bool is_last_line)
{  bool has_lines(false),has_text(false);
   int col(1);
   if (fl.preline_cb) (*fl.preline_cb)(os,y,fl.user_data);
   // transform '[-]*' into '\cline'
   int x2=0;
   
   // gtkmm2 seems to have problems with const pointers so we trick
   Glib::ListHandle<Gtk::TreeViewColumn*> cols=const_cast<Gtk::TreeView*>(cl)->get_columns();
   for (Glib::ListHandle<Gtk::TreeViewColumn*>::const_iterator x=cols.begin();x!=cols.end();++x,++x2)
   {  Glib::ustring content=get_text(y,*x);
      
      if (!content.empty())
      {  if (is_lines(content))
         {  has_lines=true;
            os << "\\cline{"<<col<<'-'<<col<<'}';
         }
         else if (!has_text && !content.empty()) has_text=true;
      }
      col++;
   }
   if (has_text) 
   {  x2=0;
      guint next2=x2;
      for (Glib::ListHandle<Gtk::TreeViewColumn*>::const_iterator x=cols.begin(),xend=cols.end(),next=x;x!=xend;x=next,x2=next2)
      {  guint multicol=1;
         
         for (++next,++next2;next!=xend;++next,++next2)
         {  if (fl.multicolumn && get_text(y,*next).empty()
            	&& (*next)->get_alignment()<0.9)
               ++multicol;
            else break;
         }
         Glib::ustring content=get_text(y,*x);
         if (multicol>1) 
         {  os << "\\multicolumn{"<< multicol<< "}{l}{";
         }
         if (fl.element_cb)
            os << (*fl.element_cb)(y,x2,Gtk2TeX::string2TeX(content),fl.user_data);
         else 
            os << Gtk2TeX::string2TeX(content);
         if (multicol>1) os << '}';
         if (next!=xend) os << '&';
      }
   }
   if (fl.postline_cb) (*fl.postline_cb)(os,y,fl.user_data);
   bool visible_children=const_cast<Gtk::TreeView*>(cl)
   		->row_expanded(cl->get_model()->get_path(y))
   	 && !y->children().empty();
   if ((has_text || !has_lines) 
       && !(is_last_line && !visible_children))
   	// Nur clines und letzte Zeile erfordern kein '\\'
      os << "\\\\\n";
      
   if (visible_children)
      for (Gtk::TreeModel::Children::const_iterator i=y->children().begin();
   		i!=y->children().end();++i)
         TreeView2Table_sub(os,cl,fl,i,is_last_line?(i+1==y->children().end()):false);
}

std::ostream &Gtk2TeX::TreeView2Table(std::ostream &os,const Gtk::TreeView *cl,const TableFlags &fl)
{  // gtkmm2 seems to have problems with const pointers so we trick
   Glib::ListHandle<Gtk::TreeViewColumn*> cols=const_cast<Gtk::TreeView*>(cl)->get_columns();
   int columns(cols.size());

   if (fl.environment)
   {  os << "\\begin{" << (fl.longtable?"longtable":"tabular") << "}"
   	"{";
      unsigned idx=0;
      for (Glib::ListHandle<Gtk::TreeViewColumn*>::const_iterator i=cols.begin();i!=cols.end();++i,++idx)
      {  std::string typ("l");
         if ((*i)->get_alignment()>0.66) typ="r";
         else if ((*i)->get_alignment()>0.33) typ="c";
         // p{} ??? GTK_JUSTIFY_FILL ???

         if (fl.columntype_cb) 
         {  const std::string title((*i)->get_title());
            typ=(*fl.columntype_cb)(idx,typ,title,fl.user_data);
         }
         os << typ;
      }
      os << "}%\n";
   }
   if (fl.prehead_cb) (*fl.prehead_cb)(os,fl.user_data);
   if (fl.headline)
   {  unsigned idx=0;
      for (Glib::ListHandle<Gtk::TreeViewColumn*>::const_iterator i=cols.begin();i!=cols.end();++i,++idx)
      {  if (!fl.columntitle_cb)
	    os << string2TeX((*i)->get_title());
	 else
	    os << (*fl.columntitle_cb)(idx,(*i)->get_title(),fl.user_data);
         if (idx+1<columns) os << '&';
      }
      if (fl.posthead_cb) (*fl.posthead_cb)(os,fl.user_data);
      os << "\\endhead\n";
   }

#if 0   
   else if ((fl.first_line!=0 || fl.last_line!=-1) 
   	&& cl->rows().begin()!=cl->rows().end())  // empty() is non const :-(
   {  guint last=fl.last_line;
      if (fl.last_line==-1 || last>cl->rows().size()-1) 
         last=cl->rows().size()-1;
      for (guint y=fl.first_line;y<=last;++y)
      	 CList2Table_sub(os,cl,fl,cl->row(y),y==last);
   }
#endif   

  if(fl.firstrow_cb)
    {  
    std::string first_row=(*fl.firstrow_cb)(fl.user_data,fl.deep,fl.sequence); 
      os << first_row;  
    } 


   if (!fl.selection.empty()) //  && fl.selection->begin()!=fl.selection->end())
   { for (Gtk::TreeSelection::ListHandle_Path::const_iterator i=fl.selection.begin();i!=fl.selection.end();++i)
     { Gtk::TreeSelection::ListHandle_Path::const_iterator j=i;
       ++j;
       TreeView2Table_sub(os,cl,fl,const_cast<Gtk::TreeView*>(cl)->get_model()->get_iter(*i),j==fl.selection.end());
     }
   }
   else
   {  Gtk::TreeModel::const_iterator yend(const_cast<Gtk::TreeView*>(cl)->get_model()->children().end());
      // no working const_iterator yet ...   
      for (Gtk::TreeModel::iterator y=const_cast<Gtk::TreeView*>(cl)->get_model()->children().begin();
			y!=yend;++y)
         TreeView2Table_sub(os,cl,fl,y,y+1==yend);
   }
   
   if (fl.postlist_cb) (*fl.postlist_cb)(os,fl.user_data);
   if (fl.environment)
      os << "\\end{" << (fl.longtable?"longtable":"tabular") << "}";
   return os;
}
