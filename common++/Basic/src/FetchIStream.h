// $Id: FetchIStream.h,v 1.66 2005/10/11 09:58:30 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001-2005 Adolf Petig GmbH & Co. KG, 
 *  written by Christof Petig
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
#include <ManuProCConfig.h>
#include <Misc/SQLerror.h>
#include <Misc/compiler_ports.h>

#ifndef MPC_SQLITE
#  define MPC_POSTGRESQL
#endif

#ifdef MPC_SQLITE
#include <algorithm>
#include <sqlite3.h>
typedef unsigned long Oid;
// enum ECPG...
#else
#include <ecpgerrno.h>
extern "C" {
#include <libpq-fe.h>
}
#endif

#ifndef OLD_ECPG
#define USE_PARAMETERS
#endif

// please access this class under the new alias "Query::Row"
class Query_Row
{public:
	struct check_eol { check_eol() {} };
	class Fake;
private:
	int naechstesFeld;
	/* const */ int zeile;
	
#ifdef MPC_SQLITE
	const char * const * result;
	unsigned nfields;
#else
protected:
        const PGresult * /* const */ result;
private:
#endif	

	friend class Query;
	friend class ArgumentList;
	// print if debugging on and then throw it	
	static void mythrow(const SQLerror &e);

	template <class T>
	 class MapNull_s
	{	T &var;
		T nullval;
		
		friend class Query_Row;
	 public:
	 	template <class U> MapNull_s(T &v,const U &nv)
	 	 : var(v), nullval(nv) {}
	};
	template <class T>
	 class WithIndicator_s
	{	T &var;
		int &ind;
		
		friend class Query_Row;
	 public:
	 	WithIndicator_s(T &v,int &i)
	 	  : var(v), ind(i) {}
	};
public:
#ifndef MPC_SQLITE
	Query_Row(const std::string &descr, int line=0);
	Query_Row(const PGresult *res=0, int line=0)
	  : naechstesFeld(0), zeile(line), result(res)
	{}
#else
	Query_Row(const char *const *res=0, unsigned nfields=0, int line=0);
#endif
	
	int getIndicator() const;
#ifdef MPC_POSTGRESQL
        std::string getFieldName() const;
#endif
	bool good() const; // noch Spalten verfügbar
	
	Query_Row &operator>>(std::string &str);
	Query_Row &operator>>(int &i);
	Query_Row &operator>>(unsigned &i);
	Query_Row &operator>>(long &i);
	Query_Row &operator>>(long long &i);	
	Query_Row &operator>>(unsigned long &i);
	Query_Row &operator>>(float &f);
	Query_Row &operator>>(double &f);
	Query_Row &operator>>(bool &b);
	Query_Row &operator>>(char &c);
	void operator>>(const check_eol &eol)
	{ ThrowIfNotEmpty("check_eol"); }
	
	template <class T> static WithIndicator_s<T> WithIndicator(T &v,int &i)
	{ return WithIndicator_s<T>(v,i); }
	template <class T>
	 Query_Row &operator>>(const WithIndicator_s<T> &wi)
	{  if ((wi.ind=getIndicator())) ++naechstesFeld;
	   else *this >> wi.var;
	   return *this;
	}

	template <class T,class U> static MapNull_s<T> MapNull(T &v,const U&nv)
	{ return MapNull_s<T>(v,nv); }
	template <class T> static MapNull_s<T> MapNull(T &v)
	{ return MapNull_s<T>(v,T()); }
	template <class T>
	 Query_Row &operator>>(const MapNull_s<T> &mn)
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

typedef Query_Row FetchIStream;

// one time internal (fake) result (mostly a hack)
class Query_Row::Fake : public Query_Row
{ 	  std::string value;
          void operator=(const Fake &);
          
          void init();
public:
          Fake() { init(); } // NULL
          Fake(const std::string &value);
          Fake(const Fake &);
          ~Fake();
};


struct Query_types
{	template <class T>
	 struct NullIf_s
	{	T data;
		bool null;
		
		template <class U> NullIf_s(const T &a,const U &b) : data(a), null(a==b) {}
	};
	struct null { null(){} };
	typedef Query_Row::check_eol check_eol;
};

class ArgumentList
{	unsigned params_needed;
        std::vector<Oid> types;
	std::vector<std::string> params;
	std::vector<bool> binary;
public:
	typedef std::vector<std::string>::const_iterator const_iterator;
	ArgumentList() : params_needed(unsigned(-1)) {}
	void setNeededParams(unsigned i)
	{  params_needed=i; }
	bool complete() const { return !params_needed; }
	unsigned HowManyNeededParams() const { return params_needed; }
	const_iterator begin() const { return params.begin(); }
	const_iterator end() const { return params.end(); }
	Oid type_of(const_iterator const& which) const;
	size_t size() const { return params.size(); }

	//-------------------- parameters ------------------
	// must be already quoted for plain SQL inclusion
	__deprecated ArgumentList &add_argument(const std::string &s);
	ArgumentList &add_argument(const std::string &s, Oid type);

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
	ArgumentList &operator<<(Query_types::null n);
	
