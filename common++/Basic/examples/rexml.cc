// $Id: rexml.cc,v 1.1 2003/10/21 11:01:46 christof Exp $
/*  glade--: C++ frontend for glade (Gtk+ User Interface Builder)
 *  Copyright (C) 1998-2002  Christof Petig
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

// Simple TagStream testing program

#include <Misc/TagStream.h>
#include <iostream>

int main(int argc, char ** argv)
{  TagStream ts(std::cin);
   bool compact=false;
   if (argc>2) compact=true;
   if (argc>3) ts.debug(200);
   ts.setEncoding(argc>1?argv[1]:"UTF-8");
   ts.write(std::cout,compact);
   return 0;
}
