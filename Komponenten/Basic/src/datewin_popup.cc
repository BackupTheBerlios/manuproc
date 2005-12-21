// generated 2005/12/19 18:03:59 CET by christof@vesta.petig-baender.de
// using glademm V2.10.0.0
//
// newer (non customized) versions of this file go to datewin_popup.cc_new

// This file is for your program, I won't touch it again!

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
}
