// $Id: Association.h,v 1.2 2003/09/18 16:16:05 christof Exp $
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

#ifndef MANUPROC_WIDGETS_ASSOCIATION_H
#define MANUPROC_WIDGETS_ASSOCIATION_H

#include <ModelWidgetConnection.h>
#include <string>

namespace Gtk
{ class SpinButton; class Entry; class CheckButton; class Label; 
  class CheckMenuItem; class ToggleButton;
}

namespace ManuProC
{   ModelWidgetConnection<int,Gtk::SpinButton> &Association(Gtk::SpinButton &);
    ModelWidgetConnection<std::string,Gtk::Entry> &Association(Gtk::Entry &);
    ModelWidgetConnection<bool,Gtk::ToggleButton> &Association(Gtk::ToggleButton &);
    ModelWidgetConnection<bool,Gtk::CheckMenuItem> &Association(Gtk::CheckMenuItem &);
    ModelWidgetConnection<std::string,Gtk::Label> &Association(Gtk::Label &);
}
#endif
