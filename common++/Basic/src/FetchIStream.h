// $Id: FetchIStream.h,v 1.22 2003/01/29 13:56:00 christof Exp $
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

#ifndef MPB_FETCHISTREAM_H
#define MPB_FETCHISTREAM_H
#include <string>
#include <vector>
#include <list>
#include <map>
#include <Misc/SQLerror.h>
extern "C" {
#include <libpq-fe.h>
}

class FetchIStream
{	int naechstesFeld;
	/* const */ int zeile;
	const PGresult * /* const */ result;

	friend class Query;
	// print if debugging on and then throw it	
	static void mythrow(const SQLerror &e);
public:
	FetchIStream(const std::string &descr, int line=0);
	FetchIStream(const PGresult *res=0, int line=0)
	  : naechstesFeld(0), zeile(line), result(res)
	{}
	
	int getIndicator() const;
	bool good() const
	{  return result; }
	
	FetchIStream &operator>>(std::string &str);
	FetchIStream &operator>>(int &i);
	FetchIStream &operator>>(unsigned &i);
	FetchIStream &operator>>(long &i);
	FetchIStream &operator>>(unsigned long &i);
	FetchIStream &operator>>(float &f);
	FetchIStream &operator>>(double &f);
	FetchIStream &operator>>(bool &b);
	FetchIStream &operator>>(char &c);
	
	template <class T>
	 class WithIndicator
	{	T &var;
		int &ind;
		
		friend class FetchIStream;
	 public:
	 	WithIndicator(T &v,int &i)
	 	  : var(v), ind(i) {}
	};
	template <class T>
	 FetchIStream &operator>>(const WithIndicator<T> &wi)
	{  if ((wi.ind=getIndicator())) ++naechstesFeld;
	   else *this >> wi.var;
	   return *this;
	}

	template <class T>
	 class MapNull
	{	T &var;
		T nullval;
		
		friend class FetchIStream;
	 public:
	 	MapNull(T &v,const T &nv)
	 	 : var(v), nullval(nv) {}
	};
	template <class T>
	 FetchIStream &operator>>(const MapNull<T> &mn)
	{  if (getIndicator()) 
	   {  ++naechstesFeld;
	      mn.var=mn.nullval;
	   }
	   else *this >> mn.var;
	   return *this;
	}
	
	void ThrowIfNotEmpty(const char *where);
};

class Query
{	std::string descriptor;
	bool eof;
	int line;
	const PGresult *result;
	unsigned num_params;
	unsigned params_needed;
	std::string query;
	std::vector<std::string> params;
	FetchIStream embedded_iterator;
	
	// not possible yet (because result can not refcount)
	const Query &operator=(const Query &);
	Query(const Query &);
	
	// perform it
	void Execute();
public:
	Query(const std::string &command);
	~Query();

	bool good() const 
	{ return !eof; }
	void ThrowOnBad(const char *where) const;

	static void Execute(const std::string &command);

	//-------------------- parameters ------------------
	// must be already quoted for plain SQL inclusion
	void add_argument(const std::string &s);

	// for user defined << operators and temporary queries 
	// 	you need to insert this one
	// e.g. Query("...").lvalue() << your_type ...;
	Query &lvalue() { return *this; }

	Query &operator<<(const std::string &str);
	Query &operator<<(int i)
	{  return operator<<(long(i)); }
	Query &operator<<(unsigned long i);
	Query &operator<<(long i);
	Query &operator<<(unsigned i)
	{  return operator<< ((unsigned long)(i)); }
	Query &operator<<(unsigned long long i);
	Query &operator<<(double f);
	Query &operator<<(bool b);
	Query &operator<<(char c);
	Query &operator<<(const char *s)
	{  return operator<<(std::string(s)); }
	struct null { null(){} };
	Query &operator<<(null n)
	{  add_argument("null"); return *this; }
	template <class T>
	 struct NullIf
	{	T data;
		bool null;
		
		NullIf(const T &a,const T &b) : data(a), null(a==b) {}
	};
	template <class T>
	 Query &operator<<(const NullIf<T> &n)
	{  if (n.null) return operator<<(null());
	   return (*this)<<(n.data);
	}
	
	//--------------- result --------------------
	FetchIStream &Fetch();
	FetchIStream &FetchOne();
	void Fetch(FetchIStream &);
        Query &operator>>(FetchIStream &s);
        // we might as well define >> for this
	template <class T> void FetchArray(std::vector<T> &);
	template <class T> void FetchArray(std::list<T> &);
	template <class T1, class T2> void FetchArray(std::map<T1,T2> &);
	// please do not use this variant in new code!
	// template <class T> std::vector<T> FetchArray();

	template <class T> T FetchOne();
	template <class T> void FetchOne(T &);

	template <class T> FetchIStream &operator>>(T &x)
	{  return FetchOne() >> x; }
	template <class T> FetchIStream &operator>>(const FetchIStream::MapNull<T> &x)
	{  return FetchOne() >> x; }
	template <class T> FetchIStream &operator>>(const FetchIStream::WithIndicator<T> &x)
	{  return FetchOne() >> x; }
};

// we use the embedded FetchIStream but that's ok, 
// 	since it makes no sense to mix us with Fetch[One]
template <class T> 
void Query::FetchArray(std::vector<T> &res)
{  ThrowOnBad(__FUNCTION__);
   while (((*this)>>embedded_iterator).good()) 
   { T x;
     embedded_iterator >> x;
     embedded_iterator.ThrowIfNotEmpty(__FUNCTION__);
     res.push_back(x);
   }
}

template <class T1, class T2> 
void Query::FetchArray(std::map<T1,T2> &res)
{  ThrowOnBad(__FUNCTION__);
   while (((*this)>>embedded_iterator).good()) 
   { T1 x;
     T2 y;
     embedded_iterator >> x >> y;
     embedded_iterator.ThrowIfNotEmpty(__FUNCTION__);
     res[x]=y;
   }
}

template <class T> 
void Query::FetchArray(std::list<T> &res)
{  ThrowOnBad(__FUNCTION__);
   while (((*this)>>embedded_iterator).good()) 
   { T x;
     embedded_iterator >> x;
     embedded_iterator.ThrowIfNotEmpty(__FUNCTION__);
     res.push_back(x);
   }
}

// T a; FetchOne(a); variant
template <class T>
void Query::FetchOne(T &res)
{  ThrowOnBad(__FUNCTION__);
   (FetchOne() >> res).ThrowIfNotEmpty(__FUNCTION__);
}

// T a = q.FetchOne<T>(); variant (slower)
template <class T>
T Query::FetchOne()
{  T res;
   FetchOne(res);
   return res;
}

#endif
