// $Id: tag_speed.cc,v 1.2 2004/06/03 08:51:19 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/TagStream.h>
#include <iostream>
#include <Misc/itos.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sstream>
#include <cassert>

std::ostream &operator<<(std::ostream &o, const timeval &x)
{  o << x.tv_sec << '.';
   o.width(6);
   o.fill('0');
   return o << x.tv_usec;
}

timeval operator-(const timeval &a, const timeval &b)
{  timeval res;
   res.tv_sec=a.tv_sec-b.tv_sec;
   if (a.tv_usec<b.tv_usec)
   {  res.tv_sec--;
      res.tv_usec=1000000+a.tv_usec-b.tv_usec;
   }
   else res.tv_usec=a.tv_usec-b.tv_usec;
   return res;
}

int main()
{  // populate
   struct rusage res;
   timeval last;
   if (getrusage(RUSAGE_SELF,&res)) perror("getrusage");
   std::cout << "start " << res.ru_utime << '\n';
   last=res.ru_utime;

   TagStream ts;
   Tag &s=ts.push_back(Tag("content"));
   for (unsigned i=0;i<1000;++i)
   {  Tag &e=s.push_back(Tag("SubTäg","Wért")); 
      for (unsigned j=0;j<50;++j) e.setAttr("A"+itos(j),itos(j));
      for (unsigned j=0;j<50;++j) e.push_back(Tag("ä"+itos(j),itos(j)));
   }
   
   if (getrusage(RUSAGE_SELF,&res)) perror("getrusage");
   std::cout << "population " << res.ru_utime-last << '\n';
   last=res.ru_utime;
   
   std::stringstream ss;
   ts.write(ss,true);

   if (getrusage(RUSAGE_SELF,&res)) perror("getrusage");
   std::cout << "write " << res.ru_utime-last << ' ' << ss.str().size() << "bytes\n";
   last=res.ru_utime;

   {  ss.seekp(0);
      TagStream ts2(ss);
      assert(ts2.find("content"));
   }
   if (getrusage(RUSAGE_SELF,&res)) perror("getrusage");
   std::cout << "read " << res.ru_utime-last << '\n';
   last=res.ru_utime;

   ts.setEncoding("ISO-8859-1");
   std::stringstream ss2;
   ts.write(ss2,true);

   if (getrusage(RUSAGE_SELF,&res)) perror("getrusage");
   std::cout << "write ISO " << res.ru_utime-last << ' ' << ss2.str().size() << "bytes\n";
   last=res.ru_utime;

   {  ss2.seekp(0);
      TagStream ts2(ss2);
      if (!ts2.find("content"))
      {  std::cerr << ss2.str().substr(0,40) << '\n';
         ts2.debug();
      }
   }
   if (getrusage(RUSAGE_SELF,&res)) perror("getrusage");
   std::cout << "read ISO " << res.ru_utime-last << '\n';
   return 0;
}
