// $Id: Pointer.h,v 1.6 2005/11/04 09:22:18 christof Exp $
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

#ifndef POINTER_H
#define POINTER_H

// Pointer class in the spirit of Handles, makes transition to handles
// more easy in the future ...

// if your object resides in a static cache, you might use this one 
// though I recommend handles ("Prinzip")

template <class T> class Pointer
{private:
	T *_data;
	typedef class Pointer<T> _this_t;
public:
	// replace this default value FAST via *this=Something !!!
	Pointer() : _data(0) {}

	// BEWARE: might not act like expected, it compares content not address
	bool operator==(const _this_t &s) const
	{  return (*_data)==(*s);
	}
	bool operator!=(const _this_t &s) const
	{  return (*_data)!=(*s);
	}

	T *operator->() const
	{  return _data;
	}
	
	T &operator*() const
	{  return *_data;
	}
	
	Pointer(T *b) : _data(b)
	{  }

	// difference to handles: I cant do this with the object itself
	void *ref() const
	{  return (void*)_data;
	}
	bool operator!() const
	{ return !_data;
	}
};
#endif
