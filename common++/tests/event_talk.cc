// $Id: event_talk.cc,v 1.2 2003/05/09 12:34:29 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
#include <Misc/dbconnect.h>

int main(int argc, char **argv)
{  ManuProC::dbconnect();
   std::string c="test",k,d;
   if (argc>1) c=argv[1];
   if (argc>2) k=argv[2];
   if (argc>3) d=argv[3];
   ManuProC::Event(c,k,d);
}
