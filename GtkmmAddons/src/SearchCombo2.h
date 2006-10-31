/* $Id: SearchCombo2.h,v 1.4 2006/10/31 16:14:14 christof Exp $ */

/* searchcombo.h
 * 
 * Copyright (C) 1999-2002 Adolf Petig GmbH & Co. KG
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <gtkmm/comboboxentry.h>

typedef enum
{
  GTK_SEARCH_FETCH,
  GTK_SEARCH_OPEN,
  GTK_SEARCH_CLOSE,
  GTK_SEARCH_REOPEN
} GtkSCContext;

namespace Gtk
{
class SearchCombo2 : public Gtk::ComboBoxEntry
{ struct TextModelColumns : Gtk::TreeModel::ColumnRecord
  { TextModelColumns() { add(m_column); }

    Gtk::TreeModelColumn<Glib::ustring> m_column;
  };
  TextModelColumns m_text_columns;
  struct Model;
  
  Glib::RefPtr<Model> mymodel;
  
	bool ok_if_empty:1; /** not yet used */
	bool value_in_list:1;
	bool case_sensitive:1;
	bool always_fill:1;	/** whether to call search on an empty pattern */
	bool enable_tab:1;	/** whether to recognize tab for completion */
	bool autoexpand:1;	/** whether to automatically expand and activate once unique */
	bool autoexpand_on_activate:1; /** whether to expand on pressing Return */
	bool auto_narrow:1;  /** do not call search on key press but
				instread try to shrink displayed list ourselves */
	

  void on_entry_changed();
  void popupdown();
//  sigc::signal0<void> activate;
public:
  explicit SearchCombo2(bool always_fill=false, bool autoexpand=false);
  ~SearchCombo2();
  
  Glib::SignalProxy0<void> signal_activate();
  sigc::signal2<void,gboolean *,GtkSCContext> &signal_search();

  /** Allow direct text entry
   * Whether the text in the entry must be or not be in the list.
   *
   * @param value Set to true if the value must be in list.
   *
   * @param empty Set to true if the text area is allowed to be empty.
   */
  void set_value_in_list(bool value = true, bool empty = false);

  /** Sets list case sensitive
   * Determines if the list items and text comparisons for
   * set_use_arrows_always() should be case sensitive.
   */
  
  void set_case_sensitive(bool val = true);

  //: Append an item.
  
  Gtk::TreeModel::iterator add_item(const Glib::ustring &text);
  
  //: clear entry+list, mark for research
  
  void reset();

  //: set text no research
  
  void set_text(const Glib::ustring &text);
  
  Glib::ustring get_text() const;
//  const Glib::ustring get_text() const;
  
  void set_always_fill(bool val);
  void set_enable_tab(bool val);
  guint get_size() const;

  //: set sensitivity
  
  void set_editable(bool val);

  //: autoexpand - think twice!
  
  void set_autoexpand(bool val);
    
  void trigger_search();
  
  void set_start_on_idle(bool val);
    
  gint get_selected_index() const;
   
    bool empty() const {  return !get_size(); }
    guint size() const {  return get_size(); }

};
}
