
/*  TCList gbwidget (for glade)
 *  Copyright (C) 1998  Christof Petig
 *
 *  this file is based upon gbclist.c by Damon Chaplin
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

#include <string.h>

#include <gtk/gtk.h>
#include "../gb.h"

/* Include the 21x21 icon pixmap for this widget, to be used in the palette */
#include "../graphics/tclist.xpm"

/************************************
 * tclist dummy widget
 ***********************************/
 
typedef struct _GtkCList TCList;
typedef struct _GtkCListClass TCListClass;

static GtkCListClass *parent_class = NULL;

static void
tclist_size_allocate (GtkWidget     *widget,
			 GtkAllocation *allocation)
{
  GTK_WIDGET_CLASS (parent_class)->size_allocate (widget, allocation);
}

static void
tclist_destroy (GtkObject * combo)
{ if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (*GTK_OBJECT_CLASS (parent_class)->destroy) (combo);
}

static void
tclist_class_init (TCListClass * klass)
{
  GtkObjectClass *oclass;
  GtkWidgetClass *widget_class;

  parent_class = gtk_type_class (gtk_clist_get_type ());
  
  oclass = (GtkObjectClass *) klass;
  oclass->destroy = tclist_destroy;
  
  widget_class = (GtkWidgetClass *) klass;
  widget_class->size_allocate = tclist_size_allocate;
} 

static guint
tclist_get_type ()
{
  static guint tclist_type = 0;
  
  if (!tclist_type)
  {   static const GtkTypeInfo tclist_info =
      { "TCList",
	sizeof (TCList),
	sizeof (TCListClass),
	(GtkClassInitFunc) tclist_class_init,
	(GtkObjectInitFunc) NULL,
        (GtkArgSetFunc) NULL,
	(GtkArgSetFunc) NULL
      };
    tclist_type = gtk_type_unique(gtk_clist_get_type(),&tclist_info);
  }
  return tclist_type;
}

static GtkWidget *
tclist_new (int cols)
{ GtkWidget *widget;

  widget = gtk_type_new (tclist_get_type ());
  gtk_clist_construct (GTK_CLIST (widget), cols, NULL);
  return widget;
}

/************************************/

/*
 * This is the GbWidget struct for this widget (see ../gbwidget.h).
 * It is initialized in the init() function at the end of this file
 */
static GbWidget gbwidget;

/* These are the special children of the widget. */
static gchar *CListTitle = "CList:title";

static gchar *Mode = "GtkCList::selection_mode";
static gchar *Titles = "GtkCList::show_titles";
static gchar *Shadow = "GtkCList::shadow_type";

#ifndef GTK_HAVE_FEATURES_1_1_4
static gchar *HPolicy = "GtkCList::hscrollbar_policy";
static gchar *VPolicy = "GtkCList::vscrollbar_policy";
#endif

/* This is only used for loading & saving - it isn't displayed in the property
   editor. */
static gchar *Cols = "GtkCList::columns";
static gchar *ColumnWidths = "GtkCList::column_widths";

static gchar *GbModeChoices[] =
{"Single", "Browse", "Multiple",
 "Extended", NULL};
static gint GbModeValues[] =
{
  GTK_SELECTION_SINGLE,
  GTK_SELECTION_BROWSE,
  GTK_SELECTION_MULTIPLE,
  GTK_SELECTION_EXTENDED
};
static gchar *GbModeSymbols[] =
{
  "GTK_SELECTION_SINGLE",
  "GTK_SELECTION_BROWSE",
  "GTK_SELECTION_MULTIPLE",
  "GTK_SELECTION_EXTENDED"
};

static gchar *GbShadowChoices[] =
{"None", "In", "Out",
 "Etched In", "Etched Out", NULL};
static gint GbShadowValues[] =
{
  GTK_SHADOW_NONE,
  GTK_SHADOW_IN,
  GTK_SHADOW_OUT,
  GTK_SHADOW_ETCHED_IN,
  GTK_SHADOW_ETCHED_OUT
};
static gchar *GbShadowSymbols[] =
{
  "GTK_SHADOW_NONE",
  "GTK_SHADOW_IN",
  "GTK_SHADOW_OUT",
  "GTK_SHADOW_ETCHED_IN",
  "GTK_SHADOW_ETCHED_OUT"
};

