// $Id: doublebutton.c,v 1.7 2004/04/29 14:24:40 christof Exp $
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

static void doublebutton_class_init     (doublebuttonClass   *klass);
static void doublebutton_init           (doublebutton        *button);
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
	sizeof (doublebuttonClass),
	NULL,		/* base_init */
	NULL,		/* base_finalize */
	(GClassInitFunc) doublebutton_class_init,
	NULL,		/* class_finalize */
	NULL,		/* class_data */
	sizeof (doublebutton),
	0,		/* n_preallocs */
	(GInstanceInitFunc) doublebutton_init,
      };

      button_type = g_type_register_static (GTK_TYPE_BUTTON, "doublebutton",
					    &button_info, 0);
    }

  return button_type;
}

void doublebutton_class_init(doublebuttonClass *klass)
{  GtkObjectClass *object_class = (GtkObjectClass*) klass;
   GtkWidgetClass *widget_class = (GtkWidgetClass*) klass;

   parent_class = g_type_class_peek_parent (klass);
   
   widget_class->button_press_event = doublebutton_button_press;
   widget_class->expose_event = doublebutton_expose;

//   klass->secondpressed = doublebutton_secondpressed

   doublebutton_signals[SECONDPRESSED]= g_signal_new ("secondpressed",
                  G_OBJECT_CLASS_TYPE (object_class),
                  G_SIGNAL_RUN_FIRST,
                  G_STRUCT_OFFSET (doublebuttonClass, secondpressed),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__INT,
                  G_TYPE_NONE, 1, G_TYPE_INT);
}

void doublebutton_init(doublebutton *button)
{
}

GtkWidget*     doublebutton_new               (void)
{  return g_object_new (TYPE_DOUBLEBUTTON, NULL);
}

void doublebutton_paint         (doublebutton    *button,
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

#if arrow_size/2 == 5
unsigned ycircle[arrow_size/2]= { 5, 5, 5, 4, 3 };
#elif arrow_size/2 == 4
unsigned ycircle[arrow_size/2]= { 4, 4, 3, 2 };
#else
#error unsupported arrow size
#endif


static void draw_circle_arrow(GtkWidget *widget, int x,int y)
{  unsigned int i;
   GdkWindow *window= widget->window;
   GdkGC *gc;
   
   gc = (widget)->style->text_gc[GTK_STATE_ACTIVE];
   for (i=0; i<arrow_size/2; ++i)
   {  gdk_draw_line(window, gc, x+arrow_size/2-i, y+arrow_size/2-ycircle[i],
   		x+arrow_size/2-i, y+arrow_size/2+ycircle[i]);
      gdk_draw_line(window, gc, x+arrow_size/2+i, y+arrow_size/2-ycircle[i],
   		x+arrow_size/2+i, y+arrow_size/2+ycircle[i]);
   }
   gc = (widget)->style->base_gc[GTK_STATE_ACTIVE];
   for (i=0; i<arrow_size/2; ++i)
   {  gdk_draw_line(window, gc, x+arrow_size/4+i+1, y+arrow_size/4+i/2+1,
   		x+arrow_size/4+i+1, y+3*arrow_size/4-i/2);
   }
}

static gboolean
doublebutton_expose (GtkWidget      *widget, GdkEventExpose *event)
{
  if (GTK_WIDGET_DRAWABLE (widget))
    {
      (* GTK_WIDGET_CLASS (parent_class)->expose_event) (widget, event);
      draw_circle_arrow(widget, 
      		widget->allocation.x+widget->allocation.width-arrow_size,
      		widget->allocation.y+widget->allocation.height-arrow_size);
    }
  
  return FALSE;
}

static gboolean
doublebutton_button_press (GtkWidget      *widget, GdkEventButton *event)
{
  if (event->type == GDK_BUTTON_PRESS)
    { if (event->x>widget->allocation.width-arrow_size &&
	    	event->y>widget->allocation.height-arrow_size)
	 g_signal_emit_by_name(widget, "secondpressed", event->button);
      else
         (* GTK_WIDGET_CLASS (parent_class)->button_press_event) (widget, event);
    }

  return TRUE;
}

