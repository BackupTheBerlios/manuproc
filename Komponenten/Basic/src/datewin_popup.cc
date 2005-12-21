// 

#include "config.h"
#include "datewin_popup.hh"
#include "datewin.h"
#include <gtk/gtkwindow.h>
#include <Misc/Date.h>

void datewin_popup::datum_clicked()
{ parent->notebook->set_current_page(datewin::p_Datum);
  delete this;
}

void datewin_popup::woche_clicked()
{ parent->notebook->set_current_page(datewin::p_Woche);
  delete this;
}

void datewin_popup::empty_clicked()
{ parent->notebook->set_current_page(datewin::p_leer);
  parent->activate();
  delete this; 
}

void datewin_popup::on_day_selected()
{ if (block) return;
  guint y=0,m=0,day=0;
  calendar1->get_date(y,m,day);
  parent->set_value(ManuProC::Datum(day,m+1,y,parent->expandyear));
  parent->activate();
//  delete this;
}

datewin_popup::datewin_popup(datewin *p)
  : parent(p), block()
{ gint x, y;
  parent->get_window()->get_origin(x,y);
  x+=parent->get_allocation().get_x();
  y+=parent->get_allocation().get_y();
  gint height=parent->get_height();
  // we can not take parent's height because the button is not right aligned
  gint width= parent->togglebutton_menu->get_allocation().get_x()
      - parent->get_allocation().get_x()
      + parent->togglebutton_menu->get_allocation().get_width();
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
  ManuProC::Date d=parent->get_value();
  if (!!d)
  { calendar1->select_month(d.Monat()-1,d.Jahr());
    block=true;
    calendar1->select_day(d.Tag());
    block=false;
  } 
  show();
  GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(parent->gobj()));
  if (GTK_WIDGET_TOPLEVEL (toplevel))
    gtk_window_set_transient_for(gobj(),GTK_WINDOW(toplevel));
}

datewin_popup::~datewin_popup()
{ if (parent->popup==this) 
  { parent->popup=0;
    parent->togglebutton_menu->set_active(false);
  }
}
