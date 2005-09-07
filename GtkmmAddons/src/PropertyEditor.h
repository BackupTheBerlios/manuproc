// $Id: PropertyEditor.h,v 1.1 2005/09/07 16:22:52 christof Exp $

#ifndef A169CA7A_BD70_4162_88EA_58016652E754
#define A169CA7A_BD70_4162_88EA_58016652E754

#include <gtkmm/dialog.h>
#include <gtkmm/treeview.h>

namespace ManuProC {

class PropertyStore;

class PropertyEditor : Gtk::Dialog
{ Gtk::Widget *top;
  Gtk::TreeView *tree;
  
  PropertyStore store;
//  Glib::RefPtr<SimpleTreeStore> store; ??
public:
  PropertyEditor(const Gtk::Widget *_top);
};

}
#endif
