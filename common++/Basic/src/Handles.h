// $Id: Handles.h,v 1.8 2002/01/22 09:15:55 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

// Handle class to make passing Objects faster and easier
// (java like reference counting)

// WARNING: I don't know whether this is std::exception safe !!!!!!!

#ifndef HANDLES_H
#define HANDLES_H

#include <ManuProCConfig.h>
#define DEBUG_HANDLES

#ifdef DEBUG_HANDLES
#include <cassert>
#endif
#ifdef HANDLES_NOISY
#include <iostream>
#define NOISE(x) std::cerr << x
#else
#define NOISE(x) 
#endif

template <class T> class Handle;

class HandleContent
{	template<class T> friend class Handle;
private:
	mutable unsigned int _references;
#ifdef DEBUG_HANDLE_CONTENT
	mutable bool _watch_me;
#endif	
protected:	
	HandleContent() : _references(0) 
#ifdef DEBUG_HANDLE_CONTENT
					, _watch_me(false)
#endif
	{}
	// a virtual destructor is vital for heterogenous Handles
#ifndef DEBUG_HANDLE_CONTENT	
	virtual ~HandleContent()
	{
#ifdef DEBUG_HANDLES
	   assert(!_references);
#endif
	}
#else
	virtual ~HandleContent();

	// emit message on destruction
public:
	void WatchMe() const
	{  _watch_me=true; }
#endif	
public:
	void *ref()
	{  _references++;
	   return this;
	}

	void *ref() const
	{  _references++;
	   return static_cast<void*>(const_cast<HandleContent*>(this));
	}

	void unref() const
	{  _references--;
           if (!_references) delete this;
	}

	// this would have been impossible with Stroustrup's handles	
	static void unref(void *ptr)
	{  HandleContent *d=static_cast<HandleContent*>(ptr);
	   d->_references--;
	   if (!d->_references) delete d;
	}

private:
	// das darf gar nicht vorkommen, schlieﬂlich sollen diese Objekte
	// nicht dupliziert werden
	const HandleContent &operator=(const HandleContent &b);
	HandleContent(const HandleContent &b);
};

template <class T> class Handle
{public:
	typedef T ContentType;
private:
	ContentType *_data;
	typedef class Handle<ContentType> _this_t;
public:
	_this_t &operator=(const _this_t &b)
	{  NOISE("Handle @" << _data << '.' << (_data?_data->_references:0) << "= @" << b._data << '.' << (b._data?b._data->_references:0) << '\n');
	   // yes, I do not test b -- I consider b->nil as a bug
	   b->_references++;
	   if (_data)
	   {  _data->_references--;
              if (!_data->_references) delete _data;
           }
 	   _data=b._data;
 	   return *this;
	}
	
	// this STL functions do strange things, under investigation
	Handle(const _this_t &b) : _data(b._data)
	{  NOISE("Handle(@" << b._data << '.' << (b._data?b._data->_references:0) << ")\n");
	   if (_data) _data->_references++; }
	
	// replace this default value FAST via *this=Something !!!
	// usually this is only needed for cached values
	Handle() : _data(0) { NOISE("Handle()\n"); }
	
	// without this test any std::exception in T::T(...) would kill your program
	~Handle()
	{  NOISE("~Handle" << _data << '.' << (_data?_data->_references:0) << '\n');
	   if (_data) 
	   {  _data->_references--;
	      if (!_data->_references) delete _data;
	   }
	}
	
	bool operator==(const _this_t &s) const
	{  return (*_data)==(*s);
	}
	bool operator!=(const _this_t &s) const
	{  return (*_data)!=(*s);
	}
	bool operator<(const _this_t &s) const
	{  return (*_data)<(*s);
	}
	bool operator>(const _this_t &s) const
	{  return (*_data)>(*s);
	}
	bool operator<=(const _this_t &s) const
	{  return (*_data)<=(*s);
	}
	bool operator>=(const _this_t &s) const
	{  return (*_data)>=(*s);
	}
	bool operator!() const
	{  return _data==0;
	}

	ContentType *operator->() const
	{  NOISE("*Handle" << _data << '.' << (_data?_data->_references:0) << '\n');
	   return _data;
	}
	
	ContentType &operator*() const
	{  return *_data;
	}
	
	Handle(ContentType *b) : _data(b)
	{  NOISE("Handle(from " << b << '.' << (b?b->_references:0) << ")\n");
	   _data->_references++;
	}

//	ContentType &operator ContentType() { return *_data; } ???


private: // deprecated, use cH->Foo() !
	void *ref() const;
	void *ref_from_const() const;
	void unref() const;
	static void unref(void *ptr);
};

#undef NOISE

#endif
