// $Id: SimpleTreeStore.cc,v 1.90 2005/11/02 12:38:22 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

// property_editable

#include <SimpleTreeStore.h>
#include <Misc/Global_Settings.h>
#include <unistd.h> // getuid
#include <Misc/itos.h>
#include <Misc/TraceNV.h>
//#include <GType_cH_EntryValue.h>
#include <gtkmm/treepath.h>
#include <Misc/EntryValueSort.h>
//#include <Misc/EntryValueInvert.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

// we need use the old Glib::Object code
#if GTKMM_MAJOR_VERSION==2 && (GTKMM_MINOR_VERSION<2 \
	|| (GTKMM_MINOR_VERSION==2 && GTKMM_MICRO_VERSION<10))
#  define OLD_MODEL 
#endif

#ifdef __MINGW32__
#define getuid() 0
#endif

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

std::pair<std::string,std::string> SimpleTreeStore::default_load(const std::string&program, const std::string&instance)
{  std::pair<std::string,std::string> result;
   if (!program.empty())
   {  result.second=Global_Settings(getuid(),program,instance+":visible").get_Wert();
      result.first=Global_Settings(getuid(),program,instance+":order").get_Wert();
   }
   return result;
}

void SimpleTreeStore::default_save(const std::string&program, const std::string&instance, const std::pair<std::string,std::string>&value)
{
   if (!program.empty())
   {  Global_Settings::create(getuid(),program,instance+":visible",value.second);
      Global_Settings::create(getuid(),program,instance+":order",value.first);
   }
}

void SimpleTreeStore::save_remembered() const
{  std::pair<std::string,std::string> value;

   if (block_save || (mem_prog.empty() && mem_inst.empty())) return;
   unsigned int sichtbar=0,bit=1;
   for (std::vector<bool>::const_iterator i=vec_hide_cols.begin();
   		bit && i!=vec_hide_cols.end();++i,bit<<=1)
      if (!*i) sichtbar|=bit;
   std::string flags;
//   if (!titles_bool) flags+='T';
   if (auffuellen_bool) flags+='a';
   if (!expandieren_bool) flags+='E';
   if (!color_bool) flags+='C';
   value.second=itos(sichtbar)+','+itos(showdeep)+','+flags;

   guint last=invisible_column;
   sequence_t::const_reverse_iterator i=currseq.rbegin();
   for (;i!=currseq.rend();++i)
   {  if (*i>last) break;
      last=*i;
   }
   for (;i!=currseq.rend();++i) value.first=itos(*i)+','+value.first;
   
   // additional flags from derived widgets?

   default_save(mem_prog, mem_inst, value);
}

void SimpleTreeStore::load_remembered()
{  if (mem_prog.empty() && mem_inst.empty()) return;
   std::pair<std::string,std::string> value=default_load(mem_prog, mem_inst);

//   titles_bool=value.second.find('T')==std::string::npos;
   expandieren_bool=value.second.find('E')==std::string::npos;
   color_bool=value.second.find('C')==std::string::npos;
   
   std::string::size_type k0=value.second.find(','),k1=std::string::npos;
   if (k0!=std::string::npos) 
   {  guint sichtbar=strtoul(value.second.substr(0,k0).c_str(),0,10),bit=1;
      for (guint j=0;j<MaxCol();++j,bit<<=1)
         vec_hide_cols[j]=!bit ? true : !(sichtbar&bit);
      k1=value.second.find(',',k0+1);
   }
   if (k1!=std::string::npos)
   {  showdeep=strtoul(value.second.substr(k0+1,k1-(k0+1)).c_str(),0,10);
   }

   sequence_t s;
   auffuellen_bool=false;
   for (std::string::size_type b=0;;)
   {  std::string::size_type e=value.first.find(',',b);
      if (e==std::string::npos) break;
      s.push_back(strtoul(value.first.substr(b,e-b).c_str(),0,10));
      b=e+1;
   }
   auffuellen_bool=value.second.find('a')!=std::string::npos;
   // Men und Anzeige anpassen
   block_save=true;
   on_visibly_changed(bvector_iterator());
   signal_save(0);
   block_save=false;
}

void SimpleTreeStore::set_remember(const std::string &program, const std::string &instance)
{  if (mem_prog!=program || mem_inst!=instance)
   {  mem_prog=program;
      mem_inst=instance;
      load_remembered();
   }
}

static const unsigned col1=0xffff,col0=0xcfff;

#ifdef OLD_MODEL
namespace {
class MyTreeModel_Class : public Glib::Class
{public:
	const Glib::Class& init();
	static void class_init_function(void* g_class, void* class_data);
};
}

static MyTreeModel_Class myclass;
#endif

