// $Id: callbackbuf.h,v 1.2 2002/12/18 10:31:40 christof Exp $
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 2002  Adolf Petig GmbH. & Co. KG
    Developed by Christof Petig <christof.petig@wtal.de>
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef MISC_CALLBACKBUF_H
#define MISC_CALLBACKBUF_H
#include <iostream> 

template <class T>
 class callbackbuf : public std::streambuf
{public:
	typedef char char_type;
	typedef int int_type;
	typedef std::streamsize (T::*callback_t)(const char_type* __s, std::streamsize __n);
        
	callbackbuf(T &_o, callback_t _cb) : os(_o), cb(_cb) {}
protected:
	T &os;
	callback_t cb;
	
   int_type overflow(int_type c) 
   {  char_type a=c;
      if (c!=-1) 
      {  if (((os.*cb)(&a,1))<1)
         {  return -1;
         }
      }
      return c;
   }
	std::streamsize xsputn(const char_type* __s, std::streamsize __n)
	{  return (os.*cb)(__s,__n);
	}
};
#endif
