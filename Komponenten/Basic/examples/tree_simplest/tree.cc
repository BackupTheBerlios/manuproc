// $Id: tree.cc,v 1.5 2002/07/05 12:36:56 christof Exp $
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

// generated 2001/6/15 9:52:50 CEST by christof@puck.(none)
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to marc.cc_new

// This file is for your program, I won't touch it again!

#include <gtk--/main.h>

#include "simplest.hh"
#include "with_class.hh"
#ifdef MANUPROC_WITH_DATABASE
#  include <Misc/dbconnect.h>
#endif

int main(int argc, char **argv)
{   
#ifdef MANUPROC_WITH_DATABASE
   ManuProC::dbconnect();
#endif
   
   Gtk::Main m(&argc, &argv);
//manage(new class simplest());
manage(new class with_class());
   m.run();
   return 0;
}