void SimpleTreeStore::on_visibly_changed(bvector_iterator it)
{ if (it!=bvector_iterator())
  {if (!*it) // Spalte versteckt
   {  for (sequence_t::iterator i=currseq.begin();i!=currseq.end();)
      {  if (!ColumnVisible(*i))
         {  i=currseq.erase(i);
         }
         else ++i;
      }
   }
   else // Spalte hinzugekommen
   {
#if 0 // old behaviour
      fillSequence();
#else
    for(guint i=0; i<MaxCol(); ++i) 
      if (ColumnVisible(i) && std::find(currseq.begin(),currseq.end(),i)==currseq.end())
      {  for (sequence_t::iterator j=currseq.begin();j!=currseq.end();++j)
         {  if (*j>i)
            {  currseq.insert(j,i);
               break;
            }
         }
         if (std::find(currseq.begin(),currseq.end(),i)==currseq.end()) //immer noch nicht da
            currseq.push_back(i);
      }
#endif
   }
  }
   setSequence(currseq);
}

SimpleTreeStore::SimpleTreeStore(int max_col)
	: 
#ifdef OLD_MODEL	
	  Glib::ObjectBase("SimpleTree_MyTreeModel"),
	  Glib::Object(Glib::ConstructParams(myclass.init(), (char*) 0)),
#else
	  Glib::ObjectBase( typeid(SimpleTreeStore) ), //register a custom GType.
	  Glib::Object(), //The custom GType is actually registered here.