	template <class T>
	 ArgumentList &operator<<(const Query_types::NullIf_s<T> &n)
	{  if (n.null) return operator<<(Query_types::null());
	   return (*this)<<(n.data);
	}
	static const char *next_insert(const char *text);
};

class PreparedQuery;

class Query : public Query_types
{
#ifndef MPC_SQLITE
	std::string descriptor;
#endif
	bool eof;
	int line;
#ifdef MPC_SQLITE
	const char * const *result;
	unsigned nfields;
#else	
#ifndef USE_PARAMETERS
	const 
#endif	
	      PGresult *result;
#endif	
	std::string query;
	ArgumentList params;
	unsigned num_params;
	Query_Row embedded_iterator;
	int error; // error after execution
	unsigned lines; // lines affected
	PreparedQuery* prepare;
	std::string portal_name;
	
	// not possible yet (because result can not refcount)
	const Query &operator=(const Query &);
	Query(const Query &);
	
	// perform it
	void Execute() throw(SQLerror);
	void Execute_if_complete();
	void raise(std::string const& state, int code, std::string const& message, std::string const& detail=std::string());
	void raise(char const* state, int code, char const* message, char const* detail=0);

public:
        typedef Query_Row Row;
	struct check100 { check100(){} };
	typedef Row::check_eol check_eol;
	typedef SQLerror Error;

	Query(const std::string &command);
	Query(std::string const& portal_name, const std::string &command);
	Query(PreparedQuery const& prep);
	~Query();

	bool good() const 
	{ return !eof; }
	void ThrowOnBad(const char *where) const;

	static void Execute(const std::string &command) throw(SQLerror);
	int Result() const { return error; }
	unsigned LinesAffected() const { return lines; }
	
	void Check100() const throw(SQLerror);
	
	// please migrate to the functions above
	static __deprecated int Code(); // SQLCA.sqlcode
#ifndef MPC_SQLITE
	static __deprecated unsigned Lines(); // SQLCA.sqlcode
#endif

	//-------------------- parameters ------------------
	// must be already quoted for plain SQL inclusion
	__deprecated Query &add_argument(const std::string &s);
	// this is normal style
	Query &add_argument(const std::string &s, Oid type);

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
	Query_Row &Fetch();
	Query_Row &FetchOne();
	void Fetch(Query_Row &);
        Query &operator>>(Query_Row &s);
        Query &operator>>(const check100 &s);
        // we might as well define >> for this
	template <class T> void FetchArray(std::vector<T> &);
	template <class T> void FetchArray(std::list<T> &);
	template <class T1, class T2> void FetchArray(std::map<T1,T2> &);
	template <class T> void FetchArrayMap(std::vector<T> &,const T &nv=T());

	template <class T> T FetchOne();
	template <class T> void FetchOne(T &v);
	template <class T> T FetchOneMap(const T &nv=T());
	template <class T> void FetchOneMap(T &v, const T &nv=T());

	template <class T> Query_Row &operator>>(T &x)
	{  return FetchOne() >> x; }
	template <class T> Query_Row &operator>>(const Query_Row::MapNull_s<T> &x)
	{  return FetchOne() >> x; }
	template <class T> Query_Row &operator>>(const Query_Row::WithIndicator_s<T> &x)
	{  return FetchOne() >> x; }

	// SQLOPT=-E turns this on
	struct debug_environment
	{  bool on;
	   bool time_queries;
	   debug_environment();
	};
	static debug_environment debugging;
};

class PreparedQuery
{	std::string command;
#ifdef MPC_POSTGRESQL
        std::string name;
        bool active;
        std::vector<Oid> types;
#endif
public:
        PreparedQuery(std::string const& cmd) : command(cmd), active() {}
        std::string const& Command() const { return command; }
};

// we use the embedded Query_Row but that's ok, 
// 	since it makes no sense to mix us with Fetch[One]
template <class T> 
void Query::FetchArray(std::vector<T> &res)
{  ThrowOnBad(__FUNCTION__);
   while (((*this)>>embedded_iterator).good()) 
   { T x;
     embedded_iterator >> x >> Query::check_eol();
     res.push_back(x);
   }
}

template <class T> 
void Query::FetchArrayMap(std::vector<T> &res, const T &nv)
{  ThrowOnBad(__FUNCTION__);
   while (((*this)>>embedded_iterator).good()) 
   { T x;
     embedded_iterator >> Row::MapNull(x,nv) >> Query::check_eol();
     res.push_back(x);
   }
}

template <class T1, class T2> 
void Query::FetchArray(std::map<T1,T2> &res)
{  ThrowOnBad(__FUNCTION__);
   while (((*this)>>embedded_iterator).good()) 
   { T1 x;
     T2 y;
     embedded_iterator >> x >> y >> Query::check_eol();
     res[x]=y;
   }
}

template <class T> 
void Query::FetchArray(std::list<T> &res)
{  ThrowOnBad(__FUNCTION__);
   while (((*this)>>embedded_iterator).good()) 
   { T x;
     embedded_iterator >> x >> Query::check_eol();
     res.push_back(x);
   }
}

// T a; FetchOne(a); variant
template <class T>
void Query::FetchOne(T &res)
{  ThrowOnBad(__FUNCTION__);
   FetchOne() >> res >> Query::check_eol();
}

template <class T>
void Query::FetchOneMap(T &res, const T &nv)
{  ThrowOnBad(__FUNCTION__);
   (FetchOne() >> Row::MapNull(res,nv)) >> Query::check_eol();
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
