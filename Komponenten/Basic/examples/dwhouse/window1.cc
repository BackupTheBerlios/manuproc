// $Id: window1.cc,v 1.3 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// generated 2001/3/2 16:01:13 CET by jacek@mimi.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to window1.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "window1.hh"
#include <gtk--/main.h>

#include<typeinfo>

void window1::on_Beenden_activate()
{  Gtk::Main::instance()->quit();  
}


window1::window1()
{
 treebase->select_row.connect(SigC::slot(this,&window1::onRowSelect));
}


void window1::onRowSelect(int row, int col, GdkEvent *b)
{
 TCListRow_API *tclapi=(TCListRow_API*)(treebase->get_row_data(row));
 TCListRowData *selectedrow=(TCListRowData*)(*tclapi).get_user_data();

 std::cerr << typeid(*selectedrow).name() << '\n';
 std::cerr << selectedrow->Leaf() << '\n';
}

