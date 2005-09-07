// $Id: PropertyEditor.cc,v 1.1 2005/09/07 16:22:52 christof Exp $

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2

#include "PropertyEditor.h"

class ManuProC::PropertyStore : public Glib::Object, public Gtk::TreeModel
{ Gtk::Widget *top;
  
   virtual Gtk::TreeModelFlags get_flags_vfunc() STS_VFUNC_CONST;
   virtual int get_n_columns_vfunc() STS_VFUNC_CONST;
   virtual GType get_column_type_vfunc(int index) STS_VFUNC_CONST;
   virtual void get_value_vfunc(vfunc_constiter_t iter, int column, Glib::ValueBase& value) const;
   bool iter_next_vfunc(vfunc_constiter_t iter, vfunc_iter_t iter_next) const;
   virtual bool iter_children_vfunc(vfunc_constiter_t parent, vfunc_iter_t iter) const;
   virtual bool iter_has_child_vfunc(vfunc_constiter_t iter) const;
   virtual int iter_n_children_vfunc(vfunc_constiter_t iter) const;
   virtual int iter_n_root_children_vfunc() const;
   virtual bool iter_nth_child_vfunc(vfunc_constiter_t parent, int n, vfunc_iter_t iter) const;
   virtual bool iter_nth_root_child_vfunc(int n, vfunc_iter_t iter) const;
   virtual bool iter_parent_vfunc(vfunc_constiter_t child, vfunc_iter_t iter) const;
   virtual bool get_iter_vfunc(const Path& path, vfunc_iter_t iter) const;
   virtual Path get_path_vfunc(const TreeModel::iterator& iter) STS_VFUNC_CONST;
public:
  struct Columns : public Gtk::TreeModelColumnRecord
  { Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<bool> visible;
    
    Columns();
  }
  Columns cols;
  PropertyStore(Gtk::Widget *_top);
  
  static Glib::RefPtr<SimpleTreeStore> create();
};

ManuProC::PropertyStore::PropertyStore(Gtk::Widget *t)
  : Glib::ObjectBase( typeid(PropertyStore) ), //register a custom GType.
    Glib::Object(), //The custom GType is actually registered here.
    top(t)
{ static bool inited=false;
  if (!inited)
  { Gtk::TreeModel::add_interface( Glib::Object::get_type() );
    inited=true;
  }
}

ManuProC::PropertyStore::Columns::Columns()
{ add(name);
  add(visible);
}

Gtk::TreeModelFlags ManuProC::PropertyStore::get_flags_vfunc() STS_VFUNC_CONST
{  return Gtk::TreeModelFlags(0);
  //Gtk::TREE_MODEL_ITERS_PERSIST; // Gtk::TreeModelFlags(0); 
}
int ManuProC::PropertyStore::get_n_columns_vfunc() STS_VFUNC_CONST
{  return unsigned(s_text_start)+max_column; }

// speed this up by an array?
GType ManuProC::PropertyStore::get_column_type_vfunc(int index) STS_VFUNC_CONST
{  switch(e_spalten(index))
   {  case s_row: return m_columns.row.type();
      case s_deep: return m_columns.deep.type();
      case s_childrens_deep: return m_columns.childrens_deep.type();
      case s_leafdata: return m_columns.leafdata.type();
      case s_background: return m_columns.background.type();
      default: return G_TYPE_STRING;
   }
}

unsigned ManuProC::PropertyStore::IterStamp() const
{  return stamp;
}

bool ManuProC::PropertyStore::iter_valid(vfunc_constiter_t iter) const
{  return iter STS_GTKMM_22_24(->stamp,.get_stamp())==IterStamp();
}

ManuProC::PropertyStore::iterator &ManuProC::PropertyStore::iterconv(vfunc_iter_t iter)
{  assert(iter_valid(iter));
   return reinterpret_cast<ManuProC::PropertyStore::iterator&>(iter STS_GTKMM_22_24(,.gobj())->user_data);
}

