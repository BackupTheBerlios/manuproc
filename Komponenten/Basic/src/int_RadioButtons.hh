// $Id: int_RadioButtons.hh,v 1.3 2002/12/03 09:10:44 christof Exp $
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

#ifndef MANUPROC_WIDGETS_MVC_INT_RB_H
#define MANUPROC_WIDGETS_MVC_INT_RB_H

#include <gtk--/radiobutton.h>
#include <gtk--/box.h>
#include <BaseObjects/Model.h>
#include <vector>

class int_RadioButtons : public Gtk::VBox
{public:
	typedef int T;
	typedef std::pair<int, std::string> key_name_t;
	
private:	
	SigC::Connection ch_con, my_ch_con;
	Model_ref<T> model;
	std::vector<key_name_t> key_names;
	std::vector<Gtk::RadioButton> buttons;
	
	void refresh(gpointer);
	void on_toggled();
public:
	int_RadioButtons(const Model_ref<T> &model, 
		const std::vector<key_name_t> &names);
};

#endif