#ifndef GTK_HAVE_FEATURES_1_1_4
static gchar *GbPolicyChoices[] =
{"Always", "Automatic", NULL};
static gint GbPolicyValues[] =
{
  GTK_POLICY_ALWAYS,
  GTK_POLICY_AUTOMATIC
};
static gchar *GbPolicySymbols[] =
{
  "GTK_POLICY_ALWAYS",
  "GTK_POLICY_AUTOMATIC"
};
#endif

static void show_tclist_dialog (GbWidgetNewData * data);
static void on_tclist_dialog_ok (GtkWidget * widget,
				GbWidgetNewData * data);
static void on_tclist_dialog_destroy (GtkWidget * widget,
				     GbWidgetNewData * data);
static GtkWidget *new_label (GtkWidget * parent);
static GtkWidget *new_unnamed_label (GtkWidget * parent);

#ifndef GTK_HAVE_FEATURES_1_1_4
static void hadjustment_value_changed (GtkAdjustment * adjustment,
				       GtkWidget *clist);
static void vadjustment_value_changed (GtkAdjustment * adjustment,
				       GtkWidget *clist);
#endif

/******
 * NOTE: To use these functions you need to uncomment them AND add a pointer
 * to the function in the GbWidget struct at the end of this file.
 ******/

/*
 * Creates a new GtkWidget of class GtkCList, performing any specialized
 * initialization needed for the widget to work correctly in this environment.
 * If a dialog box is used to initialize the widget, return NULL from this
 * function, and call data->callback with your new widget when it is done.
 * If the widget needs a special destroy handler, add a signal here.
 */
static GtkWidget *
gb_tclist_new (GbWidgetNewData * data)
{
  GtkWidget *new_widget;
#ifndef GTK_HAVE_FEATURES_1_1_4
  GtkAdjustment *adjustment;
#endif
  gint cols = 0, i;

  if (data->action == GB_LOADING)
    {
      cols = load_int (data->loading_data, Cols);
      if (cols == 0)
	cols = 1;
      new_widget = tclist_new (cols);

      /* GtkCList has problems if the title buttons aren't created. */
      for (i = 0; i < cols; i++)
	{
	  gtk_clist_set_column_widget (GTK_CLIST (new_widget), i,
                                       new_unnamed_label (new_widget));
	  gtk_clist_set_column_width (GTK_CLIST (new_widget), i, 80);
          editor_add_mouse_signals_to_existing (GTK_CLIST (new_widget)->column[i].button);
	}

      /* Connect signals for redrawing. */
      /* The CList doesn't have scrollbars in 1.1.4+. We'll have to do this
	 another way at some point. */
#ifndef GTK_HAVE_FEATURES_1_1_4
      adjustment = gtk_range_get_adjustment (GTK_RANGE (GTK_CLIST (new_widget)->hscrollbar));
      gtk_signal_connect_after (GTK_OBJECT (adjustment), "value_changed",
				(GtkSignalFunc) hadjustment_value_changed,
				new_widget);
      adjustment = gtk_range_get_adjustment (GTK_RANGE (GTK_CLIST (new_widget)->vscrollbar));
      gtk_signal_connect_after (GTK_OBJECT (adjustment), "value_changed",
				(GtkSignalFunc) vadjustment_value_changed,
				new_widget);
#endif
      return new_widget;
    }
  else
    {
      show_tclist_dialog (data);
      return NULL;
    }
}


static void
gb_tclist_add_child (GtkWidget *widget, GtkWidget *child, GbWidgetSetArgData *data)
{
  gchar *child_name = load_get_value (data, "child_name");

  /* FIXME: This should use CListTitle from gbclist.c */
  if (child_name && (!strcmp (child_name, "CList:title")
                     || !strcmp (child_name, "CTree:title")))
    {
      /* We store the last column title read in 'last_child' */
      gint col = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (widget),
                                                       "last_child"));

      gtk_clist_set_column_widget (GTK_CLIST (widget), col, child);
      gtk_object_set_data (GTK_OBJECT (widget), "last_child",
                           GINT_TO_POINTER (col + 1));

      /* We need to add signals to the clist button, just in case the
         title widget has no window and so doesn't get signals itself.
         Since Clist always creates 1 button initially, the signals would
         be added to this button in gb_widget_new, so we could skip it,
         but it doesn't hurt. */
      editor_add_mouse_signals_to_existing (GTK_CLIST (widget)->column[col].button);
    }
  else
    {
      gtk_container_add (GTK_CONTAINER (widget), child);
    }
}


