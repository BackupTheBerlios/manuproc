// $Id: recodestream.h,v 1.1 2002/12/16 09:42:36 christof Exp $
/*  Midgard Character Generator
 *  Copyright (C) 2001 Christof Petig
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

#ifndef RECODESTREAM_H
#define RECODESTREAM_H
#ifdef __MINGW32__

#include <iostream>

std::string utf82iso(const std::string &s);

class recodebuf : public std::streambuf
{public:
   typedef char char_type;
   typedef int int_type;
        
   recodebuf(std::ostream &_o) : o(_o) {}
protected:
   int_type overflow(int_type c) 
      {  if (c!=-1)
         {  assert(!(c&0x80));
            o << char(c);
         }
         return c;
      }
   streamsize xsputn(const char_type* __s, streamsize __n)
   {  o << utf82iso(std::string(__s,__n));
      return __n;
   }
private:
   std::ostream &o;
};

class orecodestream : public std::ostream 
{	recodebuf b;
public:
	orecodestream(std::ostream &o) : std::ostream(0), b(o)
	{  this->init(&b);
	}
};
#endif
#endif
