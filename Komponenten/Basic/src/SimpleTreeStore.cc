// $Id: SimpleTreeStore.cc,v 1.24 2002/12/11 15:40:43 christof Exp $
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

// property_is_expanded
// property_is_expander
// property_xalign
// 
// property_text
// property_background (text)
// property_background_gdk (text)
// property_editable

#include <SimpleTreeStore.h>
#ifdef MANUPROC_WITH_DATABASE
#include <Misc/Global_Settings.h>
#include <unistd.h> // getuid
#endif
#include <Misc/itos.h>
#include <Misc/Trace.h>
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
   guint last=invisible_column;
   sequence_t::const_reverse_iterator i=currseq.rbegin();
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
      for (guint j=0;j<MaxCol();++j,bit<<=1)
         vec_hide_cols[j]=!bit ? true : !(sichtbar&bit);
      k1=visible.find(',',k0+1);
   }
   if (k1!=std::string::npos)
   {  showdeep=strtoul(visible.substr(k0+1,k1-(k0+1)).c_str(),0,10);
   }

   sequence_t s;
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

static const unsigned col1=0xffff,col0=0xcfff;


SimpleTreeStore::SimpleTreeStore(int max_col)
	: node_creation(0), columns(max_col), max_column(max_col),
	  showdeep(0), gp(0), 
	  auffuellen_bool(false), expandieren_bool(false),
	  color_bool(false), m_columns(max_col)
{  m_refTreeStore=Gtk::TreeStore::create(m_columns);
   defaultSequence();
   getModel().signal_title_changed().connect(SigC::slot(*this,&SimpleTreeStore::on_title_changed));
   getModel().signal_redraw_needed().connect(SigC::slot(*this,&SimpleTreeStore::redisplay));
   getModel().signal_line_appended().connect(SigC::slot(*this,&SimpleTreeStore::on_line_appended));
   getModel().signal_line_to_remove().connect(SigC::slot(*this,&SimpleTreeStore::on_line_removed));
  vec_hide_cols.resize(Cols());
  for (std::vector<bool>::iterator i=vec_hide_cols.begin();i!=vec_hide_cols.end();++i)
    (*i) = true;
  Gdk::Color c;
  c.set_rgb(col1,col1,col1); colors.push_back(c); // white
  c.set_rgb(col1,col0,col0); colors.push_back(c); // red
  c.set_rgb(col1,col1,col0); colors.push_back(c); // yellow
  c.set_rgb(col0,col1,col0); colors.push_back(c); // green
  c.set_rgb(col0,col1,col1); colors.push_back(c); // cyan
  c.set_rgb(col0,col0,col1); colors.push_back(c); // blue
  c.set_rgb(col1,col0,col1); colors.push_back(c); // magenta
  c.set_rgb(col0,col0,col0); colors.push_back(c); // dark grey
  assert(colors.size()==num_colors);
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
   }
   add(row);
   add(deep);
   add(childrens_deep);
   add(value);
   add(leafdata);
   add(background);
}

void SimpleTreeStore::on_title_changed(guint idx)
{  unsigned col=ColumnFromIndex(idx);
   if (col!=invisible_column) title_changed(col);
}

const std::string SimpleTreeStore::getColTitle(guint idx) const
{  return getModel().getColTitle(currseq[idx]);
}

void SimpleTreeStore::defaultSequence()
{  currseq.clear();
   for(guint i=0; i<MaxCol(); ++i) currseq.push_back(i);
}

#warning: auffüllen Standard, aktuelle
void SimpleTreeStore::fillSequence(sequence_t &seq) const
{  for(guint i=0; i<MaxCol(); ++i) 
      if (ColumnVisible(i) && std::find(seq.begin(),seq.end(),i)==seq.end())
         seq.push_back(i);
}

void SimpleTreeStore::SummenAnzeigen(Gtk::TreeModel::Children parent)
{for(iterator i = parent.begin(); i!=parent.end(); ++i)
 {  Handle<TreeRow> htr=(*i)[m_columns.row];
    if (htr) SummenAnzeigen(*i,htr);

    Gtk::TreeModel::Children ch=i->children();
    if (ch.begin()!=ch.end()) SummenAnzeigen(ch);
 }
}

