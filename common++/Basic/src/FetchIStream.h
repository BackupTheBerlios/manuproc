// $Id: FetchIStream.h,v 1.14 2002/11/13 08:13:07 christof Exp $
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
	FetchIStream &operator>>(long &i);
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
};

class Query
{	std::string descriptor;
	bool eof;
	int line;
	const PGresult *result;
	
	// not possible yet
	const Query &operator=(const Query &b);
public:
	Query(const std::string &command);
	FetchIStream Fetch();
	bool good() const 
	{ return !eof; }
	~Query();
	static void Execute(const std::string &command);
	template <class T> void FetchArray(std::vector<T> &);
	template <class T> void FetchArray(std::list<T> &);
	template <class T> void FetchOne(T &);
	template <class T> std::vector<T> FetchArray();
	template <class T> T FetchOne();
};

static inline Query &operator>>(Query &q, FetchIStream &s)
{  s=q.Fetch();
   return q;
}

template <class T> 
void Query::FetchArray(std::vector<T> &res)
{  if (!good()) 
   { SQLerror::test(__FUNCTION__); FetchIStream::mythrow(SQLerror(__FUNCTION__,-1,"bad result")); }
   FetchIStream is;
   while (((*this)>>is).good()) 
   { T x;
     is >> x;
     res.push_back(x);
   }
}

template <class T> 
void Query::FetchArray(std::list<T> &res)
{  if (!good()) 
   { SQLerror::test(__FUNCTION__); FetchIStream::mythrow(SQLerror(__FUNCTION__,-1,"bad result")); }
   FetchIStream is;
   while (((*this)>>is).good()) 
   { T x;
     is >> x;
     res.push_back(x);
   }
}

template <class T>
void Query::FetchOne(T &res)
{  if (!good()) 
   { SQLerror::test(__FUNCTION__); FetchIStream::mythrow(SQLerror(__FUNCTION__,-1,"bad result")); }
   FetchIStream is=Fetch();
   is >> res;
   if (Fetch().good()) FetchIStream::mythrow(SQLerror(__FUNCTION__,-2,"more than one result"));
}

template <class T>
std::vector<T> Query::FetchArray()
{  std::vector<T> res;
   FetchArray(res);
   return res;
}

template <class T>
T Query::FetchOne()
{  T res;
   FetchOne(res);
   return res;
}

#endif