#endif	  
          columns_are_equivalent(true),
	  node_creation(), columns(max_col), max_column(max_col),
	  showdeep(), gp(), 
	  auffuellen_bool(), expandieren_bool(), block_save(),
	  color_bool(),
	  sortierspalte(invisible_column), invert_sortierspalte(), 
	  stamp(reinterpret_cast<long>(this)),
	  m_columns(max_col)
{  
#ifndef OLD_MODEL
  //We need to specify a particular get_type() from one of the virtual base classes, but they should
  //both return the same piece of data.
  {static bool inited=false;
   if (!inited)
   { Gtk::TreeModel::add_interface( Glib::Object::get_type() );
     inited=true;
   }
  }
#endif
  vec_hide_cols.resize(Cols());
  for (std::vector<bool>::iterator i=vec_hide_cols.begin();i!=vec_hide_cols.end();++i)
    (*i) = true;
   defaultSequence();
   getModel().signal_title_changed().connect(SigC::slot(*this,&SimpleTreeStore::on_title_changed));
//   getModel().signal_redraw_needed().connect(SigC::slot(*this,&SimpleTreeStore::redisplay));
   getModel().signal_please_detach().connect(please_detach.slot());
   getModel().signal_please_attach().connect(SigC::slot(*this,&SimpleTreeStore::redisplay));
   getModel().signal_line_appended().connect(SigC::slot(*this,&SimpleTreeStore::on_line_appended));
   getModel().signal_line_to_remove().connect(SigC::slot(*this,&SimpleTreeStore::on_line_removed));
   getModel().signal_value_changed().connect(SigC::slot(*this,&SimpleTreeStore::value_change_impl));
   signal_save.connect(SigC::slot(*this,&SimpleTreeStore::save_remembered1));
   signal_visibly_changed.connect(SigC::slot(*this,&SimpleTreeStore::on_visibly_changed));
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

// eigentlich müssen versteckte Spalten gar nicht hinzugenommen werden
// aber wie dann das Model neu aufbauen?

#define MC_ADD(name) add(name); assert(name.index()==int(s_##name));

SimpleTreeStore::ModelColumns::ModelColumns(int _cols)
{  // GType t=cH_entry_value_get_type();
   
   MC_ADD(row);
   MC_ADD(deep);
   MC_ADD(childrens_deep);
//   MC_ADD(value);
   MC_ADD(leafdata);
   MC_ADD(background);
   for (int i=0; i<_cols; ++i)
   {  cols.push_back(Gtk::TreeModelColumn<Glib::ustring>());
      add(cols.back());
      assert(cols.back().index()==int(s_text_start)+i);
   }
}

void SimpleTreeStore::on_title_changed(guint idx)
{  unsigned col=ColumnFromIndex(idx);
   ManuProC::Trace(trace_channel,__FUNCTION__,idx,col);
   if (col!=invisible_column) title_changed(col);
}

const std::string SimpleTreeStore::getColTitle(guint idx) const
{  return getModel().getColTitle(currseq[idx]);
}

void SimpleTreeStore::defaultSequence()
{  currseq.clear();
   for(guint i=0; i<MaxCol(); ++i) 
      if (ColumnVisible(i)) currseq.push_back(i);
}

void SimpleTreeStore::fillSequence(sequence_t &seq,bool standard) const
{  if (auffuellen_bool || standard)
   {for(guint i=0; i<MaxCol(); ++i) 
      if (ColumnVisible(i) && std::find(seq.begin(),seq.end(),i)==seq.end())
         seq.push_back(i);
   }
   else
   {  for (sequence_t::const_iterator i=currseq.begin();i!=currseq.end();++i)
         if (ColumnVisible(*i) && std::find(seq.begin(),seq.end(),*i)==seq.end())
            seq.push_back(*i);
   }
}

void SimpleTreeStore::resort(Node &parent, unsigned stop_at)
{  ManuProC::Trace(trace_channel,__FUNCTION__,&parent,stop_at);
   SimpleTreeStoreNode::map_t swapmap;
   std::swap(swapmap,parent.children);
   for (SimpleTreeStoreNode::map_t::iterator i=swapmap.begin();i!=swapmap.end();++i)
   {  cH_EntryValue x=i->first;
      cH_EntryValue y,z;
      if (i->second.row) y=i->second.row->Value(sortierspalte,ValueData());
      else y=i->second.leafdata->Value(sortierspalte,ValueData());
      if (invert_sortierspalte) z=cH_EntryValueReverseSort(y,x);
      else z=cH_EntryValueSort(y,x);
      Node &nd=parent.children.insert(parent.children.upper_bound(z),std::make_pair(z,Node()))->second;
      std::swap(i->second,nd);
      nd.parent=&parent;
      if (nd.childrens_deep<stop_at) resort(nd,stop_at);
   }
}

void SimpleTreeStore::redisplay()
{
 please_detach();
 ++stamp;
// liste loeschen
 root.children.clear();

 std::vector<cH_RowDataBase>::const_iterator i=getDataVec().begin();
 std::vector<cH_RowDataBase>::const_iterator j=getDataVec().end();

// neu einordnen, Summen berechnen
 for(; i!=j; ++i)
    insertLine(root,*i,currseq.begin(),0,false);
 if (sortierspalte!=invisible_column)
 {  sequence_t::const_iterator si=std::find(currseq.begin(),currseq.end(),sortierspalte); 
    resort(root,si-currseq.begin());
    root.fix_pointer();
//    test();
 }
// needs_redisplay();
 please_attach();
}

void SimpleTreeStore::on_line_appended(cH_RowDataBase row)
{  ManuProC::Trace(trace_channel,__FUNCTION__,row->Value(0,ValueData())->getStrVal(),
		row->Value(1,ValueData())->getStrVal());
   ++stamp;
   assert(sortierspalte==invisible_column);
   insertLine(root,row,currseq.begin(),0,true);
}

// these macros are a lot faster than x.size() because that needs a division
#define KeinKind(x) ((x).begin()==(x).end())
#define NurEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) != (x).end())

void SimpleTreeStore::insertLine(Node &parent,
            const cH_RowDataBase &v,  sequence_t::const_iterator seqit,
            guint deep, bool live)
{if (seqit==currseq.end()) return;

 iterator current_iter=parent.children.begin();
 iterator apiend = parent.children.end();
 iterator upper_b=apiend;
 sequence_t::const_iterator seqlast=--currseq.end();
 guint seqnr=*seqit;
 cH_EntryValue ev=v->Value(seqnr,ValueData());
 
// node/leaf mit Wert<=ev suchen
// optimization: we expect to need upper_bound if this is the last attribute
 if (seqit==seqlast) // !MehrAlsEinKind(selseq))
 {  std::pair<iterator,iterator> range 
 		= parent.children.equal_range(ev);
    current_iter=range.first;	// lower_bound
    upper_b=range.second;	// upper_bound
 }
 else
    current_iter=parent.children.lower_bound(ev);
 				
 if(current_iter!=apiend) // dann einfuegen
   {//----------------- gleicher Wert ------------------
    if ((ev) == current_iter->first)
     { 
      if (seqit!=seqlast)
      // MehrAlsEinKind(selseq)) // ???? wenn Blatt noch nicht erreicht ???
      				// SimpleTree2: Summen stehen mit drin ...
      // eine neue Node erzeugen(?)
      {  cH_RowDataBase v2=current_iter->second.leafdata;
         guint child_s_deep=deep;

	do 
	{seqit++;
	 ++child_s_deep;
	 
	 // darum muss sich eine andere Node kümmern
         if (child_s_deep==current_iter->second.childrens_deep)
         {weiter_unten_einhaengen:
            Handle<TreeRow> htr=current_iter->second.row;
            if (htr) 
            {  htr->cumulate(v);
               if (live) row_changed(getPath(current_iter),getIter(current_iter));
            }
            // goto ist schneller als (end?)rekursion ?
            insertLine(current_iter->second,v,seqit,child_s_deep,live);
            return;
         }
         
        } while (seqit!=seqlast // MehrAlsEinKind(selseq) 
			&& v->Value(*seqit,ValueData())
				==v2->Value(*seqit,ValueData()));
         
	 // mitten in current_iter einfügen 
	 // (current_iter wandert nach unten rechts)
         // (man könnte dies auch aufbrechen nennen)
 	 current_iter= MoveTree (current_iter,deep,child_s_deep,*seqit,live);
         goto weiter_unten_einhaengen;
      }
      else // Blatt erreicht
      {  // als letztes der Gleichen an parent anhängen
         // upper_b steht schon richtig (s.o.)
         current_iter=upper_b;
      }
     }
   }
   // --------------- kleinerer Wert (davor Einfügen) ----------   
   //----------------- oder am Ende der Liste: anhängen ---------------------
   iterator newnode=parent.children.insert(current_iter,std::make_pair(ev,Node(deep,&parent,v)));
   if (live) row_inserted(getPath(newnode),getIter(newnode));
}                                

// deep    childs_deep   childrens_deep
//  (vorher)
// -+ (oldnode=current_iter)=A
//                       + old children=B
//  (hinterher)
// -+ (oldnode=current_iter)
//         +  newchild=A                      leafdata
//                       + old children=B

SimpleTreeStore::iterator SimpleTreeStore::MoveTree(iterator current_iter,
	guint deep,guint child_s_deep,guint value_index,bool live)
{//  ManuProC::Trace(trace_channel,__FUNCTION__,deep,child_s_deep,value_index);
   Node &oldnode=current_iter->second;
   Node newnode(deep,oldnode.parent,oldnode.leafdata,child_s_deep);

   if (node_creation) 
   {  Handle<TreeRow> htr= (*node_creation)(&*oldnode.row);
      newnode.row=htr;
      // leaves have no row (so initial sum is always 0), 
      // so we need to cumulate their data
      if (!oldnode.childrens_deep && htr) // leaf
         htr->cumulate(oldnode.leafdata);
   }

   // ein Kind an newnode anfügen (newchild)
   cH_EntryValue val=oldnode.leafdata->Value(value_index,ValueData());
   newnode.children.insert(std::make_pair(val,Node(deep,oldnode.parent)));
   // dieser Knoten wird als oldnode im Baum hängen
   Node &newchild=newnode.children.begin()->second;

   // newchild und oldnode vertauschen (Unterbaum ist dann außerhalb des Baumes
   //		unterhalb von newnode, newchild ist dann im Baum)
   // oldnode ist jetzt die unnötige Node, newchild die alte Node
   std::swap(newchild,oldnode);

   // newnode und oldnode vertauschen (newnode+Kinder sind dann wieder im Baum)
   //      nun muss newnode wieder leer sein und den Inhalt von newchild enthalten
   // oldnode ist jetzt die neue Eltern-Node, newnode ist jetzt die unnötige
   std::swap(newnode,oldnode);
   newchild.parent=&oldnode;
   newchild.deep=child_s_deep;
   // oldnode's address has changed to newchild ...
   for (iterator i=newchild.children.begin();i!=newchild.children.end();++i)
   {  i->second.parent=&newchild;
   }
   // row_has_child_toggled(newnode), row_changed(newnode)
   
//   ManuProC::Trace(trace_channel,"",&oldnode,&newnode,&newchild);
//   oldnode.fix_pointer();
   if (live) 
   { row_has_child_toggled(getPath(current_iter),getIter(current_iter));
//     row_changed(getPath(current_iter),getIter(current_iter));
     iterator child=current_iter->second.children.begin();
     assert(child!=current_iter->second.children.end());
     row_inserted(getPath(child),getIter(child));
   }
   return current_iter;
}

void SimpleTreeStore::setSequence(const sequence_t &neu)
{  please_detach();
   ++stamp;
   currseq=neu; // Spaltenzahl anpassen?
   if (currseq.size()!=columns)
   {  columns=currseq.size();
      save_remembered();
      spaltenzahl_geaendert();
   }
   else if (columns_are_equivalent)
   {  for (unsigned i=0;i<Cols();++i) title_changed(i);
   }
   else spaltenzahl_geaendert();
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
static ManuProC::Tracer::Environment trace_channele("STS_TRACE",SimpleTreeStore::trace_channel);

void SimpleTreeStore::on_line_removed(cH_RowDataBase r)
{  ManuProC::Trace _t(trace_channel,__FUNCTION__,&*r);
   ++stamp;
   std::list<iterator> l=find_row(r,true);
   assert(sortierspalte==invisible_column);
   if (l.begin()==l.end()) 
   { std::cerr << "SimpleTreeStore::on_line_removed: trying again unoptimized\n";
     l=find_row(r,false);
   }
   if (l.begin()!=l.end())
   {  ManuProC::Trace(trace_channel,__FUNCTION__,"depth=",l.size());
      Path p=getPath(l.front());
      std::list<iterator>::const_iterator parent_iter=++l.begin();
      if (parent_iter==l.end()) 
      // nur 1 iterator, das zu löschende Element ist dann Kind von root
      { root.children.erase(l.front());
        row_deleted(p);
      }
      else
      { (*parent_iter)->second.children.erase(l.front());
        row_deleted(p);
      // we might be able to eliminate the nodes if they no longer
      //   have children
        for (std::list<iterator>::const_iterator i=parent_iter;i!=l.end();++i)
        { if (&*(*i)->second.leafdata==&*r)
          { if ((*i)->second.children.empty())
            { std::list<iterator>::const_iterator parent_iter2=i;
              ++parent_iter2;
              Path p2=getPath(*i);
              if (parent_iter2==l.end())
                root.children.erase(*i);
              else
                (*parent_iter2)->second.children.erase(*i);
              row_deleted(p2);
              goto continue_outer;
            }
            else
            { (*i)->second.leafdata=(*i)->second.children.begin()->second.leafdata;
            }
          }
          { Handle<TreeRow> htr=(*i)->second.row;
            if (htr)
            { htr->deduct(r);
              row_changed(getPath(*i),getIter(*i));
            }
          }
         continue_outer: ;
        }
      }
   }
   else std::cerr << "line to remove not found\n";
}

// optimize indicates we could binary search by value (possible optimization)
// this is impossible, if a value has already changed ...

bool SimpleTreeStore::find_row(Node &parent, const cH_RowDataBase &r,bool optimize,std::list<iterator> &result)
{ ManuProC::Trace _t(trace_channel,__FUNCTION__,&parent,&*r);
   if (optimize && parent.childrens_deep && sortierspalte==invisible_column)
   {  cH_EntryValue val=r->Value(currseq[parent.childrens_deep],ValueData());
      ManuProC::Trace(trace_channel,"",NV("depth",parent.childrens_deep),
                NV("val",val->getStrVal()));
      std::pair<iterator,iterator> p=parent.children.equal_range(val);
      if (p.first==p.second) 
      { ManuProC::Trace(trace_channel,"","empty equal_range");
        return false;
      }
      if (!p.first->second.childrens_deep) // nodes
      {  ManuProC::Trace(trace_channel,"","leaves");
         for (iterator i=p.first;i!=p.second;++i) 
            if (&*i->second.leafdata==&*r)
            {  result.push_back(i);
               return true;
            }
      }
      else 
      {  ManuProC::Trace(trace_channel,"","nodes");
         if (find_row(p.first->second,r,optimize,result))
         {  result.push_back(p.first);
            return true;
         }
      }
      ManuProC::Trace(trace_channel,"","not found");
      return false;
   }
   
   for (iterator i= parent.children.begin(); i!=parent.children.end(); ++i)
   {  if (i->second.children.empty())
      {  ManuProC::Trace(trace_channel,"",NV("found",&*i->second.leafdata));
         if (&*i->second.leafdata==&*r)
         {  result.push_back(i);
            return true;
         }
      }
      else 
      {  ManuProC::Trace(trace_channel,"","recurse");
         if (find_row(i->second,r,optimize,result))
         {  result.push_back(i);
            return true;
         }
      }
   }
   return false;
}

std::list<SimpleTreeStore::iterator> SimpleTreeStore::find_row(const cH_RowDataBase &r,bool optimize)
{ ManuProC::Trace _t(trace_channel,__FUNCTION__,&*r,optimize);
  std::list<iterator> result;
  find_row(root,r,optimize,result);
  return result;
}

void SimpleTreeStore::redisplay_old(Gtk::TreeModel::iterator it, unsigned index)
{  unsigned col=ColumnFromIndex(index);
   if (col==invisible_column) return;
   row_changed(get_path(it),it);
}

#if 1 // wird das noch gebraucht?
void SimpleTreeStore::redisplay_old(cH_RowDataBase data, unsigned index)
{  unsigned col=ColumnFromIndex(index);
   if (col==invisible_column) return;
   std::list<iterator> rows=find_row(data);
   if (rows.begin()!=rows.end())
     row_changed(getPath(rows.back()),getIter(rows.back()));
}
#endif

void SimpleTreeStore::set_tree_column_visibility(unsigned index,bool visible)
{  ShowColumn(index)=visible;
//   on_visibly_changed(vec_hide_cols.begin()+index);
}

Gtk::TreeModelFlags SimpleTreeStore::get_flags_vfunc() STS_VFUNC_CONST
{  return Gtk::TreeModelFlags(0);
  //Gtk::TREE_MODEL_ITERS_PERSIST; // Gtk::TreeModelFlags(0); 
}
int SimpleTreeStore::get_n_columns_vfunc() STS_VFUNC_CONST
{  return unsigned(s_text_start)+max_column; }

// speed this up by an array?
GType SimpleTreeStore::get_column_type_vfunc(int index) STS_VFUNC_CONST
{  switch(e_spalten(index))
   {  case s_row: return m_columns.row.type();
      case s_deep: return m_columns.deep.type();
      case s_childrens_deep: return m_columns.childrens_deep.type();
      case s_leafdata: return m_columns.leafdata.type();
      case s_background: return m_columns.background.type();
      default: return G_TYPE_STRING;
   }
}

unsigned SimpleTreeStore::IterStamp() const
{  return stamp;
}

bool SimpleTreeStore::iter_valid(vfunc_constiter_t iter) const
{  return iter STS_GTKMM_22_24(->stamp,.get_stamp())==IterStamp();
}

SimpleTreeStore::iterator &SimpleTreeStore::iterconv(vfunc_iter_t iter)
{  assert(iter_valid(iter));
   return reinterpret_cast<SimpleTreeStore::iterator&>(iter STS_GTKMM_22_24(,.gobj())->user_data);
}

const SimpleTreeStore::iterator &SimpleTreeStore::iterconv(vfunc_constiter_t iter) const
{  if (!iter_valid(iter))
   {  std::cerr << "iterconv: iterator mismatch " << iter STS_GTKMM_22_24(->stamp,.get_stamp()) << "!=" << IterStamp()
   	<< " user_data=" << iter STS_GTKMM_22_24(->,.gobj()->)user_data << '\n';
      abort();
   }
   return reinterpret_cast<const SimpleTreeStore::iterator&>(iter STS_GTKMM_22_24(->,.gobj()->)user_data);
}

void SimpleTreeStore::iterinit(vfunc_iter_t iter,const const_iterator &schema) const
{  assert(3*sizeof(iter STS_GTKMM_22_24(->,.gobj()->)user_data)>=sizeof(SimpleTreeStore::const_iterator));
   STS_GTKMM_22_24(iter->stamp=stamp, iter.set_stamp(stamp));
   reinterpret_cast<SimpleTreeStore::const_iterator&>(iter STS_GTKMM_22_24(->,.gobj()->)user_data)=schema;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2   
   iter.set_model_gobject(const_cast<GtkTreeModel*>(Gtk::TreeModel::gobj()));
#endif   
   ManuProC::Trace(trace_channel,__FUNCTION__,iter STS_GTKMM_22_24(->stamp,.get_stamp()),
   		iter STS_GTKMM_22_24(->,.gobj()->)user_data,
   		iter STS_GTKMM_22_24(->,.gobj()->)user_data2,
   		iter STS_GTKMM_22_24(->,.gobj()->)user_data3);
}

void SimpleTreeStore::iterclear(vfunc_iter_t iter) const
{  ManuProC::Trace(trace_channel,__FUNCTION__);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   iter=TreeModel::iterator();
#else
   memset(iter,0,sizeof(*iter));
#endif
}

void SimpleTreeStore::iterinit(vfunc_iter_t iter,const iterator &schema) const
{  iterinit(iter,static_cast<const_iterator>(schema));
}

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#define VALUE_INIT0(type) \
	g_value_init(value.gobj(),(type))
#define VALUE_SET(type,val) \
	g_value_set_##type(value.gobj(),(val))
#else
#define VALUE_INIT0(type) \
	g_value_init(value,(type))
#define VALUE_SET(type,val) \
	g_value_set_##type(value,(val))
#endif	

#define VALUE_INIT3(type2,name,val) \
	VALUE_INIT0(m_columns.name.type()); \
	VALUE_SET(type2,(val))
#define VALUE_INIT(name) VALUE_INIT3(boxed,name,&nd.name)
#define VALUE_INIT_U(name) VALUE_INIT3(uint,name,nd.name)
#define VALUE_STRING(val) \
	VALUE_SET(string,(val).c_str())

void SimpleTreeStore::get_value_vfunc(const TreeModel::iterator& iter, 
		int column, STS_GTKMM_22_24(GValue*,Glib::ValueBase&) value) STS_VFUNC_CONST
{  if (!iter_valid(iter STS_GTKMM_22_24(->gobj(),))) return;
   Node &nd=iterconv(iter STS_GTKMM_22_24(->gobj(),))->second;
   switch(e_spalten(column))
   {  case s_row: VALUE_INIT(row);
         return;
      case s_deep: VALUE_INIT_U(deep);
         return;
      case s_childrens_deep: VALUE_INIT_U(childrens_deep);
         return;
      case s_leafdata: VALUE_INIT(leafdata);
         return;
      case s_background: VALUE_INIT3(boxed,background,colors[nd.deep%num_colors].gobj());
         return;
      default:
         if (int(s_text_start)<=column && column<int(s_text_start)+int(max_column))
         {  VALUE_INIT0(G_TYPE_STRING);
            int colno=column-int(s_text_start);
            if (colno<0 || colno>=int(Cols())) return;
            unsigned idx=currseq[colno];
            if (nd.row)
            {  if (colno<int(nd.deep)) return;
               if (colno<int(nd.childrens_deep))
               {  VALUE_STRING(nd.leafdata->Value(idx,ValueData())->getStrVal());
                  return;
               }
               const Glib::ustring s=nd.row->Value(idx,ValueData())->getStrVal();
                  VALUE_STRING(s);
            }
            else if (nd.childrens_deep) // node
            {  if (unsigned(colno)>nd.childrens_deep || unsigned(colno)<nd.deep) 
                  return;
               if (colno!=int(nd.childrens_deep))
                  VALUE_STRING(nd.leafdata->Value(idx,ValueData())->getStrVal());
            }
            else // leaf
            {  if (unsigned(colno)<nd.deep) return;
               VALUE_STRING(nd.leafdata->Value(idx,ValueData())->getStrVal());
            }
         }
         return;
   }
}

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
bool SimpleTreeStore::iter_next_vfunc(vfunc_constiter_t iter, vfunc_iter_t iter_next) const
{
#else
bool SimpleTreeStore::iter_next_vfunc(vfunc_iter_t iter)
{  GtkTreeIter *iter_next=iter;
#endif
   ManuProC::Trace _t(trace_channel, __FUNCTION__,iter STS_GTKMM_22_24(,.gobj())->user_data);
   if (!iter_valid(iter)) { iterclear(iter_next); return false; }

   iterator old=iterconv(iter),newit=old;
   iterclear(iter_next);
   if (!old->second.parent) return false;
   newit++;
   if (newit==old->second.parent->children.end()) return false;
   iterinit(iter_next,newit);
   ManuProC::Trace(trace_channel,"new iter",iter_next STS_GTKMM_22_24(,.gobj())->user_data);
   return true;
}

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
bool SimpleTreeStore::iter_children_vfunc(vfunc_constiter_t parent, vfunc_iter_t iter) const
#else
bool SimpleTreeStore::iter_children_vfunc(vfunc_iter_t iter, vfunc_constiter_t parent) 
#endif
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,parent STS_GTKMM_22_24(,.gobj())->user_data);
   iterclear(iter);
   if (!iter_valid(parent)) return false;
   iterator p=iterconv(parent);
   if (p->second.children.empty()) return false;
   iterinit(iter,p->second.children.begin());
   return true;
}

bool SimpleTreeStore::iter_has_child_vfunc(vfunc_constiter_t iter) STS_VFUNC_CONST
{  if (!iter_valid(iter)) return false;
   return !(iterconv(iter)->second.children.empty());
}

int SimpleTreeStore::iter_n_children_vfunc(vfunc_constiter_t iter) STS_VFUNC_CONST
{  if (!iter_valid(iter)) return 0;
   return iterconv(iter)->second.children.size();
}

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
int SimpleTreeStore::iter_n_root_children_vfunc() const
{  return root.children.size();
}
#endif

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
bool SimpleTreeStore::iter_nth_child_vfunc(vfunc_constiter_t parent, int n, vfunc_iter_t iter) const
#else
bool SimpleTreeStore::iter_nth_child_vfunc(vfunc_iter_t iter, vfunc_constiter_t parent, int n)
#endif
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,parent?parent STS_GTKMM_22_24(,.gobj())->user_data:0,n);
   iterator res,end;
   iterclear(iter);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION<=2
   if (!parent)
   {  res=root.children.begin();
      end=root.children.end();
   }
   else
