
/*  SearchCombo gbwidget (for glade)
 *  Copyright (C) 1998  Christof Petig
 *
 *  this file is based upon gbcombo.c by Damon Chaplin
 *          and gtkcombo.c (gtk-1.0.6)
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

/* 2do: construction argument, customization */

#include <string.h>

#include <gtk/gtk.h>

/************************************
 * search combo dummy widget
 ***********************************/
 
struct _SearchComboBox
{	GtkCombo combo;
	guint always_fill:1;
	guint enable_tab:1;
	guint autoexpand:1;
};

struct _SearchComboBoxClass
{	GtkComboClass parent_class;
	int (*search)(void*,gboolean *continue_, gboolean new_);
	void (*activate)(void);
};

typedef struct _SearchComboBox SearchComboBox;
typedef struct _SearchComboBoxClass SearchComboBoxClass;

#define SEARCHCOMBO(obj)			GTK_CHECK_CAST (obj, searchcombo_get_type (), SearchComboBox)
#define SEARCHCOMBO_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, searchcombo_get_type (), SearchComboBoxClass)
#define IS_SEARCHCOMBO(obj)       GTK_CHECK_TYPE (obj, searchcombo_get_type ())

static GtkComboClass *parent_class = NULL;
static gint searchcombo_signals[3] = { 0 };

static guint searchcombo_get_type ();

static void
searchcombo_init (SearchComboBox * combo)
{ combo->always_fill = 0;
  combo->autoexpand = 0;
  combo->enable_tab = 1;
}

static void
searchcombo_set_always_fill(SearchComboBox * combo, gboolean a)
{ g_return_if_fail (combo != NULL);
  g_return_if_fail (IS_SEARCHCOMBO (combo));
  combo->always_fill = a;
} 

static void
searchcombo_set_enable_tab(SearchComboBox * combo, gboolean a)
{ g_return_if_fail (combo != NULL);
  g_return_if_fail (IS_SEARCHCOMBO (combo));
  combo->enable_tab = a;
} 

static void
searchcombo_set_autoexpand(SearchComboBox * combo, gboolean a)
{ g_return_if_fail (combo != NULL);
  g_return_if_fail (IS_SEARCHCOMBO (combo));
  combo->autoexpand = a;
} 

static void
searchcombo_size_allocate (GtkWidget     *widget,
			 GtkAllocation *allocation)
{ g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_COMBO (widget));
  g_return_if_fail (allocation != NULL);
  
  GTK_WIDGET_CLASS (parent_class)->size_allocate (widget, allocation);
}

static void
searchcombo_destroy (GtkObject * combo)
{ if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (*GTK_OBJECT_CLASS (parent_class)->destroy) (combo);
}

static void
searchcombo_class_init (SearchComboBoxClass * klass)
{
  GtkObjectClass *oclass;
  GtkWidgetClass *widget_class;

  parent_class = gtk_type_class (gtk_combo_get_type ());
  
  oclass = (GtkObjectClass *) klass;
  oclass->destroy = searchcombo_destroy;
  
  widget_class = (GtkWidgetClass *) klass;
  widget_class->size_allocate = searchcombo_size_allocate;
  
  searchcombo_signals[0] = gtk_signal_new ("search",
  		GTK_RUN_FIRST,
  		oclass->type,
  		GTK_SIGNAL_OFFSET (SearchComboBoxClass, search),
  		gtk_signal_default_marshaller, GTK_TYPE_NONE, 0);
  /* marshaller and type are wrong - who cares */
  searchcombo_signals[1] = gtk_signal_new ("activate",
  		GTK_RUN_FIRST,
  		oclass->type,
  		GTK_SIGNAL_OFFSET (SearchComboBoxClass, activate),
  		gtk_signal_default_marshaller, GTK_TYPE_NONE, 0);
  /* marshaller and type are wrong - who cares */
  gtk_object_class_add_signals (oclass, searchcombo_signals, 3);
  klass->search=NULL;
  klass->activate=NULL;
} 

static guint
searchcombo_get_type ()
{
  static guint searchcombo_type = 0;
  
  if (!searchcombo_type)
  {   static const GtkTypeInfo searchcombo_info =
      { "GtkSearchCombo",
	sizeof (SearchComboBox),
	sizeof (SearchComboBoxClass),
	(GtkClassInitFunc) searchcombo_class_init,
	(GtkObjectInitFunc) searchcombo_init,
        (GtkArgSetFunc) NULL,
	(GtkArgSetFunc) NULL
      };
    searchcombo_type = gtk_type_unique(gtk_combo_get_type(),&searchcombo_info);
  }
  return searchcombo_type;
}

static GtkWidget *
searchcombo_new ()
{
  return GTK_WIDGET (gtk_type_new (searchcombo_get_type()));
}

/************************************/

#include "../gb.h"

