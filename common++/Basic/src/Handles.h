// $Id: Handles.h,v 1.1 2001/04/23 08:11:59 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef HANDLES_H
#define HANDLES_H
#define DEBUG_HANDLES

// Handle class to make passing ExtBezSchemas faster and easier
// (java like reference counting)

// WARNING: I don't know whether this is exception safe !!!!!!!

// it is always a good idea to derive other objects from this

#ifdef DEBUG_HANDLES
#include <cassert>
#endif
#ifdef HANDLES_NOISY
#include <iostream>
#define NOISE(x) cerr << x
#else
#define NOISE(x) 
#endif

template <class T> class Handle;
template <class T> class const_Handle;

class HandleContent
{	template<class T> friend class Handle;
	template<class T> friend class const_Handle;
private:
	mutable unsigned int _references;
protected:	
	HandleContent() : _references(0) {}
	// a virtual destructor is vital for heterogenous Handles
	virtual ~HandleContent()
	{
#ifdef DEBUG_HANDLES
	   assert(!_references);
#endif
	}
};

template <class T> class Handle
{private:
	T *_data;
	typedef class Handle<T> _this_t;
public:
	_this_t &operator=(const _this_t &b)
	{  b->_references++;
	   if (_data)
	   {  _data->_references--;
	      if (!_data->_references) delete _data;
	   }
 	   _data=b._data;
 	   return *this;
	}
	
	Handle(const _this_t &b) : _data(b._data)
	{  _data->_references++; }
	
	// replace this default value FAST via *this=Something !!!
	Handle() : _data(0) {}
	
	~Handle()
	{  _data->_references--;
	   if (!_data->_references) delete _data;
	}
	
	bool operator==(const _this_t &s) const
	{  return (*_data)==(*s);
	}

	T *operator->() const
	{  return _data;
	}
	
	T &operator*() const
	{  return *_data;
	}
	
	Handle(T *b) : _data(b)
	{  _data->_references++;
	}

	void *ref() const
	{  _data->_references++;
	   return _data;
	}
	
	void unref() const
	{  _data->_references--;
	   // we should at least have one referrer left - us
	   assert(_data->_references);
	}

	// this would have been impossible with Stroustrup's handles	
	static void unref(void *ptr)
	{  T *d=static_cast<T*>(ptr);
	   d->_references--;
	   if (!d->_references) delete d;
	}
};

template <class T> class const_Handle
{private:
	const T *_data;
	typedef class const_Handle<T> _this_t;
public:
	_this_t &operator=(const _this_t &b)
	{  NOISE("const_Handle @" << _data << '.' << (_data?_data->_references:0) << "= @" << b._data << '.' << (b._data?b._data->_references:0) << '\n');
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
	const_Handle(const _this_t &b) : _data(b._data)
	{  NOISE("const_Handle(@" << b._data << '.' << (b._data?b._data->_references:0) << ")\n");
	   if (_data) _data->_references++; }
	
	// replace this default value FAST via *this=Something !!!
	// usually this is only needed for cached values
	const_Handle() : _data(0) { NOISE("const_Handle()\n"); }
	
	// without this test any exception in T::T(...) would kill your program
	~const_Handle()
	{  NOISE("~const_Handle" << _data << '.' << (_data?_data->_references:0) << '\n');
	   if (_data) 
	   {  _data->_references--;
	      if (!_data->_references) delete _data;
	   }
	}
	
	bool operator==(const _this_t &s) const
	{  return (*_data)==(*s);
	}

	const T *operator->() const
	{  NOISE("*const_Handle" << _data << '.' << (_data?_data->_references:0) << '\n');
	   return _data;
	}
	
	const T &operator*() const
	{  return *_data;
	}
	
	const_Handle(const T *b) : _data(b)
	{  NOISE("const_Handle(from " << b << '.' << (b?b->_references:0) << ")\n");
	   _data->_references++;
	}
	
	void *ref() const
	{  _data->_references++;
	   return _data;
	}
	
	void unref() const
	{  _data->_references--;
	   // we should at least have one referrer left - us
	   assert(_data->_references);
	}

	// this would have been impossible with Stroustrup's handles	
	static void unref(void *ptr)
	{  T *d=static_cast<T*>(ptr);
	   d->_references--;
	   if (!d->_references) delete d;
	}
	
//	T &operator T() { return *_data; } ???
};
#undef NOISE
#endif