#endif   
   {  if (!iter_valid(parent)) return false;
      iterator p=iterconv(parent);
      res=p->second.children.begin();
      end=p->second.children.end();
   }
   
   if (res==end) return false;
   for (;n>0;--n) 
   {  ++res;
      if (res==end) return false;
   }
   iterinit(iter,res); 
   return true;
}

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
bool SimpleTreeStore::iter_nth_root_child_vfunc(int n, vfunc_iter_t iter) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,n);
   iterclear(iter);
   
   const_iterator res=root.children.begin(),
		   end=root.children.end();
   if (res==end) return false;
   for (;n>0;--n)
   {  ++res;
      if (res==end) return false;
   }
   iterinit(iter,res); 
   return true;
}
#endif

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
bool SimpleTreeStore::iter_parent_vfunc(vfunc_constiter_t child, vfunc_iter_t iter) const
#else
bool SimpleTreeStore::iter_parent_vfunc(vfunc_iter_t iter, vfunc_constiter_t child)
#endif
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,child STS_GTKMM_22_24(,.gobj())->user_data);
   iterclear(iter);
   if (!iter_valid(child)) return false;
   iterator c=iterconv(child);
   if (!c->second.parent || c->second.parent==&root) return false;  
   iterator p=iterbyNode(*c->second.parent);
   if (p==c->second.parent->parent->children.end()) return false;
   iterinit(iter,p);
   return true;
}

