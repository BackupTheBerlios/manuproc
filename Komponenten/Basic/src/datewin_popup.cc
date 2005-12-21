// 

#include "config.h"
#include "datewin_popup.hh"
#include "datewin.h"
#include <gtk/gtkwindow.h>

void datewin_popup::datum_clicked()
{ 
}

void datewin_popup::woche_clicked()
{  
}

void datewin_popup::empty_clicked()
{  
}

void datewin_popup::on_day_selected()
{  
}

datewin_popup::datewin_popup(datewin *p)
  : parent(p)
{ GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(parent->gobj()));
  if (GTK_WIDGET_TOPLEVEL (toplevel))
    gtk_window_set_transient_for(gobj(),GTK_WINDOW(toplevel));
  gint height, width, x, y;
  parent->get_window()->get_origin(x,y);
  gint dummyx,dummyy,dummyd;
  parent->get_window()->get_geometry(dummyx,dummyy,width,height,dummyd);
  gint avail_height = parent->get_screen()->get_height();
  Gtk::Requisition mydimens = size_request();
  y += height;
  if (mydimens.width>width && x>=(width-mydimens.width))
    x-= width-mydimens.width;
  else if (width>mydimens.width) 
    mydimens.width=width;
  if (y + mydimens.height >= avail_height)
    y -= height + mydimens.height;
  move(x,y);
  set_size_request(mydimens.width, mydimens.height);
}

datewin_popup::~datewin_popup()
{ if (parent->popup==this) parent->popup=0;
}
