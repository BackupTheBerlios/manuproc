// $Id: marc.cc,v 1.3 2004/11/16 11:40:06 christof Exp $
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

// generated 2002/5/16 16:24:34 CEST by christof@puck.petig-baender.de
// using glademm V0.6.4b_cvs
//
// newer (non customized) versions of this file go to marc.cc_new

// This file is for your program, I won't touch it again!

#include <gtkmm/main.h>

#include "window1.hh"
#include "window2.hh"

int main(int argc, char **argv)
{  
   
   Gtk::Main m(&argc, &argv);
manage(new class window1());
manage(new class window2());
   m.run();
   return 0;
}
