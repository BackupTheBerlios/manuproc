// $Id: FetchIStream.h,v 1.35 2003/07/03 06:47:10 christof Exp $
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
	friend class ArgumentList;
	// print if debugging on and then throw it	
	static void mythrow(const SQLerror &e);

	template <class T>
	 class MapNull_s
	{	T &var;
		T nullval;
		
		friend class FetchIStream;
	 public:
	 	template <class U> MapNull_s(T &v,const U &nv)
	 	 : var(v), nullval(nv) {}
	};
	template <class T>
	 class WithIndicator_s
	{	T &var;
		int &ind;
		
		friend class FetchIStream;
	 public:
	 	WithIndicator_s(T &v,int &i)
	 	  : var(v), ind(i) {}
	};
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
	FetchIStream &operator>>(long long &i);	
	FetchIStream &operator>>(unsigned long &i);
	FetchIStream &operator>>(float &f);
	FetchIStream &operator>>(double &f);
	FetchIStream &operator>>(bool &b);
	FetchIStream &operator>>(char &c);
	
	template <class T> static WithIndicator_s<T> WithIndicator(T &v,int &i)
	{ return WithIndicator_s<T>(v,i); }
	template <class T>
	 FetchIStream &operator>>(const WithIndicator_s<T> &wi)
	{  if ((wi.ind=getIndicator())) ++naechstesFeld;
	   else *this >> wi.var;
	   return *this;
	}

	template <class T,class U> static MapNull_s<T> MapNull(T &v,const U&nv)
	{ return MapNull_s<T>(v,nv); }
	template <class T> static MapNull_s<T> MapNull(T &v)
	{ return MapNull_s<T>(v,T()); }
	template <class T>
	 FetchIStream &operator>>(const MapNull_s<T> &mn)
	{  if (getIndicator()) 
	   {  ++naechstesFeld;
	      mn.var=mn.nullval;
	   }
	   else *this >> mn.var;
	   return *this;
	}

	template <class T>
	 T Fetch()
	{  T res;
	   *this >> res;
	   return res;
	}
	template <class T>
	 T FetchMap(const T &nv=T())
	{  T res;
	   *this >> MapNull(res,nv);
	   return res;
	}
	
	void ThrowIfNotEmpty(const char *where);
};

struct Query_types
{	template <class T>
	 struct NullIf_s
	{	T data;
		bool null;
		
		template <class U> NullIf_s(const T &a,const U &b) : data(a), null(a==b) {}
	};
	struct null { null(){} };
};

class ArgumentList
{	unsigned params_needed;
	std::vector<std::string> params;
public:
	typedef std::vector<std::string>::const_iterator const_iterator;
	ArgumentList() : params_needed(unsigned(-1)) {}
	void setNeededParams(unsigned i)
	{  params_needed=i; }
	bool complete() const { return !params_needed; }
	unsigned HowManyNeededParams() const { return params_needed; }
	const_iterator begin() const { return params.begin(); }
	const_iterator end() const { return params.end(); }

	//-------------------- parameters ------------------
	// must be already quoted for plain SQL inclusion
	ArgumentList &add_argument(const std::string &s);

	ArgumentList &operator<<(const std::string &str);
	ArgumentList &operator<<(int i)
	{  return operator<<(long(i)); }
	ArgumentList &operator<<(unsigned long i);
	ArgumentList &operator<<(long i);
	ArgumentList &operator<<(unsigned i)
	{  return operator<< ((unsigned long)(i)); }
	ArgumentList &operator<<(unsigned long long i);
	ArgumentList &operator<<(double f);
	ArgumentList &operator<<(bool b);
	ArgumentList &operator<<(char c);
	ArgumentList &operator<<(const ArgumentList &list);
	ArgumentList &operator<<(const char *s)
	{  return operator<<(std::string(s)); }
	ArgumentList &operator<<(Query_types::null n)
	{  return add_argument("null"); }
	
	template <class T>
	 ArgumentList &operator<<(const Query_types::NullIf_s<T> &n)
	{  if (n.null) return operator<<(Query_types::null());
	   return (*this)<<(n.data);
	}
};

class Query : public Query_types
{	std::string descriptor;
	bool eof;
	int line;
	const PGresult *result;
	std::string query;
	ArgumentList params;
	unsigned num_params;
	FetchIStream embedded_iterator;
	
	// not possible yet (because result can not refcount)
	const Query &operator=(const Query &);
	Query(const Query &);
	
	// perform it
	void Execute();
	void Execute_if_complete();

public:
	Query(const std::string &command);
	~Query();

	bool good() const 
	{ return !eof; }
	void ThrowOnBad(const char *where) const;

	static void Execute(const std::string &command);
	static int Code(); // SQLCA.sqlcode
	static unsigned Lines(); // SQLCA.sqlerrd[2]

	//-------------------- parameters ------------------
	// must be already quoted for plain SQL inclusion
	Query &add_argument(const std::string &s);

	// for user defined << operators and temporary queries 
	// 	you need to insert this one
	// e.g. Query("...").lvalue() << your_type ...;
	// is this any longer true?
	Query &lvalue() { return *this; }

	template <class T>
	 Query &operator<<(const T &t)
	{  params << t;
	   Execute_if_complete();
	   return *this;
	}
	template <class T,class U> static NullIf_s<T> NullIf(const T &a,const U &b)
	{  return NullIf_s<T>(a,b); }
	template <class T> static NullIf_s<T> NullIf(const T &a)
	{  return NullIf_s<T>(a,T()); }
	
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
	template <class T> void FetchOne(T &v);
	template <class T> T FetchOneMap(const T &nv=T());
	template <class T> void FetchOneMap(T &v, const T &nv=T());

	template <class T> FetchIStream &operator>>(T &x)
	{  return FetchOne() >> x; }
	template <class T> FetchIStream &operator>>(const FetchIStream::MapNull_s<T> &x)
	{  return FetchOne() >> x; }
	template <class T> FetchIStream &operator>>(const FetchIStream::WithIndicator_s<T> &x)
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

template <class T>
void Query::FetchOneMap(T &res, const T &nv)
{  ThrowOnBad(__FUNCTION__);
   (FetchOne() >> FetchIStream::MapNull(res,nv)).ThrowIfNotEmpty(__FUNCTION__);
}

// T a = q.FetchOne<T>(); variant (slower)
template <class T>
T Query::FetchOne()
{  T res;
   FetchOne(res);
   return res;
}

template <class T>
T Query::FetchOneMap(const T &nv)
{  T res;
   FetchOneMap(res,nv);
   return res;
}

#endif
