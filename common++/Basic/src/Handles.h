// $Id: Handles.h,v 1.21 2006/08/03 11:18:28 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *  Copyright (C) 2006 Christof Petig
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

// Handle class (aka smart pointer) to make passing Objects faster and easier
// works like a boost::intrusive_ptr (and provides java like reference counting)

// WARNING: I don't know whether this is std::exception safe !!!!!!!

#ifndef HANDLES_H
#define HANDLES_H

#include <ManuProCConfig.h>
#include <stdlib.h>
//#define DEBUG_HANDLES
//#define DEBUG_HANDLE_CONTENT
// paranoid handle checking code
#define MPC_HANDLE_MAGIC 0xf3c66e37

#if defined(DEBUG_HANDLES) || defined(MPC_HANDLE_MAGIC)
# include <cassert>
#endif
#ifdef HANDLES_NOISY
# include <iostream>
# define NOISE(x) std::cerr << x
#else
# define NOISE(x) 
#endif
#ifdef MPC_HANDLE_MAGIC
# define MPC_HANDLE_CHECK_MAGIC(hc) assert(!(hc) || (hc)->_magic==MPC_HANDLE_MAGIC)
#else
# define MPC_HANDLE_CHECK_MAGIC(hc)
#endif

template <class T> class Handle;

class HandleContent
{	template<class T> friend class Handle;
#ifdef MPC_HANDLE_MAGIC
protected:
	unsigned _magic;
#endif
private:
	// is using a bit field here a performance killer (?)
	mutable unsigned int _references:24;
	bool _is_static:1;
#ifdef DEBUG_HANDLE_CONTENT
	mutable bool _watch_me:1;
#endif	
protected:	
	HandleContent() : 
#ifdef MPC_HANDLE_MAGIC
			  		_magic(MPC_HANDLE_MAGIC),
#endif
			  _references(0), _is_static(false)
#ifdef DEBUG_HANDLE_CONTENT
					, _watch_me(false)
#endif
	{}
	// a virtual destructor is vital for heterogenous Handles
#if defined DEBUG_HANDLE_CONTENT || defined DEBUG_HANDLES
	virtual ~HandleContent();
#elif defined(MPC_HANDLE_MAGIC)
	virtual ~HandleContent() { _magic=0; }
#endif

public:
#ifdef DEBUG_HANDLE_CONTENT
	// emit message on destruction
	void WatchMe() const
	{ MPC_HANDLE_CHECK_MAGIC(this); 
	  _watch_me=true; }
#endif	
	void *ref()
	{  MPC_HANDLE_CHECK_MAGIC(this);
	   if (this) _references++;
	   return this;
	}

	// HACK to return void* from const element
	void *ref() const
	{  MPC_HANDLE_CHECK_MAGIC(this);
	   return const_cast<HandleContent*>(this)->ref();
	}

	void unref() const
	{  MPC_HANDLE_CHECK_MAGIC(this);
	   if (this)
	   {  _references--;
              if (!is_static() && !_references) delete this;
           }
	}

	// the return value is handy to call it right after object construction	
	// (static initializers)
	bool is_static(bool b)
	{  MPC_HANDLE_CHECK_MAGIC(this);
	   _is_static=b;
	   return b;
	}

	bool is_static() const
	{  MPC_HANDLE_CHECK_MAGIC(this);
	   return _is_static;
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
	HandleContentCopyable(const HandleContentCopyable &b) { MPC_HANDLE_CHECK_MAGIC(&b); }
	const HandleContentCopyable &operator=(const HandleContentCopyable &b)
	{ MPC_HANDLE_CHECK_MAGIC(this);
	  MPC_HANDLE_CHECK_MAGIC(&b); 
	  return *this;
	}
};

template <class T> class Handle
{public:
	typedef T ContentType;
private:
	ContentType *_data;
	typedef class Handle<ContentType> _this_t;
public:
	_this_t &operator=(const _this_t &b)
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   MPC_HANDLE_CHECK_MAGIC(b._data);
	   NOISE("Handle @" << _data << '.' << (_data?_data->_references:0) << "= @" << b._data << '.' << (b._data?b._data->_references:0) << '\n');
	   if (b._data) b._data->ref();
	   if (_data) _data->unref();
 	   _data=b._data;
 	   return *this;
	}
	
