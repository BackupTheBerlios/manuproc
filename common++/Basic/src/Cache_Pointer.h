// $Id: Cache_Pointer.h,v 1.4 2002/06/24 07:35:40 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#ifndef AUX_CACHEPOINTER_H
#define AUX_CACHEPOINTER_H

#include <Misc/safemap.h>

// Implementation of a _non_ copying cache
// WARNING:
// this only makes sense for classes which are stored elsewhere !!!
// Otherwise use CacheStatic !!! (see Prozess)

template <class Index,class Payload>
 class Cache_Pointer
{	typedef safemap<Index,Payload *> map_t;
	map_t _mp;
public:
	CachePointer() throw() {}
	Payload *Register(const Index &i,Payload *p) throw()
	{  return _mp[i]=p; }
	void deregister(const Index &ix,const Payload *p) throw()
	{  map_t::iterator i(_mp.find(ix));
	   if (i!=_mp.end() && i->second==p) _mp.erase(i);
	}
	Payload *lookup(const Index &ix) throw()
	{  map_t::iterator i(_mp.find(ix));
	   if (i!=_mp.end()) return i->second;
	   return 0;
	}
};
#endif
