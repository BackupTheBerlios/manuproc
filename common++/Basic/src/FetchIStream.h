// $Id: FetchIStream.h,v 1.7 2002/05/06 13:41:22 christof Exp $
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

class FetchIStream
{	std::string descriptor;
	int naechstesFeld;
public:
	FetchIStream(const std::string &descr="")
		: descriptor(descr), naechstesFeld(1)
	{}
	int getIndicator() const;
	bool good() const
	{  return !descriptor.empty(); }
	
	FetchIStream &operator>>(std::string &str);
	FetchIStream &operator>>(int &i);
	FetchIStream &operator>>(float &f);
	FetchIStream &operator>>(double &f);
	FetchIStream &operator>>(bool &b);
	
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
{	std::string cursor;
	bool eof;
	
	// not possible yet
	const Query &operator=(const Query &b);
public:
	Query(const std::string &command);
	FetchIStream Fetch();
	bool good() const 
	{ return !eof; }
	~Query();
	static void Execute(const std::string &command);
};

static inline Query &operator>>(Query &q, FetchIStream &s)
{  s=q.Fetch();
   return q;
}

#endif