	// this STL functions do strange things, under investigation
	Handle(const _this_t &b) : _data(b._data)
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   NOISE("Handle(@" << b._data << '.' << (b._data?b._data->_references:0) << ")\n");
	   if (_data) _data->ref();
	}
	
	// replace this default value FAST via *this=Something !!!
	// usually this is only needed for cached values
	Handle() : _data(0) { NOISE("Handle()\n"); }

	template <typename X>
	 Handle(const Handle<X> &b) : _data(b.operator->())
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   NOISE("Handle(from " << b << '.' << (b?b->_references:0) << ")\n");
	   if (_data) _data->ref();
	}
	
	Handle(ContentType *b) : _data(b)
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   NOISE("Handle(from " << b << '.' << (b?b->_references:0) << ")\n");
	   if (_data) _data->ref();
	}

	// without this test any std::exception in T::T(...) would kill your program
	~Handle()
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   NOISE("~Handle" << _data << '.' << (_data?_data->_references:0) << '\n');
	   if (_data) _data->unref();
	}
	
	bool operator==(const _this_t &s) const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   MPC_HANDLE_CHECK_MAGIC(s._data);
	   return (*_data)==(*s);
	}
	bool operator!=(const _this_t &s) const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   MPC_HANDLE_CHECK_MAGIC(s._data);
	   return (*_data)!=(*s);
	}
	bool operator<(const _this_t &s) const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   MPC_HANDLE_CHECK_MAGIC(s._data);
	   return (*_data)<(*s);
	}
	bool operator>(const _this_t &s) const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   MPC_HANDLE_CHECK_MAGIC(s._data);
	   return (*_data)>(*s);
	}
	bool operator<=(const _this_t &s) const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   MPC_HANDLE_CHECK_MAGIC(s._data);
	   return (*_data)<=(*s);
	}
	bool operator>=(const _this_t &s) const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   MPC_HANDLE_CHECK_MAGIC(s._data);
	   return (*_data)>=(*s);
	}
	
	// Ich hoffe, diese Konversion tut niemandem weh? Sonst bitte Meldung
	operator bool() const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   return _data!=0;
	}
	bool operator!() const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   return _data==0;
	}

	ContentType *operator->() const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   NOISE("*Handle" << _data << '.' << (_data?_data->_references:0) << '\n');
	   return _data;
	}
	
	ContentType &operator*() const
	{  MPC_HANDLE_CHECK_MAGIC(_data);
	   return *_data;
	}
	
	// geht auch ¸ber ctor?
	template <typename X>
	 operator Handle<X>() const { MPC_HANDLE_CHECK_MAGIC(_data); return _data; }
	 
	// Variant:  base.cast_static<Derived>();
	template <typename X>
	 Handle<X> cast_static() const { MPC_HANDLE_CHECK_MAGIC(_data); return static_cast<X*>(_data); }
	template <typename X>
	 Handle<X> cast_dynamic() const { MPC_HANDLE_CHECK_MAGIC(_data); return dynamic_cast<X*>(_data); }
	template <typename X>
	 Handle<X> cast_const() const { MPC_HANDLE_CHECK_MAGIC(_data); return const_cast<X*>(_data); }

	 
	// Variant: Handle<Derived>::cast_static(base);
#if ! defined(__GNUC__) || __GNUC__>2
	template <class X>
	 static inline _this_t cast_static(const Handle<X>& src)
	{  return static_cast<T*>(src.operator->()); }
	template <class X>
	 static inline _this_t cast_dynamic(const Handle<X>& src)
	{  return dynamic_cast<T*>(src.operator->()); }
	template <class X>
	 static inline _this_t cast_const(const Handle<X>& src)
	{  return const_cast<T*>(src.operator->()); }
#endif	

//	ContentType &operator ContentType() { return *_data; } ???

private: // deprecated, use cH->Foo() !
	void *ref() const;
	void *ref_from_const() const;
	void unref() const;
	static void unref(void *ptr);
	// to make it clear that this won't work
	operator int() const;
};

#undef NOISE
#ifndef MPC_HANDLES_INTERNAL
#undef MPC_HANDLES_CHECK_MAGIC
#undef MPC_HANCLES_MAGIC
#undef DEBUG_HANDLE_CONTENT
#undef DEBUG_HANDLES
#endif

#if 0 // if you want to use this with boost::intrusive_ptr
namespace boost
{ void intrusive_ptr_add_ref(HandleContent *c)
  { c->ref();
  }
  void intrusive_ptr_release(HandleContent *c)
  { c->unref();
  }
}
#endif

#endif
