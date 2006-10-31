#include "SearchCombo2.h"
#include <gtkmm/liststore.h>

struct Gtk::SearchCombo2::Model : Gtk::ListStore
//virtual Glib::Object, Gtk::TreeModel
{ 
private:
  Model(const TreeModelColumnRecord& columns);
//  std::vector<std::string> results;

public:
  sigc::signal2<void,gboolean *,GtkSCContext> search;
  
  static Glib::RefPtr<Model> create(const TreeModelColumnRecord& columns);

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

Gtk::SearchCombo2::Model::Model(const TreeModelColumnRecord& columns)
  : ListStore(columns)
//  : Glib::ObjectBase( typeid(Model) ), //register a custom GType.
//    Glib::Object() //The custom GType is actually registered here.
{
}

Glib::RefPtr<Gtk::SearchCombo2::Model> Gtk::SearchCombo2::Model::create(const TreeModelColumnRecord& columns)
{ return Glib::RefPtr<Model>(new Model(columns));
}

Gtk::SearchCombo2::SearchCombo2(bool,bool)
{ mymodel=Model::create(m_text_columns);
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
