#include <gtk/gtkexpander.h>
#include <gtk/gtkhpaned.h>
#include <gtk/gtkbox.h>

static void 
expander_callback(GObject    *object,
                   GParamSpec *param_spec,
                   gpointer    user_data)
{
  GtkExpander *expander = GTK_EXPANDER (object);
  GtkWidget* parent= gtk_widget_get_parent(GTK_WIDGET(object));
  
  if (!parent) return;
  if (GTK_IS_BOX(parent))
  { gboolean expand,fill;
    guint padding;
    GtkPackType pack_type;
    gtk_box_query_child_packing(GTK_BOX(parent),GTK_WIDGET(object),
          &expand, &fill, &padding, &pack_type);
    expand=gtk_expander_get_expanded (expander);
//    fill=gtk_expander_get_expanded (expander);
    gtk_box_set_child_packing(GTK_BOX(parent),GTK_WIDGET(object),
          expand, fill, padding, pack_type);
  }
  else if (GTK_IS_PANED(parent))
  { GtkPaned *paned=GTK_PANED(parent);
    gboolean is_first= gtk_paned_get_child1(paned)==GTK_WIDGET(expander);
    GtkRequisition req;
    guint minimum = 0;
    guint maximum = 0;
    guint position = 0;
    guint handle_size = 0;
    g_object_get(G_OBJECT(paned),"position",&position,NULL);
    gtk_widget_style_get(GTK_WIDGET (paned),"handle-size", &handle_size,NULL);
    /* the resize calculation seems to occur later than this callback ...
     * this bug looks like fixed in HEAD, but for 2.8.6 it is necessary
     * to get correct results */
    if (GTK_BIN(expander)->child)
    { gtk_widget_set_child_visible (GTK_BIN (expander)->child, gtk_expander_get_expanded (expander));
      gtk_widget_queue_resize (GTK_WIDGET (expander));
    }
    gtk_widget_size_request (GTK_WIDGET(expander), &req);
    if (GTK_IS_HPANED(parent)) 
    { minimum= req.width;
      maximum= GTK_WIDGET(paned)->allocation.width;
    }
    else 
    { minimum=req.height;
      maximum= GTK_WIDGET(paned)->allocation.height;
    }
    if (gtk_expander_get_expanded (expander))
    { if ((is_first && position<(maximum/2)) 
                || (!is_first && position>(maximum/2)))
        /* set the position at _least_ to the middle */
        gtk_paned_set_position(paned,maximum/2);
    }
    else
    { maximum -= handle_size + minimum;
      if (is_first && position>minimum)
        gtk_paned_set_position(paned,minimum);
      else if (!is_first && position<maximum)
        gtk_paned_set_position(paned,maximum);
    }
  }
}

gulong
gtk_expander_hook_size_to_parent(GtkExpander *expander)
{ /* adjust the initial state */
  expander_callback(G_OBJECT(expander),NULL,NULL);
  return g_signal_connect (expander, "notify::expanded",
        G_CALLBACK (expander_callback), NULL);
}

