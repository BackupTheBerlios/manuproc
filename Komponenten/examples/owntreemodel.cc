// $Id: owntreemodel.cc,v 1.4 2003/10/13 06:34:15 christof Exp $
/*  ManuProcWidgets: ManuProC's GUI element library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <gtkmm/window.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/main.h>
#include <cassert>
#include <vector>
#include <cstdio>
#include <iostream>
#include <Misc/TraceNV.h>

static const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channele("TRACE",trace_channel);

static const unsigned columns=10;
static const unsigned multiply1=1<<4;
static const unsigned multiply2=multiply1*multiply1;
static const unsigned multiply4=multiply2*multiply2;
static const unsigned int maxdepth=7;
static const unsigned rows=multiply1-2;

static const unsigned multiply[maxdepth+1]=
{  1,multiply1,multiply2,multiply2*multiply1,
   multiply4,multiply4*multiply1,multiply4*multiply2,multiply4*multiply2*multiply1
};

static std::string itoa(int i)
{  char buf[20];
   snprintf(buf,sizeof buf,"%d",i);
   buf[sizeof(buf)-1]=0;
   return buf;
}

static unsigned &iter2row(GtkTreeIter* iter)
{  return reinterpret_cast<unsigned&>(iter->user_data);
}

static unsigned iter2row(const GtkTreeIter* iter)
{  return reinterpret_cast<unsigned>(iter->user_data);
}

unsigned depth(unsigned i)
{  for (unsigned j=maxdepth;j>0;--j) if (i>=multiply[j]) return j;
   return 0;
}

unsigned rowno(unsigned i,unsigned d)
{  if (!d) return i%multiply1;
   return (i/multiply[d])%multiply1 -1;
}

class MyTreeModel_Class : public Glib::Class
{public:
	const Glib::Class& init();
	static void class_init_function(void* g_class, void* class_data);
};

class MyTreeModel : public Glib::Object, public Gtk::TreeModel
{  static MyTreeModel_Class myclass;

   virtual Gtk::TreeModelFlags get_flags_vfunc()
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__);
      return Gtk::TreeModelFlags(0); }
   virtual int get_n_columns_vfunc()
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__);
      return columns; }
   virtual GType get_column_type_vfunc(int index)
   {  return G_TYPE_STRING; }
   virtual void get_value_vfunc(const TreeModel::iterator& iter, int column, 
   		GValue* value)
   {  g_value_init(value,G_TYPE_STRING);
      std::string res=itoa(iter2row(iter.gobj()))+"-"+itoa(column);
      g_value_set_string(value,res.c_str());
   }
   virtual bool iter_next_vfunc(GtkTreeIter* iter)
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__,iter2row(iter));
      unsigned d=depth(iter2row(iter));
      iter2row(iter)+=multiply[d];
      return rowno(iter2row(iter),d)<rows;
   }
   virtual bool iter_children_vfunc(GtkTreeIter* iter, const GtkTreeIter* parent)
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__,iter2row(parent));
      unsigned d=depth(iter2row(parent))+1;
      if (d>maxdepth) return false;
      iter2row(iter)=multiply[d]+iter2row(parent);
      return true;
   }
   virtual bool iter_has_child_vfunc(const GtkTreeIter* iter)
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__,iter2row(iter));
      return depth(iter2row(iter))<maxdepth;
   }
   virtual int iter_n_children_vfunc(const GtkTreeIter* iter)
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__,iter2row(iter));
      return iter_has_child_vfunc(iter)?rows:0;
   }
   virtual bool iter_nth_child_vfunc(GtkTreeIter* iter, const GtkTreeIter* parent, int n)
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__,iter2row(parent),n);
      unsigned r=parent?iter2row(parent):0;
      unsigned d=depth(r)+1;
      if (d>maxdepth) return false;
      iter2row(iter)=n*multiply[d]+r;
      return n<rows;
   }
   virtual bool iter_parent_vfunc(GtkTreeIter* iter, const GtkTreeIter* child)
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__,iter2row(child));
      unsigned d=depth(iter2row(child));
      if (!d) return false;
      iter2row(iter)=iter2row(child)%multiply[d];
      return true;
   }
   virtual Gtk::TreeModel::Path get_path_vfunc(const Gtk::TreeModel::iterator& iter)
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__,iter2row(iter->gobj()));
      return Gtk::TreeModel::Path();
   }
   virtual bool get_iter_vfunc(GtkTreeIter* iter, const Gtk::TreeModel::Path& path)
   {  ManuProC::Trace _t(trace_channel, __FUNCTION__,path.to_string());
      unsigned sz=path.size();
      if (!sz || path[0]>=rows) return false;
      iter2row(iter)=path[0];
      for (unsigned j=1;j<maxdepth;++j)
      {  if (path[j]>=rows) return false;
         if (sz==j) return true;
         iter2row(iter)+=(path[j]+1)*multiply[j];
      }
      return false;
   }
   
public:
   MyTreeModel()
     : Glib::ObjectBase("MyTreeModel"),
//     typeid(MyTreeModel))
       Glib::Object(Glib::ConstructParams(myclass.init(), (char*) 0))
   {  
   }
};

MyTreeModel_Class MyTreeModel::myclass;

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
        
        gtype_ = g_type_register_static(G_TYPE_OBJECT, "MyTreeModel",
            &derived_info, GTypeFlags(0));

        Gtk::TreeModel::add_interface(get_type());
    }

    return *this;
}

int main(int argc,char **argv)
{  Gtk::Main m(argc,argv);
   Gtk::Window w;
   Gtk::TreeView v;
   Gtk::ScrolledWindow sw;
   Glib::RefPtr<Gtk::TreeModel> model
   	=Glib::RefPtr<Gtk::TreeModel>(new MyTreeModel);
   std::vector<Gtk::TreeModelColumn< int > > cols(columns);
   Gtk::TreeModel::ColumnRecord colrec;
   for (unsigned i=0;i<columns;++i) colrec.add(cols[i]);
   w.add(sw);
   sw.add(v);
   v.set_model(model);

   for (unsigned i=0;i<columns;++i)
      v.append_column("data",cols[i]);
   
   v.show();
   sw.show();
   w.show();
   m.run(w);
}
