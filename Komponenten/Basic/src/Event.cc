// $Id: Event.cc,v 1.2 2003/05/12 06:37:03 christof Exp $
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

#include <Misc/Event.h>
#include <glibmm/main.h>

static Glib::RefPtr<Glib::IOSource> iosrc;

static bool IOevent(Glib::IOCondition cond)
{  ManuProC::Event::look_for_notifications();
   return true;
}

void ManuProC::Event::connect_glib(bool ignore_old)
{  ManuProC::Event::connect(ignore_old);
   iosrc=Glib::IOSource::create(ManuProC::Event::filedesc(),Glib::IO_IN);
   iosrc->connect(&IOevent);
   iosrc->attach(); // Glib::MainContext::get_default());
}