static void
show_tclist_dialog (GbWidgetNewData * data)
{
  GtkWidget *dialog, *vbox, *hbox, *label, *spinbutton;
  GtkObject *adjustment;

  dialog = glade_util_create_dialog (_("New tree columned list"), data->parent,
				     on_tclist_dialog_ok, data, &vbox);
  gtk_signal_connect (GTK_OBJECT (dialog), "destroy",
		      GTK_SIGNAL_FUNC (on_tclist_dialog_destroy), data);

  hbox = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 5);
  gtk_container_border_width (GTK_CONTAINER (hbox), 10);
  gtk_widget_show (hbox);

  label = gtk_label_new (_("Number of columns:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 5);
  gtk_widget_show (label);

  adjustment = gtk_adjustment_new (3, 1, 100, 1, 10, 10);
  spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (adjustment), 1, 0);
  /* save pointer to spinbutton so we can find it easily in the OK handler */
  gtk_object_set_data (GTK_OBJECT (dialog), "cols", spinbutton);
  gtk_box_pack_start (GTK_BOX (hbox), spinbutton, TRUE, TRUE, 5);
  gtk_widget_set_usize (spinbutton, 50, -1);
  gtk_widget_grab_focus (spinbutton);
  gtk_widget_show (spinbutton);

  gtk_widget_show (dialog);
  gtk_grab_add (dialog);
}


static void
on_tclist_dialog_ok (GtkWidget * widget, GbWidgetNewData * data)
{
  GtkWidget *new_widget, *spinbutton, *dialog, *label;
#ifndef GTK_HAVE_FEATURES_1_1_4
  GtkAdjustment *adjustment;
#endif
  gint cols, i;

  dialog = gtk_widget_get_toplevel (widget);

  /* Only call callback if placeholder/fixed widget is still there */
  if (gb_widget_can_finish_new (data))
    {
      spinbutton = gtk_object_get_data (GTK_OBJECT (dialog), "cols");
      g_return_if_fail (spinbutton != NULL);
      cols = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spinbutton));

      new_widget = tclist_new (cols);
      gtk_clist_column_titles_show (GTK_CLIST (new_widget));
      for (i = 0; i < cols; i++)
	{
	  label = new_label (new_widget);
	  gtk_clist_set_column_widget (GTK_CLIST (new_widget), i,
				       label);
	  gtk_clist_set_column_width (GTK_CLIST (new_widget), i, 80);
	}

      /* Connect signals for redrawing. */
      /* The CList doesn't have scrollbars in 1.1.4+. We'll have to do this
	 another way at some point. */
#ifndef GTK_HAVE_FEATURES_1_1_4
      adjustment = gtk_range_get_adjustment (GTK_RANGE (GTK_CLIST (new_widget)->hscrollbar));
      gtk_signal_connect_after (GTK_OBJECT (adjustment), "value_changed",
				(GtkSignalFunc) hadjustment_value_changed,
				new_widget);
      adjustment = gtk_range_get_adjustment (GTK_RANGE (GTK_CLIST (new_widget)->vscrollbar));
      gtk_signal_connect_after (GTK_OBJECT (adjustment), "value_changed",
				(GtkSignalFunc) vadjustment_value_changed,
				new_widget);
#endif

      gb_widget_initialize (new_widget, data);
      (*data->callback) (new_widget, data);
    }
  gtk_widget_destroy (dialog);
}


static void
on_tclist_dialog_destroy (GtkWidget * widget, GbWidgetNewData * data)
{
  gb_widget_free_new_data (data);
  gtk_grab_remove (widget);
}


static GtkWidget *
new_label (GtkWidget * parent)
{
  GtkWidget *label;

  label = gb_widget_new ("GtkLabel", parent);
  g_return_val_if_fail (label != NULL, NULL);
  gb_widget_set_child_name (label,CListTitle);
  return label;
}


static GtkWidget *
new_unnamed_label (GtkWidget * parent)
{
  GtkWidget *label;

  label = gb_widget_new_full ("GtkLabel", FALSE, parent, NULL, 0, 0, NULL,
			      GB_CREATING, NULL);
  g_return_val_if_fail (label != NULL, NULL);
  gb_widget_set_child_name (label, CListTitle);
  return label;
}


/* These two functions redraw the clist when the scrollbars are used, so
   that the selection rectangle doesn't get messed up - the clist uses
   gdk_window_copy_area() which will copy the selection handles which we
   do not want to happen. */