Gtk::TreeModel::Path SimpleTreeStore::get_path_vfunc(const Gtk::TreeModel::iterator& iter) STS_VFUNC_CONST
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,iter->gobj()->user_data);
  if (!iter_valid(iter STS_GTKMM_22_24(->gobj(),))) 
     return Path();
  return getPath(iterconv(iter STS_GTKMM_22_24(->gobj(),)));
}

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
bool SimpleTreeStore::get_iter_vfunc(const Path& path, vfunc_iter_t iter) const
#else
bool SimpleTreeStore::get_iter_vfunc(vfunc_iter_t iter, const Path& path)
#endif
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,path.to_string());
   
   STS_GTKMM_22_24(iterator,const_iterator) 
   		res=root.children.begin(),  end=root.children.end();
   
   iterclear(iter);
   for (Path::const_iterator piter=path.begin();piter!=path.end();)
   {  if (res==end) return false;
      for (unsigned i=*piter;i>0;--i) 
      {  ++res;
         if (res==end) return false;
      }
      ++piter;
      if (piter==path.end()) {  iterinit(iter,res); return true; }
      end=res->second.children.end();
      res=res->second.children.begin();
   }
   return false;
}

#ifdef OLD_MODEL
void MyTreeModel_Class::class_init_function(void* g_class, void* class_data)
{
}