void SimpleTreeStore::redisplay()
{
 
// liste loeschen
 m_refTreeStore->clear();

 std::vector<cH_RowDataBase>::const_iterator i=getDataVec().begin();
 std::vector<cH_RowDataBase>::const_iterator j=getDataVec().end();

// neu einordnen, Summen berechnen
 for(; i!=j; ++i)
    insertLine(m_refTreeStore->children(),*i,currseq.begin(),0,false);

 SummenAnzeigen(m_refTreeStore->children());

#if 0
 show_or_hide_Spalten();
 expand();
 // callback breitstellen:
 reorder();
#endif
}

void SimpleTreeStore::on_line_appended(cH_RowDataBase row)
{  ManuProC::Trace(trace_channel,__FUNCTION__,row->Value(0,0)->getStrVal(),
		row->Value(1,0)->getStrVal());
   insertLine(m_refTreeStore->children(),row,currseq.begin(),0,true);
}

namespace {
class CompareValue
{	Gtk::TreeModelColumn<cH_EntryValue> value;
public:
	CompareValue(Gtk::TreeModelColumn<cH_EntryValue> &v)
		: value(v) {}
	bool operator()(const Gtk::TreeStore::iterator &a, const cH_EntryValue &b)
	{  return *static_cast<cH_EntryValue>((*a)[value]) < *b;
	}
	bool operator()(const cH_EntryValue &a, const Gtk::TreeStore::iterator &b)
	{  return *a < *static_cast<cH_EntryValue>((*b)[value]);
	}
};
}

//#define DEBUG_NEW

// these macros are a lot faster than x.size() because that needs a division
#define KeinKind(x) ((x).begin()==(x).end())
#define NurEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) != (x).end())

void SimpleTreeStore::SummenAnzeigen(const Gtk::TreeRow &row,const Handle<TreeRow> &htr)
{  assert(htr);
   assert(row[m_columns.childrens_deep]!=0);
   unsigned i=row[m_columns.childrens_deep];
   if (i<Cols()) 
      row[m_columns.cols[i]]= "... "+htr->Value(currseq[i],ValueData())->getStrVal();
   for (++i;i<Cols();++i)
      row[m_columns.cols[i]]= htr->Value(currseq[i],ValueData())->getStrVal();
}

void SimpleTreeStore::insertLine(Gtk::TreeModel::Children parent,
            const cH_RowDataBase &v,  sequence_t::const_iterator seqit,
            guint deep, bool summe_aktualisieren)
{if (seqit==currseq.end()) return;
recurse:
 Gtk::TreeStore::iterator current_iter=parent.begin();
 Gtk::TreeStore::iterator apiend = parent.end();
 Gtk::TreeStore::iterator upper_b=apiend;
 sequence_t::const_iterator seqlast=--currseq.end();
 guint seqnr=*seqit;
 cH_EntryValue ev=v->Value(seqnr,ValueData());

// node/leaf mit Wert<=ev suchen
// optimization: we expect to need upper_bound if this is the last attribute
 if (seqit==seqlast) // !MehrAlsEinKind(selseq))
 {  std::pair<Gtk::TreeStore::iterator,Gtk::TreeStore::iterator> range 
 		= std::equal_range(current_iter,apiend,ev,
 				CompareValue(m_columns.value));
    current_iter=range.first;	// lower_bound
    upper_b=range.second;	// upper_bound
 }
 else
    current_iter=std::lower_bound(current_iter,apiend,ev,
 				CompareValue(m_columns.value));

 if(current_iter!=apiend) // dann einfuegen
   {//----------------- gleicher Wert ------------------
    if ((ev) == (*current_iter)[m_columns.value])
     { 
      if (seqit!=seqlast)
      // MehrAlsEinKind(selseq)) // ???? wenn Blatt noch nicht erreicht ???
      				// SimpleTree2: Summen stehen mit drin ...
      // eine neue Node erzeugen(?)
      {  cH_RowDataBase v2=(*current_iter)[m_columns.leafdata];
         guint child_s_deep=deep;

	do 
	{seqit++;
	 ++child_s_deep;
	 
	 // darum muss sich eine andere Node kümmern
         if (child_s_deep==(*current_iter)[m_columns.childrens_deep])
         {weiter_unten_einhaengen:
            Handle<TreeRow> htr=(*current_iter)[m_columns.row];
            if (htr) 
            {  htr->cumulate(v);
               if (summe_aktualisieren) SummenAnzeigen(*current_iter,htr);
            }
            // goto ist schneller als (end?)rekursion !!!
            parent=current_iter->children();
            deep=child_s_deep;
            goto recurse;
            // insertLine(current_iter->children(),v,seqit,child_s_deep);
            // return;
         }
         
        } while (seqit!=seqlast // MehrAlsEinKind(selseq) 
			&& v->Value(*seqit,ValueData())
				==v2->Value(*seqit,ValueData()));
         
	 // mitten in current_iter einfügen 
	 // (current_iter wandert nach unten rechts)
         // (man könnte dies auch aufbrechen nennen)
         current_iter= MoveTree (current_iter,deep,child_s_deep,*seqit);
         goto weiter_unten_einhaengen;
      }
      else // Blatt erreicht
      {  // als letztes der Gleichen an parent anhängen
         // upper_b steht schon richtig (s.o.)
         Gtk::TreeRow newnode = *(m_refTreeStore->insert(upper_b));
         InitColumns(newnode,deep,ev,v);
      }
      return;
     }
     else // --------------- kleinerer Wert (davor Einfügen) ----------
	{  Gtk::TreeRow newnode = *(m_refTreeStore->insert(current_iter));
           InitColumns(newnode,deep,ev,v);
	}
   }
 else //----------------- am Ende der Liste: anhängen ---------------------
   {  Gtk::TreeRow newnode = *(m_refTreeStore->append(parent));
      InitColumns(newnode,deep,ev,v);
    }
}                                

