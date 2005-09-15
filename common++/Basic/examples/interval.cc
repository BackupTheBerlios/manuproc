// $Id: interval.cc,v 1.2 2005/09/15 09:48:15 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#include <Misc/Date.h>
#include <Misc/Interval.h>
#include <Misc/TimeStamp.h>
#include <iostream>

std::ostream &operator<<(std::ostream &o,const ManuProC::Interval &i)
{  return o << i.str();
}

ManuProC::TimeStamp gerundet(ManuProC::TimeStamp v, ManuProC::TimeStamp::precision prec) 
{ ManuProC::TimeStamp v2(v); v2.Round(prec); return v2; }

ManuProC::Interval add(const ManuProC::Interval &a,const ManuProC::Interval &b)
{ ManuProC::Interval res(a);
  res+=b;
  return res;
}

int main()
{  
   ManuProC::TimeStamp t0("2005-9-13"),t1("2005-9-13 08:52"),
       t2("2005-9-13 08:52:17"), t3("2005-9-13 08:52:17.501234");
   std::cout << t0 << '\t' << t1 << '\t' << t2 << '\t' << t3 << '\n';
   std::cout << gerundet(t3,ManuProC::TimeStamp::milliseconds)
     << '\t' << gerundet(t3,ManuProC::TimeStamp::seconds)
     << '\t' << gerundet(t3,ManuProC::TimeStamp::minutes)
     << '\t' << gerundet(t3,ManuProC::TimeStamp::hours)
     << '\t' << gerundet(t3,ManuProC::TimeStamp::days) << '\n';
   ManuProC::TimeStamp t4("2005-9-13 11:29:29.499499"),
       t5("2005-9-13 12:30:30.500500");
   std::cout << gerundet(t4,ManuProC::TimeStamp::milliseconds)
     << '\t' << gerundet(t4,ManuProC::TimeStamp::seconds)
     << '\t' << gerundet(t4,ManuProC::TimeStamp::minutes)
     << '\t' << gerundet(t4,ManuProC::TimeStamp::hours)
     << '\t' << gerundet(t4,ManuProC::TimeStamp::days) << '\n';
   std::cout << gerundet(t5,ManuProC::TimeStamp::milliseconds)
     << '\t' << gerundet(t5,ManuProC::TimeStamp::seconds)
     << '\t' << gerundet(t5,ManuProC::TimeStamp::minutes)
     << '\t' << gerundet(t5,ManuProC::TimeStamp::hours)
     << '\t' << gerundet(t5,ManuProC::TimeStamp::days) << '\n';
     
#define IR(p) \
     ManuProC::Interval(gerundet(t4,ManuProC::TimeStamp::p), \
                        gerundet(t5,ManuProC::TimeStamp::p))
   std::cout << ManuProC::Interval(t4,t5) << '\t'
     << IR(milliseconds) << '\t' << IR(seconds) << '\t' 
     << IR(minutes) << '\t' << IR(hours) << '\t' << IR(days) << '\n';

#define IR2(p) \
     ManuProC::Interval(gerundet(t5,ManuProC::TimeStamp::p), \
                        gerundet(t4,ManuProC::TimeStamp::p))
   std::cout << ManuProC::Interval(t5,t4) << '\t'
     << IR2(milliseconds) << '\t' << IR2(seconds) << '\t' 
     << IR2(minutes) << '\t' << IR2(hours) << '\t' << IR2(days) << '\n';

   std::cout << add(ManuProC::Interval("1 day"),ManuProC::Interval("2 days"))
     << '\t' << add(ManuProC::Interval("12:00"),ManuProC::Interval("12:00"))
     << '\t' << add(ManuProC::Interval("0:30"),ManuProC::Interval("0:30"))
     << '\t' << add(ManuProC::Interval("0:00:30"),ManuProC::Interval("0:00:30"))
     << '\t' << add(ManuProC::Interval("0:00:00.700000"),ManuProC::Interval("0:00:00.3"))
     << '\n';
   std::cout << add(ManuProC::Interval("1 day"),ManuProC::Interval("-2 days"))
     << '\t' << add(ManuProC::Interval("2 days"),ManuProC::Interval("-00:00:01"))
     << '\t' << add(ManuProC::Interval(),ManuProC::Interval("-00:00:01"))
     << '\t' << add(ManuProC::Interval("12:00"),ManuProC::Interval("-00:00:00.1"))
     << '\t' << add(ManuProC::Interval("-0:30"),ManuProC::Interval("0:00:00.1"))
     << '\t' << add(ManuProC::Interval("1:00:00.3"),ManuProC::Interval("-1 day -1:00"))
     << '\n';
   return 0;
}