const Glib::Class& MyTreeModel_Class::init()
{
    if (!gtype_)
    {
        class_init_func_ = &MyTreeModel_Class::class_init_function;

        static const GTypeInfo derived_info = 
        {
            sizeof(GObjectClass),
            NULL,
            NULL,
            class_init_func_,
            NULL,
            NULL,
            sizeof(GObject),
            0,
            0,
            NULL,
        };
        
        gtype_ = g_type_register_static(G_TYPE_OBJECT, "SimpleTree_MyTreeModel",
            &derived_info, GTypeFlags(0));

        Gtk::TreeModel::add_interface(get_type());
    }

    return *this;
}
#endif

SimpleTreeStore::iterator SimpleTreeStore::iterbyValue(Node &parent,const cH_EntryValue &val) const
{  return parent.children.find(val);
}

std::ostream &operator<<(std::ostream &o,const SimpleTreeStore::iterator &i)
{  return o << reinterpret_cast<const gconstpointer&>(i);
//(&*i);
}

SimpleTreeStore::iterator SimpleTreeStore::iterbyNode(Node &nd) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,&nd);
   cH_EntryValue val=nd.leafdata->Value(currseq[nd.deep],ValueData());
   std::pair<iterator,iterator> p;
   if (sortierspalte==invisible_column) p=nd.parent->children.equal_range(val);
   else p=make_pair(nd.parent->children.begin(),nd.parent->children.end());
   for (iterator i=p.first;i!=p.second;++i) 
   {  ManuProC::Trace(trace_channel,"i",&i->second);
      if (&i->second==&nd) return i;
   }
   return nd.parent->children.end();
}