const ManuProC::PropertyStore::iterator &ManuProC::PropertyStore::iterconv(vfunc_constiter_t iter) const
{  if (!iter_valid(iter))
   {  std::cerr << "iterconv: iterator mismatch " << iter STS_GTKMM_22_24(->stamp,.get_stamp()) << "!=" << IterStamp()
   	<< " user_data=" << iter STS_GTKMM_22_24(->,.gobj()->)user_data << '\n';
      abort();
   }
   return reinterpret_cast<const ManuProC::PropertyStore::iterator&>(iter STS_GTKMM_22_24(->,.gobj()->)user_data);
}

void ManuProC::PropertyStore::iterinit(vfunc_iter_t iter,const const_iterator &schema) const
{  assert(3*sizeof(iter STS_GTKMM_22_24(->,.gobj()->)user_data)>=sizeof(ManuProC::PropertyStore::const_iterator));
   STS_GTKMM_22_24(iter->stamp=stamp, iter.set_stamp(stamp));
   reinterpret_cast<ManuProC::PropertyStore::const_iterator&>(iter STS_GTKMM_22_24(->,.gobj()->)user_data)=schema;
   iter.set_model_gobject(const_cast<GtkTreeModel*>(gobj()));
   ManuProC::Trace(trace_channel,__FUNCTION__,iter STS_GTKMM_22_24(->stamp,.get_stamp()),
   		iter STS_GTKMM_22_24(->,.gobj()->)user_data,
   		iter STS_GTKMM_22_24(->,.gobj()->)user_data2,
   		iter STS_GTKMM_22_24(->,.gobj()->)user_data3);
}

void ManuProC::PropertyStore::iterclear(vfunc_iter_t iter) const
{  ManuProC::Trace(trace_channel,__FUNCTION__);
   iter=TreeModel::iterator();
}

void ManuProC::PropertyStore::iterinit(vfunc_iter_t iter,const iterator &schema) const
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

void ManuProC::PropertyStore::get_value_vfunc(const TreeModel::iterator& iter, 
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

bool ManuProC::PropertyStore::iter_next_vfunc(vfunc_constiter_t iter, vfunc_iter_t iter_next) const
{
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

bool ManuProC::PropertyStore::iter_children_vfunc(vfunc_constiter_t parent, vfunc_iter_t iter) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,parent STS_GTKMM_22_24(,.gobj())->user_data);
   iterclear(iter);
   if (!iter_valid(parent)) return false;
   iterator p=iterconv(parent);
   if (p->second.children.empty()) return false;
   iterinit(iter,p->second.children.begin());
   return true;
}

bool ManuProC::PropertyStore::iter_has_child_vfunc(vfunc_constiter_t iter) STS_VFUNC_CONST
{  if (!iter_valid(iter)) return false;
   return !(iterconv(iter)->second.children.empty());
}

int ManuProC::PropertyStore::iter_n_children_vfunc(vfunc_constiter_t iter) STS_VFUNC_CONST
{  if (!iter_valid(iter)) return 0;
   return iterconv(iter)->second.children.size();
}

int ManuProC::PropertyStore::iter_n_root_children_vfunc() const
{  return root.children.size();
}

bool ManuProC::PropertyStore::iter_nth_child_vfunc(vfunc_constiter_t parent, int n, vfunc_iter_t iter) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,parent?parent STS_GTKMM_22_24(,.gobj())->user_data:0,n);
   iterator res,end;
   iterclear(iter);
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

bool ManuProC::PropertyStore::iter_nth_root_child_vfunc(int n, vfunc_iter_t iter) const
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

bool ManuProC::PropertyStore::iter_parent_vfunc(vfunc_constiter_t child, vfunc_iter_t iter) const
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

Gtk::TreeModel::Path ManuProC::PropertyStore::get_path_vfunc(const Gtk::TreeModel::iterator& iter) STS_VFUNC_CONST
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,iter->gobj()->user_data);
  if (!iter_valid(iter STS_GTKMM_22_24(->gobj(),))) 
     return Path();
  return getPath(iterconv(iter STS_GTKMM_22_24(->gobj(),)));
}

bool ManuProC::PropertyStore::get_iter_vfunc(const Path& path, vfunc_iter_t iter) const
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

#endif
