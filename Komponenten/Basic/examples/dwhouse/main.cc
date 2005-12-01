// $Id: main.cc,v 1.5 2005/12/01 18:36:17 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
// generated 1999/11/17 16:24:15 MET by christof@petig.petig.de
// using glademm V0.5.4a
//
// newer (non customized) versions of this file go to project1.cc_glade

// This file is for your program, I won't touch it again!

#include <gtkmm/main.h>
#include "window1.hh"
#include<Misc/exception.h>

int main(int argc, char **argv)
{

 ManuProC::PrintUncaughtExceptions();

   
   Gtk::Main m(argc, argv);
   manage(new class window1());
   m.run();

 return 0;
}

