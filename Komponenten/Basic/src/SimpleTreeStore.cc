// $Id: SimpleTreeStore.cc,v 1.3 2002/11/22 11:08:00 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <SimpleTreeStore.h>
#ifdef MANUPROC_WITH_DATABASE
#include <Misc/Global_Settings.h>
#include <unistd.h> // getuid
#endif
#include <Misc/itos.h>
//#include <GType_cH_EntryValue.h>

void SimpleTreeModel_Proxy::setModel(SimpleTreeModel &_model)
{  if (model_is_ours) { delete model; model_is_ours=false; }
   model=&_model;
}

SimpleTreeModel_Proxy::SimpleTreeModel_Proxy()
	: model(new SimpleTreeModel), model_is_ours(true)
{}

SimpleTreeModel_Proxy::~SimpleTreeModel_Proxy()
{  if (model_is_ours) { delete model; model_is_ours=false; }
}

// =========================================================

void SimpleTreeStore::save_remembered() const
{  if (mem_prog.empty()) return;
#ifdef MANUPROC_WITH_DATABASE
   unsigned int sichtbar=0,bit=1;
   for (std::vector<bool>::const_iterator i=vec_hide_cols.begin();
   		bit && i!=vec_hide_cols.end();++i,bit<<=1)
      if (!*i) sichtbar|=bit;
   std::string flags;
//   if (!titles_bool) flags+='T';
   if (auffuellen_bool) flags+='a';
   if (!expandieren_bool) flags+='E';
   if (!color_bool) flags+='C';
   Global_Settings::create(getuid(),mem_prog,mem_inst+":visible",
   	itos(sichtbar)+','+itos(showdeep)+','+flags);
   std::string cseq;
   guint last=999999; // big enough
   std::deque<guint>::const_reverse_iterator i=currseq.rbegin();
   for (;i!=currseq.rend();++i)
   {  if (*i>last) break;
      last=*i;
   }
   for (;i!=currseq.rend();++i) cseq=itos(*i)+','+cseq;
   Global_Settings::create(getuid(),mem_prog,mem_inst+":order",cseq);
#endif
}

void SimpleTreeStore::load_remembered()
{  if (mem_prog.empty()) return;
#ifdef MANUPROC_WITH_DATABASE
   std::string visible=Global_Settings(getuid(),mem_prog,mem_inst+":visible").get_Wert();
//   titles_bool=visible.find('T')==std::string::npos;
   expandieren_bool=visible.find('E')==std::string::npos;
   color_bool=visible.find('C')==std::string::npos;
   
   std::string::size_type k0=visible.find(','),k1=std::string::npos;
   if (k0!=std::string::npos) 
   {  guint sichtbar=strtoul(visible.substr(0,k0).c_str(),0,10),bit=1;
      for (guint j=0;j<attrcount;++j,bit<<=1)
         vec_hide_cols[j]=!bit ? true : !(sichtbar&bit);
      k1=visible.find(',',k0+1);
   }
   if (k1!=std::string::npos)
   {  showdeep=strtoul(visible.substr(k0+1,k1-(k0+1)).c_str(),0,10);
   }

   std::deque<guint> s;
   auffuellen_bool=false;
   std::string order=Global_Settings(getuid(),mem_prog,mem_inst+":order").get_Wert();
   for (std::string::size_type b=0;;)
   {  std::string::size_type e=order.find(',',b);
      if (e==std::string::npos) break;
      s.push_back(strtoul(order.substr(b,e-b).c_str(),0,10));
      b=e+1;
   }
#if 0
   clicked_seq=s;
   reihenfolge_anzeigen();
   
   auffuellen_bool=visible.find('a')!=std::string::npos;
   delete menu;
   menu=0;
   fillMenu();
#endif   
#endif
}

void SimpleTreeStore::set_remember(const std::string &program, const std::string &instance)
{  if (mem_prog!=program || mem_inst!=instance)
   {  mem_prog=program;
      mem_inst=instance;
      load_remembered();
   }
}

SimpleTreeStore::SimpleTreeStore(int cols,int attrs)
	: node_creation(0), columns(cols),
	  showdeep(0), attrcount(attrs), gp(0), 
	  auffuellen_bool(false), expandieren_bool(false),
	  color_bool(false), m_columns(cols)
{  if (attrs<1) attrcount=cols;
   m_refTreeStore=Gtk::TreeStore::create(m_columns);
}

class TreeModelColumn_C : public Gtk::TreeModelColumnBase
{public:
	TreeModelColumn_C(GType t) : Gtk::TreeModelColumnBase(t)
	{}
};

// eigentlich müssen versteckte Spalten gar nicht hinzugenommen werden
// aber wie dann das Model neu aufbauen?
SimpleTreeStore::ModelColumns::ModelColumns(int _cols)
{  // GType t=cH_entry_value_get_type();
   for (int i=0; i<_cols; ++i)
   {  cols.push_back(Gtk::TreeModelColumn<Glib::ustring>());
      add(cols.back());
//      assert(c.index()==i);
   }
}

// CellItem ^= TreeRow