// former initTCL, initDepth
void SimpleTreeStore::InitColumns(Gtk::TreeRow &node, guint deep,
	const cH_EntryValue &ev, const cH_RowDataBase &v)
{  // node[m_columns.row]= Handle<TreeRow>();
   node[m_columns.value]= ev;
   node[m_columns.leafdata]= v;
   node[m_columns.deep]=deep;
   node[m_columns.background]= colors[deep%num_colors];
   node[m_columns.childrens_deep]=0;
   for (guint i=deep;i<Cols();++i)
   {  node[m_columns.cols[i]]=v->Value(currseq[i],ValueData())->getStrVal();
   }
}

Gtk::TreeRow SimpleTreeStore::CopyTree(Gtk::TreeRow src, Gtk::TreeModel::Children dest)
{  Gtk::TreeRow newrow = *(m_refTreeStore->append(dest));

// isn't there an easier way to copy _all_ columns?   
   newrow[m_columns.row]= static_cast<Handle<TreeRow> >(src[m_columns.row]);
   newrow[m_columns.value]= static_cast<cH_EntryValue>(src[m_columns.value]);
   newrow[m_columns.leafdata]= static_cast<cH_RowDataBase>(src[m_columns.leafdata]);
   newrow[m_columns.deep]= static_cast<guint>(src[m_columns.deep]);
   newrow[m_columns.background]= static_cast<Gdk::Color>(src[m_columns.background]);
   newrow[m_columns.childrens_deep]= static_cast<guint>(src[m_columns.childrens_deep]);
   for (guint i=0;i<m_columns.cols.size();++i)
      newrow[m_columns.cols[i]]= static_cast<Glib::ustring>(src[m_columns.cols[i]]);
      
   for (Gtk::TreeStore::iterator i=src.children().begin();
   		i!=src.children().end();++i)
      CopyTree(*i,newrow.children());
   return newrow;
}

// deep    childs_deep   childrens_deep
// -+ newnode
//  |      +  oldnode2                        leafdata
//  |                    + old2 children
//  + (oldnode=current_iter)
//                       + old children

