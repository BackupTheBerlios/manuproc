#include "SearchCombo2.h"
#include <gtkmm/liststore.h>
#include <cassert>

struct Gtk::SearchCombo2::Model : Gtk::ListStore
//virtual Glib::Object, Gtk::TreeModel
{ 
private:
  Gtk::ComboBoxEntry *combo;
  Model(const TreeModelColumnRecord& columns, Gtk::ComboBoxEntry *e);
//  std::vector<std::string> results;

  bool fill_idle();
  void fill_list();
  bool close_search();
  void clear_list();
public:
  sigc::signal2<void,gboolean *,GtkSCContext> search;
  
  static Glib::RefPtr<Model> create(const TreeModelColumnRecord& columns, Gtk::ComboBoxEntry *e);
  
  void entry_changed();
  void stop_if_running();

  bool start_idle:1;	/** start search at idle time not in focus_in */

private:
  /* internal state variables */
  sigc::connection idle_conn;
  
  bool backspace:1;	/** last key was backspace -> do not autocomplete */
  bool search_in_progress:1;	/** whether a search is in progress */
  bool already_started:1;	/** if not true we have to start at idle time */
  bool search_finished:1;	/** search is finished, simply display */
  bool value_selected:1;	/** a value has been selected, do not search without explicit request */
  bool reopen:1;		/** next open should be reopen */

/*
   typedef const TreeModel::iterator& vfunc_constiter_t;
   typedef TreeModel::iterator& vfunc_iter_t;
// vfunc overrides for my tree model
   virtual bool row_draggable_vfunc(const Gtk::TreeModel::Path& path) const;
   virtual bool drag_data_get_vfunc(const Gtk::TreeModel::Path& path, Gtk::SelectionData& selection_data) const;
   virtual bool drag_data_delete_vfunc(const Gtk::TreeModel::Path& path);
   virtual bool drag_data_received_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data);
   virtual bool row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data) const;

   virtual Gtk::TreeModelFlags get_flags_vfunc() const;
   virtual int get_n_columns_vfunc() const;
   virtual GType get_column_type_vfunc(int index) const;
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
   virtual Path get_path_vfunc(const TreeModel::iterator& iter) const; */
};

Gtk::SearchCombo2::Model::Model(const TreeModelColumnRecord& columns, Gtk::ComboBoxEntry *e)
  : ListStore(columns), combo(e),
    start_idle(true), backspace(), search_in_progress(), already_started(),
    search_finished(), value_selected(), reopen()
//  : Glib::ObjectBase( typeid(Model) ), //register a custom GType.
//    Glib::Object() //The custom GType is actually registered here.
{
}

void Gtk::SearchCombo2::Model::clear_list()
{ stop_if_running();
  Gtk::ListStore::clear();
  combo->popdown();
  search_finished=false;
  value_selected=false;
}

void Gtk::SearchCombo2::Model::fill_list()
{ gboolean continue_=false;
  reopen=value_selected;
  clear_list();
  assert(!search_in_progress);
  search_in_progress=true;
  already_started=false;
  value_selected=false;
/*  if (!start_idle)
  {
  } else */
    continue_=true;
  if (continue_)
  { idle_conn=Glib::signal_idle().connect(sigc::mem_fun(*this,&Model::fill_idle));
  }
  else close_search();
  combo->popup();
}

void Gtk::SearchCombo2::Model::entry_changed()
{ // narrow optimization (gtksearchcombo.c:712)
  fill_list();
}

bool Gtk::SearchCombo2::Model::close_search()
{ search_in_progress=false;
  search_finished=true;
  idle_conn.disconnect();
  search(0, GTK_SEARCH_CLOSE);
  /* if (autoexpand && !backspace)
  { 
  } */
  return false;
}

bool Gtk::SearchCombo2::Model::fill_idle()
{ gboolean continue_=false;
  // THREADS_ENTER?
  assert(search_in_progress && !search_finished);
  search(&continue_, already_started?GTK_SEARCH_FETCH:
    (reopen?GTK_SEARCH_REOPEN:GTK_SEARCH_OPEN));
  already_started=true;
  if (!continue_)
  { close_search();
  }
  return continue_;
}

void Gtk::SearchCombo2::Model::stop_if_running()
{ if (search_in_progress) 
  { idle_conn.disconnect();
    search_in_progress=false;
    if (already_started)
      search(0, GTK_SEARCH_CLOSE);
  }
}

Glib::RefPtr<Gtk::SearchCombo2::Model> Gtk::SearchCombo2::Model::create(const TreeModelColumnRecord& columns, Gtk::ComboBoxEntry *e)
{ return Glib::RefPtr<Model>(new Model(columns,e));
}

void Gtk::SearchCombo2::on_entry_changed()
{ mymodel->entry_changed();
}

Gtk::SearchCombo2::SearchCombo2(bool,bool)
{ mymodel=Model::create(m_text_columns,this);
  get_entry()->signal_changed().connect(sigc::mem_fun(*this,&SearchCombo2::on_entry_changed));
}

Gtk::SearchCombo2::~SearchCombo2()
{
}

Glib::SignalProxy0<void> Gtk::SearchCombo2::signal_activate()
{ return get_entry()->signal_activate();
}

sigc::signal2<void,gboolean *,GtkSCContext> &Gtk::SearchCombo2::signal_search()
{ return mymodel->search;
}

Glib::ustring Gtk::SearchCombo2::get_text() const
{ return get_entry()->get_text();
}

void Gtk::SearchCombo2::add_item(const Glib::ustring &text)
{ Gtk::TreeModel::iterator i=mymodel->append();
  (*i)[m_text_columns.m_column]=text;
}
