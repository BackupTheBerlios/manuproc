// $Id: SimpleTreeStore.cc,v 1.7 2002/11/27 23:38:00 christof Exp $
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
   getModel().signal_title_changed().connect(SigC::slot(*this,&SimpleTreeStore::on_title_changed));
   defaultSequence();
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
   add(row);
   add(node_val);
}

// CellItem ^= TreeRow

void SimpleTreeStore::on_title_changed(guint idx)
{  for (guint i=0; i<Cols(); ++i)
   {  if (currseq[i]==idx)
      {  title_changed(i);
         return; // this assumes that there are no duplicates
      }
   }
}

const std::string SimpleTreeStore::getColTitle(guint idx) const
{  return getModel().getColTitle(currseq[idx]);
}

void SimpleTreeStore::defaultSequence()
{  currseq.clear();
   for(guint i=0; i<Cols(); ++i) currseq.push_back(i);
}

#if 0
void SimpleTreeStore::redisplay()
{
 
// liste loeschen
 if(clear_me) TCList::clear();

 std::vector<cH_RowDataBase>::const_iterator i=datavec.begin();
 std::vector<cH_RowDataBase>::const_iterator j=datavec.end();

// neu einordnen, Summen berechnen
 for(; i!=j; ++i)
    insertLine((TCListRow_API*)this,*this,*i,currseq,0);

#if 0
// Summen anzeigen
 for(TCListRow_API::iterator i = begin(); i!=end(); ++i)
 {  if (!((TreeRow*)(*i).get_user_data())->Leaf())
   	((TreeRow*)(*i).get_user_data())->refreshSum(*this);
 }
 show_or_hide_Spalten();
 expand();

//CList Breite anpassen
 for (unsigned int i=0;i<Cols();++i)
        set_column_auto_resize(i,true);

 // callback breitstellen:
 reorder();
#endif
}
#endif

void SimpleTreeStore::on_line_appended(cH_RowDataBase row)
{  insertLine(get_iter("0"),row,currseq,0);
// Summen neu anzeigen (hmmm, overkill!)
#if 0
 for(TCListRow_API::iterator i = begin(); i!=end(); ++i)
 {  if (!((TreeRow*)(*i).get_user_data())->Leaf())
   	((TreeRow*)(*i).get_user_data())->refreshSum(*this);
 }
#endif
}

namespace {
class CompareValue
{	Gtk::TreeModelColumn<cH_EntryValue> node_val;
public:
	CompareValue(Gtk::TreeModelColumn<cH_EntryValue> &v)
		: node_val(v) {}
	bool operator()(const TreeStore::iterator &a, const cH_EntryValue &b)
	{  return *((*a)[node_val]) < *b;
	}
	bool operator()(const cH_EntryValue &a, const TreeStore::iterator &b)
	{  return *a < *((*b)[node_val]);
	}
};
}

//#define DEBUG_NEW

// these macros are a lot faster than x.size() because that needs a division
#define KeinKind(x) ((x).begin()==(x).end())
#define NurEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) != (x).end())

void SimpleTreeStore::insertLine(TreeStore::iterator &parent,
            const cH_RowDataBase &v, std::deque<guint> selseq, guint deep)
{
recurse:
 TreeStore::iterator current_iter=parent.begin();
 TreeStore::iterator apiend = parent.end();
 TreeStore::iterator upper_b=apiend;
 guint seqnr=selseq.front();	
 cH_EntryValue ev=v->Value(seqnr,ValueData());

// node/leaf mit Wert<=ev suchen
// optimization: we expect to need upper_bound if this is the last attribute
 if (!MehrAlsEinKind(selseq))
 {  std::pair<TreeStore::iterator,TreeStore::iterator> range 
 		= std::equal_range(current_iter,apiend,ev,
 				CompareValue(m_columns.node_val));
    current_iter=range.first;	// lower_bound
    upper_b=range.second;	// upper_bound
 }
 else
    current_iter=std::lower_bound(current_iter,apiend,ev,
 				CompareValue(m_columns.node_val));

 if(current_iter!=apiend) // dann einfuegen
   {// eigentlich nur ein gecastetes current_iter
    TreeRow *current_tclr=reinterpret_cast<TreeRow*>((*current_iter).get_user_data());
    //----------------- gleicher Wert ------------------
    if((ev) == *((*current_iter)[m_columns.node_val]))
     { 
      if (MehrAlsEinKind(selseq)) // wenn Blatt noch nicht erreicht
      // eine neue Node erzeugen(?)
      {  cH_RowDataBase v2=(*current_iter)[m_columns.row];
         guint child_s_deep=deep;

	do 
	{selseq.pop_front();
	 ++child_s_deep;
	 
	 // darum muss sich eine andere Node kümmern
         if (child_s_deep==current_tclr->Children_s_Deep())
         {  current_tclr->cumulate(v);
            // insertIntoTCL((&*current_iter),tree,v,selseq,child_s_deep);
            // return;
            // goto ist schneller als (end?)rekursion !!!
            parent=&*current_iter;
            deep=child_s_deep;
            goto recurse;
         }
         
        } while (MehrAlsEinKind(selseq) 
			&& v->Value(selseq.front(),ValueData())
				==v2->Value(selseq.front(),ValueData()));
         
	 // vor current_iter einfügen
         TreeRow *newnode=NewNode(deep, ev, child_s_deep, v2, child_s_deep < showdeep, *current_tclr);
	 newnode->initTCL(parent,current_iter,tree);
	 tree.initDepth(newnode,deep);
	 
	 current_tclr->getTCL_API()->reparent(*parent,*newnode->getTCL_API());
	 current_tclr->ValueDeep(v2->Value(selseq.front(),ValueData()),child_s_deep);
	 tree.initDepth(current_tclr,child_s_deep);

	 // das neue Blatt einsortieren
	 newnode->cumulate(v);
         // insertIntoTCL(newnode->getTCL_API(),tree,v,selseq,child_s_deep);
         parent=newnode->getTCL_API();
         deep=child_s_deep;
         goto recurse;
      }
      else // Blatt erreicht
      {  // als letztes der Gleichen an parent anhängen
         // upper_b steht schon richtig (s.o.)
         TreeRow *newleaf=NewLeaf(deep,ev,v);
	 newleaf->initTCL(parent, upper_b, tree);
      }
      return;
     }
     else // --------------- kleinerer Wert (davor Einfügen) ----------
	{  TreeRow *newleaf=NewLeaf(deep,ev,v);
	 newleaf->initTCL(parent,current_iter,tree);
	 tree.initDepth(newleaf,deep);
	}
   }
 else //----------------- am Ende der Liste: anhängen ---------------------
   {   TreeRow *newleaf=NewLeaf(deep,ev,v);
	    newleaf->initTCL(parent,tree); 
	 tree.initDepth(newleaf,deep);
    }
}                                