/* Include the 21x21 icon pixmap for this widget, to be used in the palette */
#include "../graphics/searchcombo.xpm"

/*
 * This is the GbWidget struct for this widget (see ../gbwidget.h).
 * It is initialized in the init() function at the end of this file
 */
static GbWidget gbwidget;

static gchar *Case = "SearchComboBox::case_sensitive";
static gchar *AlwaysFill = "SearchComboBox::always_fill";
static gchar *EnableTab = "SearchComboBox::enable_tab";
static gchar *AutoExpand = "SearchComboBox::autoexpand";

/******
 * NOTE: To use these functions you need to uncomment them AND add a pointer
 * to the function in the GbWidget struct at the end of this file.
 ******/

/*
 * Creates a new GtkWidget of class SearchComboBox, performing any specialized
 * initialization needed for the widget to work correctly in this environment.
 * If a dialog box is used to initialize the widget, return NULL from this
 * function, and call data->callback with your new widget when it is done.
 * If the widget needs a special destroy handler, add a signal here.
 */

   GtkWidget*
   gb_searchcombo_new(GbWidgetNewData *data)
   {  return searchcombo_new();
   }



/*
 * Creates the components needed to edit the extra properties of this widget.
 */
static void
gb_searchcombo_create_properties (GtkWidget * widget, GbWidgetCreateArgData * data)
{
  property_add_bool (Case, _("Case Sens.:"),
		     _("If the searching is case sensitive"));
  property_add_bool (AlwaysFill, _("Always Fill:"),
		     _("If search is started also with an empty pattern"));
  property_add_bool (EnableTab, _("Enabl. TAB:"),
		     _("If tab completes"));
  property_add_bool (AutoExpand, _("Autoexp.:"),
		     _("If any unique selection fires activate"));
}



/*
 * Gets the properties of the widget. This is used for both displaying the
 * properties in the property editor, and also for saving the properties.
 */
static void
gb_searchcombo_get_properties (GtkWidget * widget, GbWidgetGetArgData * data)
{
  gb_widget_output_bool (data, Case, GTK_COMBO (widget)->case_sensitive);
  gb_widget_output_bool (data, EnableTab, SEARCHCOMBO (widget)->enable_tab);
  gb_widget_output_bool (data, AlwaysFill, SEARCHCOMBO (widget)->always_fill);
  gb_widget_output_bool (data, AutoExpand, SEARCHCOMBO (widget)->autoexpand);
}

/*
 * Sets the properties of the widget. This is used for both applying the
 * properties changed in the property editor, and also for loading.
 */
static void
gb_searchcombo_set_properties (GtkWidget * widget, GbWidgetSetArgData * data)
{
  gboolean case_sensitive, enable_tab, always_fill, autoexpand;

  case_sensitive = gb_widget_input_bool (data, Case);
  if (data->apply)
    gtk_combo_set_case_sensitive (GTK_COMBO (widget), case_sensitive);
  enable_tab = gb_widget_input_bool (data, EnableTab);
  if (data->apply)
    searchcombo_set_enable_tab (GTK_COMBO (widget), enable_tab);
  always_fill = gb_widget_input_bool (data, AlwaysFill);
  if (data->apply)
    searchcombo_set_always_fill (SEARCHCOMBO (widget), always_fill);
  autoexpand = gb_widget_input_bool (data, AutoExpand);
  if (data->apply)
    searchcombo_set_autoexpand (SEARCHCOMBO (widget), autoexpand);
}



/*
 * Adds menu items to a context menu which is just about to appear!
 * Add commands to aid in editing a SearchComboBox, with signals pointing to
 * other functions in this file.
 */
/*
   static void
   gb_searchcombo_create_popup_menu(GtkWidget *widget, GbWidgetCreateMenuData *data)
   {

   }
 */

/*
 * Initializes the GbWidget structure.
 * I've placed this at the end of the file so we don't have to include
 * declarations of all the functions.
 */

GbWidget *
gb_searchcombo_init ()
{
  /* Initialise the GTK type */
  searchcombo_get_type();

  /* Initialize the GbWidget structure */
  gb_widget_init_struct (&gbwidget);

  /* Fill in the pixmap struct & tooltip */
  gbwidget.pixmap_struct = searchcombo_xpm;
  gbwidget.tooltip = _("SearchComboBox");

  /* Fill in any functions that this GbWidget has */
  gbwidget.gb_widget_create_properties = gb_searchcombo_create_properties;
  gbwidget.gb_widget_get_properties = gb_searchcombo_get_properties;
  gbwidget.gb_widget_set_properties = gb_searchcombo_set_properties;
  gbwidget.gb_widget_new               = gb_searchcombo_new;
/*
  gbwidget.gb_widget_write_source = gb_searchcombo_write_source;
   gbwidget.gb_widget_create_popup_menu = gb_searchcombo_create_popup_menu;
 */

  return &gbwidget;
}
