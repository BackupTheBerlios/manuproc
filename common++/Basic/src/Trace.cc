// $Id: Trace.cc,v 1.4 2003/07/04 11:08:23 christof Exp $
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

#include <Misc/Trace.h>
#include <cassert>
#include <unistd.h>

static int enabled_parts;
static int depth;

static std::ostream *os;

ManuProC::Tracer::Environment::Environment(const std::string &name, Channel c)
{  if (getenv(name.c_str())) ManuProC::Tracer::Enable(c);
}

void ManuProC::Tracer::Enable(Channel c, bool an)
{  if (!os) os = &std::cerr;
   assert ((unsigned int)(c)<sizeof(enabled_parts)*8);
   if (an) enabled_parts|=1<<int(c);
   else enabled_parts&=~(1<<int(c));
}

std::ostream &ManuProC::Tracer::FunctionStart(const std::string &s)
{  
  if (depth<0) depth=0; // nonsense
  else if (depth) (*os) << std::string(depth,' ');
   (*os) << s << '(';
   depth++;
   return *os;
}

void ManuProC::Tracer::FunctionEnd()
{  --depth;
}

bool ManuProC::Tracer::enabled(Channel c)
{  return enabled_parts&(1<<int(c));
}

UniqueValue ManuProC::Tracer::channels;
