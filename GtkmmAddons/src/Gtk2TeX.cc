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

// $Id: Gtk2TeX.cc,v 1.4 2003/11/11 10:15:28 christof Exp $

#include "Gtk2TeX.h"

static bool is_lines(const std::string &s)
{  if (s.begin()==s.end()) return false;
   for (std::string::const_iterator i=s.begin();i!=s.end();++i)
      if (*i!='-') return false;
   return true;
}

#if 0
static void CList2Table_sub(std::ostream &os,const Gtk::CList *cl,const Gtk2TeX::TableFlags &fl,const Gtk::CList::Row &y,bool is_last_line)
{  bool has_lines(false),has_text(false);
   int col(1);
   if (fl.preline_cb) (*fl.preline_cb)(os,y,fl.user_data);
   // transform '[-]*' into '\cline'
   int x2=0;
   for (Gtk::CList::Row::iterator x=y.begin(),xend=y.end();x!=xend;++x,++x2)
   {  if (!cl->gtkobj()->column[x2].visible) continue;
      Gtk::nstring content;
      if (!(content=x->get_text()).null())
      {  if (is_lines(content))
         {  has_lines=true;
            os << "\\cline{"<<col<<'-'<<col<<'}';
         }
         else if (content.size()) has_text=true;
      }
      col++;
   }
   if (has_text) 
   {  x2=0;
      guint next2=x2;
      for (Gtk::CList::Row::iterator x=y.begin(),xend=y.end(),next=x;x!=xend;x=next,x2=next2)
      {  if (!cl->gtkobj()->column[x2].visible) continue;
         guint multicol=1;
         
         for (++next,++next2;next!=xend;++next,++next2)
         {  if (!cl->gtkobj()->column[next2].visible) continue;
            if (fl.multicolumn && 
            	(next->get_text().null() || next->get_text().empty())
            	// we could move this out of the for loop
            	&& cl->gtkobj()->column[x2].justification!=GTK_JUSTIFY_RIGHT)
               ++multicol;
            else break;
         }
         Gtk::nstring content;
         if (multicol>1) 
         {  os << "\\multicolumn{"<< multicol<< "}{l}{";
         }
         if (!(content=x->get_text()).null()) 
         {  if (fl.element_cb)
               os << (*fl.element_cb)(y,x2,Gtk2TeX::string2TeX(content),fl.user_data);
            else 
               os << Gtk2TeX::string2TeX(content);
         }
         if (multicol>1) os << '}';
         if (next!=xend) os << '&';
      }
   }
   if (fl.postline_cb) (*fl.postline_cb)(os,y,fl.user_data);
   if ((has_text || !has_lines) && !is_last_line)
   	// Nur clines und letzte Zeile erfordern kein '\\'
      os << "\\\\\n";
}

std::ostream &Gtk2TeX::CList2Table(std::ostream &os,const Gtk::CList *cl,const TableFlags &fl)
{  int columns(cl->gtkobj()->columns);

   if (fl.environment)
   {  os << "\\begin{" << (fl.longtable?"longtable":"tabular") << "}"
   	"{";
      for (int i=0;i<columns;++i)
      {  if (!cl->gtkobj()->column[i].visible) continue;
         std::string typ("l");
         switch(cl->gtkobj()->column[i].justification)
         {  case GTK_JUSTIFY_LEFT: typ="l"; break;
            case GTK_JUSTIFY_RIGHT: typ="r"; break;
            case GTK_JUSTIFY_CENTER: typ="c"; break;
            case GTK_JUSTIFY_FILL: typ="l"; break; // p{}
         }
         if (fl.columntype_cb) 
         {  const std::string title(cl->gtkobj()->column[i].title?cl->gtkobj()->column[i].title:"");
            typ=(*fl.columntype_cb)(i,typ,title,fl.user_data);
         }
         os << typ;
      }
      os << "}%\n";
   }
   if (fl.prehead_cb) (*fl.prehead_cb)(os,fl.user_data);
   if (fl.headline)
   { for (int i=0;i<columns;++i)
      {  if (!cl->gtkobj()->column[i].visible) continue;

	 if(cl->gtkobj()->column[i].title)
	 {  if (!fl.columntitle_cb)
	        os << string2TeX(cl->gtkobj()->column[i].title);
	    else
		os << (*fl.columntitle_cb)(i,cl->gtkobj()->column[i].title,fl.user_data);
         }
         if (i+1<columns) os << '&';
      }
      if (fl.posthead_cb) (*fl.posthead_cb)(os,fl.user_data);
      os << "\\endhead\n";
   }
   
   if (fl.selection && fl.selection->begin()!=fl.selection->end())
   {  Gtk::CList::SelectionList::iterator yend(fl.selection->end()),last(yend);
      --last;
      for (Gtk::CList::SelectionList::iterator 
      				y=fl.selection->begin();y!=yend;++y)
      	 CList2Table_sub(os,cl,fl,*y,y==last);
   }
   else if ((fl.first_line!=0 || fl.last_line!=-1) 
   	&& cl->rows().begin()!=cl->rows().end())  // empty() is non const :-(
   {  guint last=fl.last_line;
      if (fl.last_line==-1 || last>cl->rows().size()-1) 
         last=cl->rows().size()-1;
      for (guint y=fl.first_line;y<=last;++y)
      	 CList2Table_sub(os,cl,fl,cl->row(y),y==last);
   }
   else
   {  Gtk::CList::RowList::iterator yend(cl->rows().end()),last(yend);
      --last;
      for (Gtk::CList::RowList::iterator y=cl->rows().begin();y!=yend;++y)
         CList2Table_sub(os,cl,fl,*y,y==last);
   }
   if (fl.postlist_cb) (*fl.postlist_cb)(os,fl.user_data);
   if (fl.environment)
      os << "\\end{" << (fl.longtable?"longtable":"tabular") << "}";
   return os;
}

#endif
