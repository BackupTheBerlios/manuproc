// $Id: vectormap.h,v 1.1 2003/09/16 08:54:37 christof Exp $
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

// slightly more space efficient but less performance variant of an associative container

#ifndef VECTORMAP_H
#define VECTORMAP_H

#include <vector>
#include <utility> // for pair

// unsorted variant

template <typename _Key, typename _Tp> // perhaps some day we might need _Compare
 class vectormap_u : public std::vector<std::pair<_Key,_Tp> >
{public:
	typedef _Key key_type;
	typedef _Tp mapped_type;
	// value_type ?

	iterator find(const key_type& x)
	{ return std::find(begin(),end(),x); }
	const_iterator find(const key_type& x) const
	{ return std::find(begin(),end(),x); }
	
	mapped_type &operator[](const key_type& k)
	{  iterator i=find(k);
	   if (i==end()) 
	   {  push_back(value_type(k,mapped_type()));
	      return back();
	   }
	   return i->second;
	}
}

#if 0
template <typename _Key, typename _Tp> // perhaps some day we might need _Compare
 class vectormap_s : public std::vector<std::pair<_Key,_Tp> >
{public:
	typedef _Key key_type;
	typedef _Tp mapped_type;
	// value_type ?

	iterator find(const key_type& x)
	{ return std::find(begin(),end(),x); }
	const_iterator find(const key_type& x) const
	{ return std::find(begin(),end(),x); }
	
	mapped_type &operator[](const key_type& k)
	{  iterator i=lower_bound(k);
	   if (i==end()) 
	   {  push_back(value_type(k,mapped_type()));
	      return back();
	   }
	   if (i->first==k)
	   {  return i->second;
	   }
	   i=insert(i,value_type(k,mapped_type()));
	   return i->second;
	}
}
#endif

#endif
