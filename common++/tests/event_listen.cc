// $Id: event_listen.cc,v 1.3 2003/05/12 08:09:31 christof Exp $
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
#ifndef SIGC1_2
#include <sigc++/func_slot.h>
#endif

static void Event(const std::string &c,const std::string &k,const std::string &d)
{  std::cout << "Event[" << c << "," << k << "] " << d << '\n';
}

static void Event2(const std::string &k,const std::string &d)
{  std::cout << "Event[" << k << "] " << d << '\n';
}

int main(int argc, char **argv)
{  ManuProC::dbconnect();
   ManuProC::Event::connect();
   
   if (argc>1) ManuProC::Event::connect(argv[1],SigC::slot(&Event2));
//   signal_event(argv[1]).connect(&Event2);
   else ManuProC::Event::signal_event().connect(SigC::slot(&Event));
   
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
