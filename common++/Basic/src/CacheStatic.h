// $Id: CacheStatic.h,v 1.8 2002/10/24 14:06:49 thoma Exp $
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


#ifndef AUX_CACHESTATIC_H
#define AUX_CACHESTATIC_H

#include <Misc/safemap.h>

// Implementation of a _copying_ cache
// For an example see Prozess

template <class Index,class Payload>
 class CacheStatic
{	typedef safemap<Index,Payload> map_t;
	map_t _mp;
public:
	typedef typename map_t::const_iterator const_iterator;
	// if your default constructor is private, friend this class
	// I'd love if std C++ would allow to friend typedefs
	typedef typename map_t::stl_type stl_type;
	
	CacheStatic() throw() {}
	// register is a reserved word ...
	Payload *Register(const Index &i,const Payload &p) throw()
	{  Payload &pp(_mp[i]);
	   pp=p; 
	   return &pp;
	}
	void deregister(const Index &ix) throw()
	{  typename map_t::iterator i(_mp.find(ix));
	   if (i!=_mp.end()) _mp.erase(i);
	}
	void clear() throw()
	{  _mp.clear();
	}
	Payload *lookup(const Index &ix) throw()
	{  typename map_t::iterator i(_mp.find(ix));
	   if (i!=_mp.end()) return &i->second;
	   return 0;
	}
	const_iterator begin() const
	{  return _mp.begin(); }
	const_iterator end() const
	{  return _mp.end(); }
};
#endif
