// $Id: trace.cc,v 1.3 2003/03/10 10:17:50 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

// to get ::NV
#define MANUPROC_NV
#include <Misc/Trace.h>

static const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();

void C()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,1,2,3,4,5,6,7,8,9,10 );
}

void B(int i,const std::string &t)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("i",i),NV("t",t));
   C();
}

void A()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__);
   B(1,"text");
}

int main()
{  ManuProC::Tracer::Enable(trace_channel);
   A();
   return 0;
}