void SimpleTreeStoreNode::swap(SimpleTreeStoreNode &b)
{  std::swap(children,b.children);
   std::swap(row,b.row);
   std::swap(leafdata,b.leafdata);
   std::swap(parent,b.parent);
   std::swap(deep,b.deep);
   std::swap(childrens_deep,b.childrens_deep);
}

void std::swap(SimpleTreeStoreNode &a,SimpleTreeStoreNode &b)
{  a.swap(b);
}

void SimpleTreeStoreNode::fix_pointer()
{  //ManuProC::Trace _t(SimpleTreeStore::trace_channel,__FUNCTION__);
   for (iterator i=children.begin();i!=children.end();++i)
   {  if (i->second.parent!=this)
      {  ManuProC::Trace(SimpleTreeStore::trace_channel,__FUNCTION__,i->second.parent,this);
         i->second.parent=this;
      }
      i->second.fix_pointer();
   }
}

Gtk::TreeModel::iterator SimpleTreeStore::getIter(iterator it) const
{
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   TreeModel::iterator res;
   iterinit(res,it);
   return res;
#else
   GtkTreeIter res;
   iterinit(&res,it);
   return Gtk::TreeModel::iterator(const_cast<GtkTreeModel*>(gobj()),&res);
#endif
}

Gtk::TreeModel::const_iterator SimpleTreeStore::getIter(const_iterator it) const
{  
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   TreeModel::iterator res;
   iterinit(res,it);
   return res;
#else
   GtkTreeIter res;
   iterinit(&res,it);
   return Gtk::TreeModel::iterator(const_cast<GtkTreeModel*>(gobj()),&res);
#endif
}