#ifndef GTK_HAVE_FEATURES_1_1_4
static void
hadjustment_value_changed (GtkAdjustment * adjustment,
			   GtkWidget *clist)
{
  gint x, y, w, h;

  x = clist->allocation.x;
  y = clist->allocation.y;
  w = clist->allocation.width;
  h = clist->allocation.height;

  editor_refresh_widget_area (clist, x, y, w, GB_CORNER_HEIGHT);
  editor_refresh_widget_area (clist, x, y + h - GB_CORNER_HEIGHT,
			      w, GB_CORNER_HEIGHT);
  gtk_widget_draw (clist, NULL);
  /* FIXME: GTK bug workaround to draw CList children. */
#ifndef GTK_HAVE_FEATURES_1_1_4
  gtk_widget_draw_children (clist);
#endif
}

static void
vadjustment_value_changed (GtkAdjustment * adjustment,
			   GtkWidget *clist)
{
  gint x, y, w, h;

  x = clist->allocation.x;
  y = clist->allocation.y;
  w = clist->allocation.width;
  h = clist->allocation.height;

  editor_refresh_widget_area (clist, x, y, GB_CORNER_WIDTH, h);
  editor_refresh_widget_area (clist, x + w - GB_CORNER_WIDTH, y,
			      GB_CORNER_WIDTH, h);
  gtk_widget_draw (clist, NULL);
  /* FIXME: GTK bug workaround to draw CList children. */
#ifndef GTK_HAVE_FEATURES_1_1_4
  gtk_widget_draw_children (clist);
#endif
}
#endif


/*
 * Creates the components needed to edit the extra properties of this widget.
 */
static void
gb_tclist_create_properties (GtkWidget * widget, GtkWidget *child, GbWidgetCreateArgData * data)
{
  property_add_choice (Mode, _("Select Mode:"),
		       _("The selection mode of the columned list"),
		       GbModeChoices);
  property_add_bool (Titles, _("Show Titles:"),
		     _("If the column titles are shown"));
  property_add_choice (Shadow, _("Shadow:"),
		       _("The type of shadow of the columned list's border"),
		       GbShadowChoices);
#ifndef GTK_HAVE_FEATURES_1_1_4
  property_add_choice (HPolicy, _("H Policy:"),
		       _("When the horizontal scrollbar will be shown"),
		       GbPolicyChoices);
  property_add_choice (VPolicy, _("V Policy:"),
		       _("When the vertical scrollbar will be shown"),
		       GbPolicyChoices);
#endif
}



/*
 * Gets the properties of the widget. This is used for both displaying the
 * properties in the property editor, and also for saving the properties.
 */
static void
gb_tclist_get_properties (GtkWidget * widget, GtkWidget *child, GbWidgetGetArgData * data)
{
  gchar buffer[1024];
  gchar *pos;
  gboolean buffer_overrun = FALSE;
  gint i;

  if (data->action == GB_SAVING)
    {
      gb_widget_output_int (data, Cols, GTK_CLIST (widget)->columns);

      pos = buffer;
      for (i = 0; i < GTK_CLIST (widget)->columns; i++)
	{
	  if (i == 0)
	    sprintf (pos, "%i", GTK_CLIST (widget)->column[i].width);
	  else
	    sprintf (pos, ",%i", GTK_CLIST (widget)->column[i].width);
	  pos += strlen (pos);

	  /* Extra check to make sure we don't overrun the buffer. */
	  if (pos - buffer > 1000)
	    {
	      g_warning ("Buffer overflow");
	      buffer_overrun = TRUE;
	      break;
	    }
	}
      if (!buffer_overrun)
	gb_widget_output_string (data, ColumnWidths, buffer);
    }

  for (i = 0; i < sizeof (GbModeValues) / sizeof (GbModeValues[0]); i++)
    {
      if (GbModeValues[i] == GTK_CLIST (widget)->selection_mode)
	gb_widget_output_choice (data, Mode, i, GbModeSymbols[i]);
    }

  gb_widget_output_bool (data, Titles, GTK_CLIST_SHOW_TITLES (widget));

  for (i = 0; i < sizeof (GbShadowValues) / sizeof (GbShadowValues[0]); i++)
    {
      if (GbShadowValues[i] == GTK_CLIST (widget)->shadow_type)
	gb_widget_output_choice (data, Shadow, i, GbShadowSymbols[i]);
    }

#ifndef GTK_HAVE_FEATURES_1_1_4
  for (i = 0; i < sizeof (GbPolicyValues) / sizeof (GbPolicyValues[0]); i++)
    {
      if (GbPolicyValues[i] == GTK_CLIST (widget)->hscrollbar_policy)
	gb_widget_output_choice (data, HPolicy, i, GbPolicySymbols[i]);
    }
  for (i = 0; i < sizeof (GbPolicyValues) / sizeof (GbPolicyValues[0]); i++)
    {
      if (GbPolicyValues[i] == GTK_CLIST (widget)->vscrollbar_policy)
	gb_widget_output_choice (data, VPolicy, i, GbPolicySymbols[i]);
    }
#endif
}



