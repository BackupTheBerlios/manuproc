/* $Id: callbackbuf.cc,v 1.4 2004/04/29 14:24:40 christof Exp $ */
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 1998  Adolf Petig GmbH. & Co. KG
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

#include <callbackbuf.h>
#include <iostream>

struct OSB : public std::ostream
{  callbackbuf<OSB> b;
   std::streamsize data_cb(const char_type* __s, std::streamsize __n);
   OSB();
};

std::streamsize OSB::data_cb(const char_type* __s, std::streamsize __n)
{  std::cout << "data_cb " << __n << '\n';
   return __n;
}

OSB::OSB() : std::ostream(0), b(*this,&OSB::data_cb)
{  this->init(&b);
}

int main()
{  OSB o;
   o << "test";
   return 0;
}
