// $Id: doublebutton.c,v 1.2 2003/04/07 16:58:43 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#include <doublebutton.h>
#include <stdio.h>

enum { SECONDPRESSED, LAST_SIGNAL };

static void doublebutton_class_init     (DoubleButtonClass   *klass);
static void doublebutton_init           (DoubleButton        *button);
static gboolean doublebutton_button_press (GtkWidget      *widget, GdkEventButton *event);
static gboolean doublebutton_expose (GtkWidget *widget, GdkEventExpose *event);

static GtkButtonClass *parent_class = NULL;
static guint doublebutton_signals[LAST_SIGNAL] = { 0 };

GType          doublebutton_get_type          (void)
{
  static GType button_type = 0;

  if (!button_type)
    {
      static const GTypeInfo button_info =
      {
	sizeof (DoubleButtonClass),
	NULL,		/* base_init */
	NULL,		/* base_finalize */
	(GClassInitFunc) doublebutton_class_init,
	NULL,		/* class_finalize */
	NULL,		/* class_data */
	sizeof (DoubleButton),
	0,		/* n_preallocs */
	(GInstanceInitFunc) doublebutton_init,
      };

      button_type = g_type_register_static (GTK_TYPE_BUTTON, "DoubleButton",
					    &button_info, 0);
    }

  return button_type;
}

void doublebutton_class_init(DoubleButtonClass *klass)
{  GtkObjectClass *object_class = (GtkObjectClass*) klass;
   GtkWidgetClass *widget_class = (GtkWidgetClass*) klass;

   parent_class = g_type_class_peek_parent (klass);
   
   widget_class->button_press_event = doublebutton_button_press;
   widget_class->expose_event = doublebutton_expose;

//   klass->secondpressed = doublebutton_secondpressed

   doublebutton_signals[SECONDPRESSED]= g_signal_new ("secondpressed",
                  G_OBJECT_CLASS_TYPE (object_class),
                  G_SIGNAL_RUN_FIRST,
                  G_STRUCT_OFFSET (DoubleButtonClass, secondpressed),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__INT,
                  G_TYPE_NONE, 1, G_TYPE_INT);
}

void doublebutton_init(DoubleButton *button)
{
}

GtkWidget*     doublebutton_new               (void)
{  return g_object_new (GTK_TYPE_DOUBLEBUTTON, NULL);
}

void doublebutton_paint         (DoubleButton    *button,
                                GdkRectangle *area,  
                                GtkStateType  state_type,
                                GtkShadowType shadow_type,
                                const gchar  *main_detail,
                                const gchar  *default_detail)
{
}

// we need a gc!

#define arrow_size 10
// y[x]=arrow_size/2*sqrt(1-(x/(arrow_size/2))^2);
// select cast (5*sqrt(1- (3/5.0)*(3/5.0)) as numeric(10,0));
unsigned ycircle[arrow_size/2]= { 5, 5, 5, 4, 3 };

//GdkColor black;
//gdk_colormap_alloc_color(colormap, color, false, true);

static void draw_circle_arrow(GdkWindow *window, GdkGC *gc, int x,int y)
{  unsigned int i;
   
   gdk_gc_set_foreground(gc, black);
   for (i=0; i<arrow_size/2; ++i)
   {  gdk_draw_line(window, gc, x+arrow_size/2-i, y+arrow_size/2-ycircle[i],
   		x+arrow_size/2-i, y+arrow_size/2+ycircle[i]);
   }
   gdk_gc_set_foreground(gc, white);
   for (i=0; i<arrow_size/2; ++i)
   {  gdk_draw_line(window, gc, x+arrow_size/4+i, y+arrow_size/4+i/2,
   		x+arrow_size/4+i, y+3*arrow_size/4-i/2);
   }
}

static gboolean
doublebutton_expose (GtkWidget      *widget, GdkEventExpose *event)
{
  if (GTK_WIDGET_DRAWABLE (widget))
    {
#if 0
      GtkButton *button = GTK_BUTTON (widget);
      
      _gtk_button_paint (button, &event->area,
			 GTK_WIDGET_STATE (widget),
			 button->depressed ? GTK_SHADOW_IN : GTK_SHADOW_OUT,
			 "button", "buttondefault");
#endif      
      (* GTK_WIDGET_CLASS (parent_class)->expose_event) (widget, event);
//      draw_circle_arrow(x,y);
    }
  
  return FALSE;
}

static gboolean
doublebutton_button_press (GtkWidget      *widget, GdkEventButton *event)
{
  GtkButton *button;

  if (event->type == GDK_BUTTON_PRESS)
    { printf("doublebutton_button_press(%d,%d)\n",(int)event->x,(int)event->y);
      (* GTK_WIDGET_CLASS (parent_class)->button_press_event) (widget, event);
    }

  return TRUE;
}