/*
 * Sets the properties of the widget. This is used for both applying the
 * properties changed in the property editor, and also for loading.
 */
static void
gb_tclist_set_properties (GtkWidget * widget, GtkWidget *child, GbWidgetSetArgData * data)
{
  gchar *widths, *pos, *mode;
  gchar *shadow;
  gboolean titles;
  gint col, w, i;
#ifndef GTK_HAVE_FEATURES_1_1_4
  gboolean myApply;
  gchar *hpolicy, *vpolicy;
  GtkPolicyType hpolicy_value = GTK_POLICY_AUTOMATIC;
  GtkPolicyType vpolicy_value = GTK_POLICY_AUTOMATIC;
#endif

  if (data->action == GB_LOADING)
    {
      widths = gb_widget_input_string (data, ColumnWidths);
      if (data->apply)
	{
	  pos = widths;
	  for (col = 0; col < GTK_CLIST (widget)->columns; col++)
	    {
	      w = atoi (pos);
	      gtk_clist_set_column_width (GTK_CLIST (widget), col, w);
	      pos = strchr (pos, ',');
	      if (!pos)
		break;
	      pos++;
	    }
	}
    }

  mode = gb_widget_input_choice (data, Mode);
  if (data->apply)
    {
      for (i = 0; i < sizeof (GbModeValues) / sizeof (GbModeValues[0]); i++)
	{
	  if (!strcmp (mode, GbModeChoices[i])
	      || !strcmp (mode, GbModeSymbols[i]))
	    {
	      gtk_clist_set_selection_mode (GTK_CLIST (widget), GbModeValues[i]);
	      break;
	    }
	}
    }

  titles = gb_widget_input_bool (data, Titles);
  if (data->apply)
    {
      if (titles)
	gtk_clist_column_titles_show (GTK_CLIST (widget));
      else
	gtk_clist_column_titles_hide (GTK_CLIST (widget));
    }

  shadow = gb_widget_input_choice (data, Shadow);
  if (data->apply)
    {
      for (i = 0; i < sizeof (GbShadowValues) / sizeof (GbShadowValues[0]); i
	   ++)
	{
	  if (!strcmp (shadow, GbShadowChoices[i])
	      || !strcmp (shadow, GbShadowSymbols[i]))
	    {
#ifdef GTK_HAVE_FEATURES_1_1_4
	      gtk_clist_set_shadow_type (GTK_CLIST (widget),
					 GbShadowValues[i]);
#else
	      gtk_clist_set_border (GTK_CLIST (widget), GbShadowValues[i]);
#endif
	      break;
	    }
	}
    }

#ifndef GTK_HAVE_FEATURES_1_1_4
  hpolicy = gb_widget_input_choice (data, HPolicy);
  myApply = data->apply;
  vpolicy = gb_widget_input_choice (data, VPolicy);
  if (data->apply || myApply)
    {
      for (i = 0; i < sizeof (GbPolicyValues) / sizeof (GbPolicyValues[0]); i
	   ++)
	{
	  if (!strcmp (hpolicy, GbPolicyChoices[i])
	      || !strcmp (hpolicy, GbPolicySymbols[i]))
	    hpolicy_value = GbPolicyValues[i];
	  if (!strcmp (vpolicy, GbPolicyChoices[i])
	      || !strcmp (vpolicy, GbPolicySymbols[i]))
	    vpolicy_value = GbPolicyValues[i];
	}
      gtk_clist_set_policy (GTK_CLIST (widget), vpolicy_value, hpolicy_value);
    }
#endif
}



/*
 * Adds menu items to a context menu which is just about to appear!
 * Add commands to aid in editing a GtkCList, with signals pointing to
 * other functions in this file.
 */
/*
   static void
   gb_tclist_create_popup_menu(GtkWidget *widget, GbWidgetCreateMenuData *data)
   {

   }
 */



/*
 * Writes the source code needed to create this widget.
 * You have to output everything necessary to create the widget here, though
 * there are some convenience functions to help.
 */
