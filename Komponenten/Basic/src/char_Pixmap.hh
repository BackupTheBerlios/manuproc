// $Id: char_Pixmap.hh,v 1.2 2002/12/11 16:50:47 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#ifndef MANUPROC_WIDGETS_MVC_CHAR_PIX_H
#define MANUPROC_WIDGETS_MVC_CHAR_PIX_H

#include <gtkmm/pixmap.h>
#include <BaseObjects/Model.h>

class char_Pixmap : public Gtk::Pixmap
{	typedef const char * const * T;
	SigC::Connection ch_con;//, my_ch_con;
	Model_ref<T> model;
	
   void init();
	void refresh(gpointer);
public:
	char_Pixmap(const Model_ref<T> &model);
};

#endif
