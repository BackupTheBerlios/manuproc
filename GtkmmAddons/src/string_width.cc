// $Id: string_width.cc,v 1.4 2004/04/29 14:24:40 christof Exp $
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 1998  Adolf Petig GmbH. & Co. KG
    Developed by Christof Petig <christof.petig@wtal.de>
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <gtk/gtkwidget.h>

gint string_width(const GtkWidget *obj,const gchar *s)
{
#if 0
    GtkStyle *style=gtk_widget_get_style(GTK_WIDGET(obj));
    PangoFontDescription *font_desc=style->font_desc;
    PangoFontMetrics *metrics=pango_font_get_metrics(
    return pango_font_metrics_get_approximate_char_width(s);
#else // I don't know how to get the width
    return strlen(s)*8;
#endif    
}