Gtk::TreeModel::Path SimpleTreeStore::getPath(const cH_RowDataBase &data) const
{ std::list<iterator> l=const_cast<SimpleTreeStore*>(this)->find_row(data,true);
  // optimized search
  if (l.empty()) return Path();
  return getPath(l.front());
}

Gtk::TreeModel::Path SimpleTreeStore::getPath(iterator it) const
{  std::vector<unsigned> store;
   store.reserve(max_column);
   while(true)
   {  store.push_back(Node2nth_child(it->second));
      if (it->second.parent==&root) break;
      it=iterbyNode(*it->second.parent);
   }
   Path res;
   for (std::vector<unsigned>::reverse_iterator i=store.rbegin();i!=store.rend();++i)
   {  res.push_back(*i);
   }
   ManuProC::Trace _t(trace_channel,__FUNCTION__,res.to_string());
   return res;
}

unsigned SimpleTreeStore::Node2nth_child(const Node &nd) const
{  unsigned res=0;
   for (const_iterator i=nd.parent->children.begin();
   			i!=nd.parent->children.end();++i,++res)
      if (&i->second==&nd) return res;
   abort();
}

void SimpleTreeStore::setSortierspalte(unsigned s,bool i)
{  if (sortierspalte!=s || i!=invert_sortierspalte)
   {  please_detach();
      ++stamp;
      sortierspalte=s;
      invert_sortierspalte=i;
      redisplay();
   }
}

Glib::RefPtr<SimpleTreeStore> SimpleTreeStore::create(int cols)
{
  return Glib::RefPtr<SimpleTreeStore>( new SimpleTreeStore(cols));
}

bool SimpleTreeStore::row_draggable_vfunc(const Gtk::TreeModel::Path& path) const
{ return true;
}
bool SimpleTreeStore::drag_data_get_vfunc(const Gtk::TreeModel::Path& path, Gtk::SelectionData& selection_data) const
{ return false;
}
bool SimpleTreeStore::drag_data_delete_vfunc(const Gtk::TreeModel::Path& path)
{ return false;
}
bool SimpleTreeStore::drag_data_received_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data)
{ return false;
}
bool SimpleTreeStore::row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data) const
{ return false;
}

void SimpleTreeStore::value_change_impl(cH_RowDataBase row,unsigned idx,std::string const& newval, bool &has_changed)
{ has_changed|=row.cast_const<RowDataBase>()->changeValue(idx,gp,newval);
}
