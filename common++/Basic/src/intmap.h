// $Id: intmap.h,v 1.1 2003/09/16 21:43:24 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

// the fastest associative container (if key is compatibe to an int)

#ifndef INTMAP_H
#define INTMAP_H

#include <vector>
//#include <utility> // for pair
//#include <algorithm> // for find_if (on 2.95), lower_bound

template <typename _Key, typename _Tp, _Key base=_Key(0)>
 class intmap
{
protected:
	typedef std::vector<_Tp> _Rep_type;
	_Rep_type rep;
public:
	typedef _Key key_type;
	typedef _Tp mapped_type;
	typedef unsigned size_t;
	typedef typename _Rep_type::value_type value_type;
	typedef typename _Rep_type::iterator iterator;
	typedef typename _Rep_type::const_iterator const_iterator;

public:
	intmap(_Key max) : rep(size_t(max)-size_t(base)+1) {}
	intmap() {}
	mapped_type &operator[](key_type k)
	{  if ((size_t(k)-size_t(base)) >= rep.size())
		rep.resize(size_t(k)-size_t(base)+1);
	   return rep[size_t(k)-size_t(base)];
	}
};

#endif
