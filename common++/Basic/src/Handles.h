// $Id: Handles.h,v 1.11 2002/05/06 13:41:22 christof Exp $
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
	mutable unsigned int _references:24;
	bool _is_static:1;
#ifdef DEBUG_HANDLE_CONTENT
	mutable bool _watch_me:1;
#endif	
protected:	
	HandleContent() : _references(0), _is_static(false)
#ifdef DEBUG_HANDLE_CONTENT
					, _watch_me(false)
#endif
	{}
	// a virtual destructor is vital for heterogenous Handles
#if defined DEBUG_HANDLE_CONTENT || defined DEBUG_HANDLES
	virtual ~HandleContent();
#endif

public:
#ifdef DEBUG_HANDLE_CONTENT
	// emit message on destruction
	void WatchMe() const
	{  _watch_me=true; }
#endif	
	void *ref()
	{  /* if (!is_static()) */ _references++;
	   return this;
	}

	// HACK to return void* from const element
	void *ref() const
	{  return const_cast<HandleContent*>(this)->ref();
	}

	void unref() const
	{  /* if (!is_static()) */
	   {  _references--;
              if (!is_static() && !_references) delete this;
           }
	}

	// the return value is handy to call it right after object construction	
	// (static initializers)
	bool is_static(bool b)
	{  _is_static=b;
	   return b;
	}

	bool is_static() const
	{  return _is_static;
	}

	// this would have been impossible with Stroustrup's handles	
	static void unref(void *ptr)
	{  static_cast<HandleContent*>(ptr)->unref();
	}

private:
	// das darf gar nicht vorkommen, schlieﬂlich sollen diese Objekte
	// eigentlich nicht dupliziert werden
	const HandleContent &operator=(const HandleContent &b);
	HandleContent(const HandleContent &b);
};

class HandleContentCopyable : public HandleContent
{  public:
	HandleContentCopyable() {}
	HandleContentCopyable(const HandleContentCopyable &b) {}
	const HandleContentCopyable &operator=(const HandleContentCopyable &b)
	{  return *this; }
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
	   b->ref();
	   if (_data) _data->unref();
 	   _data=b._data;
 	   return *this;
	}
	
	// this STL functions do strange things, under investigation
	Handle(const _this_t &b) : _data(b._data)
	{  NOISE("Handle(@" << b._data << '.' << (b._data?b._data->_references:0) << ")\n");
	   if (_data) _data->ref();
	}
	
	// replace this default value FAST via *this=Something !!!
	// usually this is only needed for cached values
	Handle() : _data(0) { NOISE("Handle()\n"); }
	
	// without this test any std::exception in T::T(...) would kill your program
	~Handle()
	{  NOISE("~Handle" << _data << '.' << (_data?_data->_references:0) << '\n');
	   if (_data) _data->unref();
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
	   _data->ref();
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
