// $Id: bool_properties.hh,v 1.1 2003/02/26 13:32:22 christof Exp $
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

#ifndef MANUPROC_WIDGETS_MVC_BOOL_PROPERTIES_H
#define MANUPROC_WIDGETS_MVC_BOOL_PROPERTIES_H

#include <BaseObjects/Model.h>
//#include <gtkmm/widget.h>
#include <sigc++/connection.h>

namespace Gtk
{  class Widget;

   SigC::Connection AssociateVisibility(Widget *w,const Model_ref<bool> &model, bool invert=false);
   SigC::Connection AssociateSensitivity(Widget *w,const Model_ref<bool> &model, bool invert=false);
}

#endif
