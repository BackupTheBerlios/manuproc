// $Id: event_listen.cc,v 1.1 2003/05/09 11:08:06 christof Exp $
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
#include <iostream>
#include <sys/select.h>

static void Event(const std::string &c,const std::string &k,const std::string &d)
{  std::cout << "Event[" << c << "," << k << "] " << d << '\n';
}

int main()
{  ManuProC::dbconnect();
   ManuProC::Event::connect();
   
   while (ManuProC::Event::filedesc()>0)
   {  fd_set rfds;
      struct timeval tv;
      int retval;
      FD_ZERO(&rfds);
      FD_SET(ManuProC::Event::filedesc(), &rfds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
      retval = select(ManuProC::Event::filedesc()+1, &rfds, NULL, NULL, &tv);
      if (retval) ManuProC::Event::look_for_notifications();
   }
   ManuProC::dbdisconnect();
   return 0;
}
