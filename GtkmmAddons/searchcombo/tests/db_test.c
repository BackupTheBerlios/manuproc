/* $Id: db_test.c,v 1.5 2004/04/29 14:24:40 christof Exp $ */

#include <gtk/gtk.h>
#include "doublebutton.h"
#include <stdio.h>

gint delete_event( GtkWidget *widget,
                   GdkEvent  *event,
		   gpointer   data )
{   gtk_main_quit();
    return(TRUE);
}

void clicked( GtkWidget *widget, gpointer data)
{  printf("clicked %p\n",data);
}

void secondclicked( GtkWidget *widget, int button, gpointer data)
{  printf("secondclicked %p\n",data);
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;
    GtkWidget *doublebutton1;
    GtkWidget *doublebutton2;
    GtkWidget *hbox;
    GtkWidget *label1,*label2;

    gtk_init(&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (delete_event), NULL);
    hbox = gtk_hbox_new (0,4);

    doublebutton1 = doublebutton_new ();
    doublebutton2 = doublebutton_new ();
    label1 = gtk_label_new("Button1");
    label2 = gtk_label_new("Button2");
    gtk_signal_connect (GTK_OBJECT (doublebutton1), "secondpressed",
			GTK_SIGNAL_FUNC (secondclicked), doublebutton1);
    gtk_signal_connect (GTK_OBJECT (doublebutton1), "clicked",
			GTK_SIGNAL_FUNC (clicked), doublebutton1);
    gtk_signal_connect (GTK_OBJECT (doublebutton2), "secondpressed",
			GTK_SIGNAL_FUNC (secondclicked), doublebutton2);
    gtk_signal_connect (GTK_OBJECT (doublebutton2), "clicked",
			GTK_SIGNAL_FUNC (clicked), doublebutton2);
    gtk_container_add (GTK_CONTAINER (doublebutton1), GTK_WIDGET(label1));
    gtk_container_add (GTK_CONTAINER (doublebutton2), GTK_WIDGET(label2));
    gtk_container_add (GTK_CONTAINER (hbox), GTK_WIDGET(doublebutton1));
    gtk_container_add (GTK_CONTAINER (hbox), GTK_WIDGET(doublebutton2));
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(hbox));
    gtk_widget_show (GTK_WIDGET(label1));
    gtk_widget_show (GTK_WIDGET(label2));
    gtk_widget_show (GTK_WIDGET(doublebutton1));
    gtk_widget_show (GTK_WIDGET(doublebutton2));
    gtk_widget_show (GTK_WIDGET(hbox));
    gtk_widget_show (GTK_WIDGET(window));

    gtk_main ();
    
    return(0);
}
