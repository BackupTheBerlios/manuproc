// $Id: vectormap.h,v 1.4 2003/09/16 21:42:18 christof Exp $
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
#include <algorithm> // for find_if (on 2.95), lower_bound

// unsorted variant

template <typename _Key, typename _Tp> // perhaps some day we might need _Compare
 class vectormap_u : public std::vector<std::pair<_Key,_Tp> >
{
protected:
	typedef std::vector<std::pair<_Key,_Tp> > _Rep_type;
public:
	typedef _Key key_type;
	typedef _Tp mapped_type;
	typedef typename _Rep_type::value_type value_type;
	typedef typename _Rep_type::iterator iterator;
	typedef typename _Rep_type::const_iterator const_iterator;
protected:
	void push_back(const value_type& __x);
	
	class equal
	{  const key_type &k;
	 public:
	   equal(const key_type &_k) : k(_k) {}
	   bool operator()(const value_type &b) const
	   { return b.first==k; }
	};
public:
	iterator find(const key_type& x)
	{ return std::find_if(begin(),end(),equal(x)); }
	const_iterator find(const key_type& x) const
	{ return std::find_if(begin(),end(),equal(x)); }
	
	mapped_type &operator[](const key_type& k)
	{  iterator i=find(k);
	   if (i==end()) 
	   {  _Rep_type::push_back(value_type(k,mapped_type()));
	      return back().second;
	   }
	   return i->second;
	}
};

#if 1
template <typename _Key, typename _Tp>
 class vectormap_s : public vectormap_u<_Key,_Tp>
{
	typedef vectormap_u<_Key,_Tp> parent_t;
	typedef typename parent_t::_Rep_type _Rep_type;
public:
	typedef _Key key_type;
	typedef _Tp mapped_type;
	typedef typename _Rep_type::value_type value_type;
	typedef typename _Rep_type::iterator iterator;
	typedef typename _Rep_type::const_iterator const_iterator;
private:
	struct less
	{  bool operator()(const value_type &a,const key_type &b) const
	   { return a.first<b; }
	};

public:
	iterator lower_bound(const key_type& x)
	{ return std::lower_bound(begin(),end(),x,less()); }
	const_iterator lower_bound(const key_type& x) const
	{ return std::lower_bound(begin(),end(),x,less()); }
	iterator find(const key_type& x)
	{ iterator res=lower_bound(x);
	  if (res->first==x) return res;
	  return end();
	}
	const_iterator find(const key_type& x) const
	{ const_iterator res=lower_bound(x);
	  if (res->first==x) return res;
	  return end();
	}
	mapped_type &operator[](const key_type& k)
	{  iterator i=lower_bound(k);
	   if (i==end()) 
	   {  _Rep_type::push_back(value_type(k,mapped_type()));
	      return back().second;
	   }
	   if (i->first==k)
	   {  return i->second;
	   }
	   i=insert(i,value_type(k,mapped_type()));
	   return i->second;
	}
};
#endif

#endif
