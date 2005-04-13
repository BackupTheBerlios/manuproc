// $Id: fixedpoint.h,v 1.24 2005/04/13 22:57:09 christof Exp $
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

#include <ManuProCConfig.h>
#include <Misc/ctime_assert.h>
#include <string>

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
   return 10*zehnhochplusI<N-1>();
}

template <>
static inline int zehnhochplusI<0>()
{ return 1; }

template <int decimals=2,class Ftype=double,class Itype=long>
class fixedpoint
{
public:
	struct ScaledValue
	{  Itype wert;
	   ScaledValue(Itype w) : wert(w) {}
	};
private:
	Itype scaled; // scaled to integer
	typedef fixedpoint<decimals,Ftype,Itype> self_t;
	typedef Ftype _Ftype;
	typedef Itype _Itype;
	const static int _decimals=decimals;
	typedef typename ctime_assert<(decimals>=0)>::_true failed;
public:
	fixedpoint() : scaled(0) {}
	fixedpoint(const std::string &val)
	{  *this=strtod(val.c_str(),0);
	}
	fixedpoint(Ftype d) { *this=d; }
	fixedpoint(ScaledValue val) { scaled=val.wert; }
	
	self_t operator=(Ftype d)
	{  scaled=Itype(d*zehnhochplus<decimals>()+(d>0?.5:-.5));
   	   return *this;
	}
#if 0 // sadly explicit does not work with operators
	explicit operator Ftype() const
	{  return as_float();
	}
	explicit operator Itype() const
	{  return as_int();
	}
#endif
#if 0 // stop this if you want to debug rounding errors
	operator Ftype() const
	{  return as_float();
	}
	bool operator>(int i) const
	{  return operator>(self_t(i));
	}
	bool operator<(int i) const
	{  return operator<(self_t(i));
	}
	bool operator>=(int i) const
	{  return operator>=(self_t(i));
	}
	bool operator<=(int i) const
	{  return operator<=(self_t(i));
	}
	bool operator!=(int i) const
	{  return operator!=(self_t(i));
	}
	bool operator==(int i) const
	{  return operator==(self_t(i));
	}
#endif
	Ftype as_float() const
	{  return scaled*zehnhochminus<decimals>();
	}
	Itype as_int() const
	{  return (scaled+(zehnhochplusI<decimals>()/2))/zehnhochplusI<decimals>();
	}
	self_t operator*(Itype b) const
	{  self_t res;
	   res.scaled=scaled*b;
	   return res;
	}
	Ftype operator*(Ftype b) const
	{  return b* as_float();
	}
	Ftype operator/(Ftype b) const
	{  return as_float()/b;
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
	{  scaled=Itype(scaled*f+(f*scaled>=0?.5:-.5));
	   return *this;
	}
	self_t operator-=(const self_t b)
	{  scaled-=b.scaled;
	   return *this;
	}
	
	bool operator==(const self_t b) const
	{  return scaled==b.scaled;
	}
	bool operator!=(const self_t b) const
	{  return scaled!=b.scaled;
	}
	bool operator<(const self_t b) const
	{  return scaled<b.scaled;
	}
	bool operator<=(const self_t b) const
	{  return scaled<=b.scaled;
	}
	bool operator>(const self_t b) const
	{  return scaled>b.scaled;
	}
	bool operator>=(const self_t b) const
	{  return scaled>=b.scaled;
	}
	
	std::string String(bool _short=false, unsigned int Ziellaenge=0,
		const char *TausenderTrennzeichen="",const char *Komma=".",
		char fuehrendesZeichen=' ') const;
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
	// conversion operators
	template <int decimals2,class Ftype2,class Itype2>
	 fixedpoint(const fixedpoint<decimals2,Ftype2,Itype2> &f)
	{  *this=(Ftype)(f.as_float());
	}
	template <int decimals2,class Ftype2,class Itype2>
	 const self_t &operator=(const fixedpoint<decimals2,Ftype2,Itype2> &f)
	{  *this=(Ftype)(f.as_float());
	   return *this;
	}
	template <int decimals2,class Ftype2,class Itype2>
	 Ftype operator*(const fixedpoint<decimals2,Ftype2,Itype2> &f) const
	{  return as_float()*f.as_float();
	}
	
	self_t abs() const
	{  if (scaled<0) return -*this;
	   else return *this;
	}
	Itype floor() const
	{  return scaled/zehnhochplusI<decimals>();
	}
	
	// to tell the compiler which way to prefer
	self_t operator+(const Ftype b) const
	{  return *this+self_t(b); }
};

#include <iostream>

template <int decimals,class Ftype,class Itype>
 inline std::ostream &operator<<(std::ostream &o,const fixedpoint<decimals,Ftype,Itype> &f)
{  return o << f.String(true);
}

// allow reversed notation
template <int decimals,class Ftype,class Itype>
 inline fixedpoint<decimals,Ftype,Itype> operator*(Itype i,const fixedpoint<decimals,Ftype,Itype> &f)
{  return f*i;
}
template <int decimals,class Ftype,class Itype>
 inline Ftype operator*(Ftype a,const fixedpoint<decimals,Ftype,Itype> &f)
{  return f*a;
}

#if 1
// allow int arguments even when Itype==long
template <int decimals,class Ftype,class Itype>
 inline fixedpoint<decimals,Ftype,Itype> operator*(int i,const fixedpoint<decimals,Ftype,Itype> &f)
{  return f*Itype(i);
}
template <int decimals,class Ftype,class Itype>
 inline fixedpoint<decimals,Ftype,Itype> operator*(const fixedpoint<decimals,Ftype,Itype> &f, int i)
{  return f.operator*(Itype(i));
}
// allow float arguments even when Ftype==double
template <int decimals,class Ftype,class Itype>
 inline fixedpoint<decimals,Ftype,Itype> operator*(float a,const fixedpoint<decimals,Ftype,Itype> &f)
{  return f.operator*(Ftype(a));
}
template <int decimals,class Ftype,class Itype>
 inline fixedpoint<decimals,Ftype,Itype> operator*(const fixedpoint<decimals,Ftype,Itype> &f, float b)
{  return f.operator*(Ftype(b));
}
#endif

#if defined(DEFAULT_DB) && defined(MANUPROC_WITH_DATABASE)
#include <Misc/FetchIStream.h>

static inline FetchIStream &operator>>(FetchIStream &is, fixedpoint<0> &v)
{  long l;
   is >> l;
   v=l;
   return is;
}

template <int decimals,class Ftype,class Itype>
FetchIStream &operator>>(FetchIStream &is, fixedpoint<decimals,Ftype,Itype> &v)
{  Ftype d;
   is >> d;
   v=d;
   return is;
}

template <int decimals,class Ftype,class Itype>
ArgumentList &operator<<(ArgumentList &q, const fixedpoint<decimals,Ftype,Itype> &v)
{  q.add_argument(v.String(true));
   return q;
}
#endif

#endif