Gtk::TreeStore::iterator SimpleTreeStore::MoveTree(
	Gtk::TreeStore::iterator current_iter,
	guint deep,guint child_s_deep,guint value_index)
{  Gtk::TreeStore::iterator new_iter= m_refTreeStore->insert(current_iter);
   Gtk::TreeRow newnode = *new_iter;
   Gtk::TreeRow oldnode = *current_iter;
   Gtk::TreeRow oldnode2 = CopyTree(oldnode, newnode.children());

   // initialize the sum
   //    this const_casting is for consistency only, 
   //    a Handle<const TreeRow> argument is more logical
   if (node_creation) 
   {  Handle<TreeRow> htr= (*node_creation)(const_cast<const TreeRow*>
		(&*static_cast<Handle<TreeRow> >(oldnode[m_columns.row])));
      newnode[m_columns.row]= htr;
      // leaves have no row (so initial sum is always 0), 
      // so we need to cumulate their data
      if (!oldnode2[m_columns.childrens_deep] && htr) // leaf
         htr->cumulate(oldnode2[m_columns.leafdata]);
   }
   newnode[m_columns.leafdata]= static_cast<cH_RowDataBase>(oldnode[m_columns.leafdata]);
   newnode[m_columns.childrens_deep]= child_s_deep;
   newnode[m_columns.deep]= deep;
   newnode[m_columns.background]= colors[deep%num_colors];
   newnode[m_columns.value]= static_cast<cH_EntryValue>(oldnode[m_columns.value]);
   for (guint i=deep;i<child_s_deep;++i)
      newnode[m_columns.cols[i]]= static_cast<Glib::ustring>(oldnode[m_columns.cols[i]]);
   newnode[m_columns.cols[child_s_deep]]= "...";
   // aufklappen wenn child_s_deep < showdeep

   for (guint i=deep;i<child_s_deep;++i) 
      oldnode2[m_columns.cols[i]]= "";
   oldnode2[m_columns.deep]= child_s_deep;
   oldnode2[m_columns.background]= colors[child_s_deep%num_colors];
   oldnode2[m_columns.value]= 
   	static_cast<cH_RowDataBase>(oldnode2[m_columns.leafdata])
   		->Value(value_index,ValueData());

   m_refTreeStore->erase(current_iter);
   return new_iter;
}

void SimpleTreeStore::setSequence(const sequence_t &neu)
{  currseq=neu; // Spaltenzahl anpassen?
#warning Spaltenzahl geändert?
   for (unsigned i=0;i<Cols();++i) title_changed(i);
   redisplay();
}

unsigned SimpleTreeStore::ColumnFromIndex(unsigned idx) const
{  unsigned currsize=currseq.size();
   for (guint i=0; i<currsize; ++i)
   {  if (currseq[i]==idx) return i;
   }
   return invisible_column;
}

const UniqueValue::value_t SimpleTreeStore::trace_channel
      = ManuProC::Tracer::channels.get();

void SimpleTreeStore::on_line_removed(cH_RowDataBase r)
{  std::list<Gtk::TreeStore::iterator> l=find_row(r);
   if (l.begin()!=l.end())
   {  ManuProC::Trace(trace_channel,__FUNCTION__,"depth=",l.size());
      for (std::list<Gtk::TreeStore::iterator>::const_reverse_iterator i=l.rbegin();
      		i!=const_cast<const std::list<Gtk::TreeStore::iterator>&>(l).rend();
      		++i)
      {  Handle<TreeRow> htr=(**i)[m_columns.row];
         if (htr)
         {  htr->deduct(r);
            SummenAnzeigen(**i,htr);
         }
      }
      m_refTreeStore->erase(*l.begin());
      // TODO: we might be able to eliminate the nodes if they no longer
      //   have children
   }
}

// optimize indicates we could binary search by value (possible optimization)
// this is impossible, if a value has already changed ...

bool SimpleTreeStore::find_row(Gtk::TreeModel::Children parent, const cH_RowDataBase &r,bool optimize,std::list<Gtk::TreeStore::iterator> &result)
{  for (iterator i= parent.begin(); i!=parent.end(); ++i)
   {  Gtk::TreeModel::Children ch=i->children();
      if (ch.begin()==ch.end())
      {  cH_RowDataBase v2=(*i)[m_columns.leafdata];
         if (&*v2==&*r) 
         {  result.push_back(i);
            return true;
         }
      }
      else 
      {  if (find_row(ch,r,optimize,result))
         {  result.push_back(i);
            return true;
         }
      }
   }
   return false;
}

std::list<Gtk::TreeStore::iterator> SimpleTreeStore::find_row(const cH_RowDataBase &r,bool optimize)
{  std::list<Gtk::TreeStore::iterator> result;
   find_row(m_refTreeStore->children(),r,optimize,result);
   return result;
}
