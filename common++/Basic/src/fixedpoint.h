// $Id: fixedpoint.h,v 1.2 2001/04/30 15:30:26 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#ifndef AUX_FIXEDPOINT_H
#define AUX_FIXEDPOINT_H

#include <Aux/ctime_assert.h>

// WOW prepare for black template magic
// The compiler optimizes this fully away (once -O is used)
template <int N> 
static inline double zehnhochminus()
{  typedef typename ctime_assert<(N>0)>::_true failed;
   return 0.1*zehnhochminus<N-1>();
}

template <>
static inline double zehnhochminus<0>()
{ return 1; }

template <int N> 
static inline double zehnhochplus()
{  typedef typename ctime_assert<(N>0)>::_true failed;
   return 10*zehnhochplus<N-1>();
}

template <>
static inline double zehnhochplus<0>()
{ return 1; }

template <int N> 
static inline int zehnhochplusI()
{  typedef typename ctime_assert<(N>0)>::_true failed;
   return 10*zehnhochplus<N-1>();
}

template <>
static inline int zehnhochplusI<0>()
{ return 1; }

#include <string>
#include <cstdlib>
#include <iostream>

// I don't want to include <cstdio>
extern "C" 
{  int snprintf(char *str, size_t size, const  char  *format, ...);
};


template <int decimals=2,class Ftype=double,class Itype=long>
class fixedpoint
{	Itype scaled; // scaled to integer
	typedef fixedpoint<decimals> self_t;
	typedef Ftype _Ftype;
	typedef Itype _Itype;
	const static int _decimals=decimals;
	typename ctime_assert<(decimals>=0)>::_true failed;
public:
	fixedpoint() : scaled(0) {}
	fixedpoint(const string &val)
	{  *this=strtod(val.c_str(),0);
	}
	fixedpoint(Ftype d) { *this=d; }
	self_t operator=(Ftype d)
	{  scaled=Itype(d*zehnhochplus<decimals>()+(d>0?.5:-.5));
   	   return *this;
	}
	operator Ftype() const
	{  return scaled*zehnhochminus<decimals>();
	}
	self_t operator*(Itype b) const
	{  self_t res;
	   res.scaled=scaled*b;
	   return res;
	}
	Ftype operator*(Ftype b) const
	{  return b*(Ftype)*this;
	}
	self_t operator+(const self_t b) const
	{  self_t res;
	   res.scaled=scaled+b.scaled;
	   return res;
	}
	self_t operator-(const self_t b) const
	{  self_t res;
	   res.scaled=scaled-b.scaled;
	   return res;
	}
	self_t operator-() const
	{  self_t res;
	   res.scaled=-scaled;
	   return res;
	}
	self_t operator+=(const self_t b)
	{  scaled+=b.scaled;
	   return *this;
	}
	self_t operator*=(const Ftype f)
	{  scaled=int(scaled*f+(f>0?.5:-.5));
	   return *this;
	}
	self_t operator-=(const self_t b)
	{  scaled-=b.scaled;
	   return *this;
	}
	// do we really need this function?
	string String() const
	{  char buf[64];
	   snprintf(buf,sizeof buf,"%.*f",decimals,Ftype(*this));
	   return buf;
	}
	Itype Scaled() const { return scaled; }
	size_t Scale() const { return decimals; }
	
	// separates into integral and broken part
	bool IsInteger(Itype &int_val, Itype &broken_val) const
	{  int_val=scaled/zehnhochplusI<decimals>();
	   broken_val=scaled-int_val*zehnhochplusI<decimals>();
	   return broken_val==0;
	}
	bool operator!() const
	{  return scaled==0;
	}
	// conversion operator
	template <int decimals2,class Ftype2,class Itype2>
	 fixedpoint(const fixedpoint<decimals2,Ftype2,Itype2> &f)
	{  *this=(Ftype)(Ftype2)f;
	}
};

template <int decimals,class Ftype,class Itype>
 inline ostream &operator<<(ostream &o,const fixedpoint<decimals,Ftype,Itype> &f)
{  return o << f.String();
}

#endif
