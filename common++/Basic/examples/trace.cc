// $Id: trace.cc,v 1.7 2005/03/17 10:48:26 christof Exp $
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

#include <Misc/TraceNV.h>

static const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();
static const UniqueValue::value_t trace_channel2=ManuProC::Tracer::channels.get();
// you can enable this channel by: export TRACE=1
static ManuProC::Tracer::Environment trace_channel2e("TRACE",trace_channel2);

void C()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("first",1),
            2,3,4,5,6,7,8,9,10 );
}

void B(int i,const std::string &t)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV1(i),NV1(t));
   C();
}

void A()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__);
   B(1,"text");
}

int main()
{  ManuProC::Tracer::Enable(trace_channel);
   ManuProC::Trace(trace_channel2,"environment variable active");
   A();
   return 0;
}