static void
gb_tclist_write_source (GtkWidget * widget, GbWidgetWriteSourceData * data)
{
  gint col, i;
#ifndef GTK_HAVE_FEATURES_1_1_4
  gint hpolicy = 0, vpolicy = 0;
#endif

  if (data->create_widget)
    {
      source_add (data, "  %s = tclist_new (%i);\n", data->wname,
		  GTK_CLIST (widget)->columns);
    }

  /* We reset the last_child index, so as the title widgets are written out
     they will start at column 0. */
  gtk_object_set_data (GTK_OBJECT (widget), "last_child", GINT_TO_POINTER (-1));

  gb_widget_write_standard_source (widget, data);

  for (col = 0; col < GTK_CLIST (widget)->columns; col++)
    {
      source_add (data,
		  "  gtk_clist_set_column_width (GTK_CLIST (%s), %i, %i);\n",
		  data->wname, col, GTK_CLIST (widget)->column[col].width);
    }

  if (GTK_CLIST (widget)->selection_mode != GTK_SELECTION_SINGLE)
    {
      for (i = 0; i < sizeof (GbModeValues) / sizeof (GbModeValues[0]); i++)
	{
	  if (GbModeValues[i] == GTK_CLIST (widget)->selection_mode)
	    source_add (data,
		   "  gtk_clist_set_selection_mode (GTK_CLIST (%s), %s);\n",
			data->wname, GbModeSymbols[i]);
	}
    }

  if (GTK_CLIST_SHOW_TITLES (widget))
    source_add (data, "  gtk_clist_column_titles_show (GTK_CLIST (%s));\n",
		data->wname);
  else
    source_add (data, "  gtk_clist_column_titles_hide (GTK_CLIST (%s));\n",
		data->wname);

  if (GTK_CLIST (widget)->shadow_type != GTK_SHADOW_IN)
    {
      for (i = 0; i < sizeof (GbShadowValues) / sizeof (GbShadowValues[0]); i
	   ++)
	{
	  if (GbShadowValues[i] == GTK_CLIST (widget)->shadow_type)
#ifdef GTK_HAVE_FEATURES_1_1_4
	    source_add (data, "  gtk_clist_set_shadow_type (GTK_CLIST (%s), %s);\n",
			data->wname, GbShadowSymbols[i]);
#else
	    source_add (data, "  gtk_clist_set_border (GTK_CLIST (%s), %s);\n",
			data->wname, GbShadowSymbols[i]);
#endif
	}
    }

#ifndef GTK_HAVE_FEATURES_1_1_4
  if (GTK_CLIST (widget)->hscrollbar_policy != GTK_POLICY_ALWAYS
      || GTK_CLIST (widget)->vscrollbar_policy != GTK_POLICY_ALWAYS)
    {
      for (i = 0; i < sizeof (GbPolicyValues) / sizeof (GbPolicyValues[0]); i
	   ++)
	{
	  if (GbPolicyValues[i] == GTK_CLIST (widget)->hscrollbar_policy)
	    hpolicy = i;
	  if (GbPolicyValues[i] == GTK_CLIST (widget)->vscrollbar_policy)
	    vpolicy = i;
	}

      source_add (data, "  gtk_clist_set_policy (GTK_CLIST (%s), %s, %s);\n",
		  data->wname, GbPolicySymbols[vpolicy],
		  GbPolicySymbols[hpolicy]);
    }
#endif
}



/*
 * Initializes the GbWidget structure.
 * I've placed this at the end of the file so we don't have to include
 * declarations of all the functions.
 */
GbWidget *
gb_tclist_init ()
{
  /* Initialise the GTK type */
  tclist_get_type ();

  /* Initialize the GbWidget structure */
  gb_widget_init_struct (&gbwidget);

  /* Fill in the pixmap struct & tooltip */
  gbwidget.pixmap_struct = tclist_xpm;
  gbwidget.tooltip = _("Columned Tree (C++)");

  /* Fill in any functions that this GbWidget has */
  gbwidget.gb_widget_new = gb_tclist_new;
  gbwidget.gb_widget_create_properties = gb_tclist_create_properties;
  gbwidget.gb_widget_get_properties = gb_tclist_get_properties;
  gbwidget.gb_widget_set_properties = gb_tclist_set_properties;
  gbwidget.gb_widget_write_source = gb_tclist_write_source;
/*
   gbwidget.gb_widget_create_popup_menu = gb_tclist_create_popup_menu;
 */

  return &gbwidget;
}
