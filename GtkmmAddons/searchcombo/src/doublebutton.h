// $Id: doublebutton.h,v 1.5 2004/04/29 14:24:40 christof Exp $
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

#ifndef MPW_DOUBLEBUTTON_C_HH
#define MPW_DOUBLEBUTTON_C_HH

#include <gtk/gtkbutton.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TYPE_DOUBLEBUTTON                 (doublebutton_get_type ())
#define DOUBLEBUTTON(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DOUBLEBUTTON, doublebutton))
#define DOUBLEBUTTON_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DOUBLEBUTTON, doublebuttonClass))
#define IS_DOUBLEBUTTON(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DOUBLEBUTTON))
#define IS_DOUBLEBUTTON_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DOUBLEBUTTON))
#define DOUBLEBUTTON_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DOUBLEBUTTON, doublebuttonClass))

typedef struct _doublebutton       doublebutton;
typedef struct _doublebuttonClass  doublebuttonClass;

struct _doublebutton
{
  GtkButton button;
};

struct _doublebuttonClass
{
  GtkButtonClass        parent_class;
  
  void (* secondpressed)  (doublebutton *button, int mbutton);
};

GType          doublebutton_get_type          (void) G_GNUC_CONST;
GtkWidget*     doublebutton_new               (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
