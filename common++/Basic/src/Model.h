/* $Id: Model.h,v 1.13 2004/04/29 13:20:35 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#ifndef MANUPROC_MODEL_H
#define MANUPROC_MODEL_H
#include <ManuProCConfig.h>
#if MPC_SIGC_VERSION >= 0x120
# include <sigc++/signal.h>
#else
# include <sigc++/basic_signal.h>
#endif

// The pointer is needed to distinguish between elements in a shared
// signal environment. But the syntax is so complex it's hardly worth 
// the gain. But a shared signal handler will benefit from it, too.

class Model_Base
{public:
	SigC::Signal1<void,void*> changed;
	SigC::Signal1<void,void*> &signal_changed()
	{ return changed; }
};

template <class T>
 class Model : public Model_Base
{	T value;

	// not a good idea to use these, use a ref
	Model(const Model<T> &x);
	const Model &operator=(const Model<T> &x);
public:
	Model() : value() {}
//	Model(const Model<T> &x) : value(x.value) {}
	Model(const T &v) : value(v) {}
	// g++ 2.95 does not use this ...
	operator T() const
	{  return value; }
	// .... so we need these - choose by your taste
	const T &get_value() const
	{  return value; }
	const T &Value() const
	{  return value; }
	// only use this to create a Model_ref !!!
	T *Valueptr() 
	{  return &value; }

	const void *Id() const
	{  return static_cast<const void *>(&value); }
	void *Id()
	{  return static_cast<void *>(&value); }
	bool matches(const void *gp) const
	{  return !gp || gp==Id();
	}
	
	const T &operator=(const T &v)
	{  value=v;
	   changed(Id());
	   return value;
	}
	// perhaps implement += -= ++ --
	
	// needed for structures, perhaps strange but the only way
	template <class X>
	 void Assign(X &r, const X &v)
	{  r=v;
	   changed(&r);
	}
};

// this class explicitely allows duplication of the model
// - useful for container insertion only
// make sure to connect your controller/view to the right model if use this !!!
template <class T>
 class Model_copyable : public Model<T>
{	
public:
	Model_copyable() {} 
        Model_copyable(const T &v) : Model<T>(v) {}
        Model_copyable(const Model_copyable<T> &v) : Model<T>(v.Value()) {}
	const T &operator=(const T &v)
	{  return Model<T>::operator=(v);
	}
	const Model_copyable &operator=(const Model_copyable<T> &x)
	{  Model<T>::operator=(x.Value());
	   return *this;
	}
};

// this permits multiple values to share a changed signal
//  - is this a good idea? we'll see ...

// if we see that we don't need the extra functionality:
// template <class T> typedef Model<T> &Model_ref;

template <class T>
 class Model_ref
{	T *value;
	SigC::Signal1<void,void*> *changed;
public:
	Model_ref(Model<T> &model)
	: value(model.Valueptr()), changed(&model.changed) {}
	Model_ref(T &v, SigC::Signal1<void,void*> &sig)
	: value(&v), changed(&sig) {}
	Model_ref(T *v, SigC::Signal1<void,void*> &sig)
	: value(v), changed(&sig) {}
	Model_ref() : value(), changed() {}

	bool valid() const { return value && changed; }
	bool operator!() const { return !valid(); }
	SigC::Signal1<void,void*> &signal_changed() const
	{  return *changed; }
	// g++ 2.95 does not use this ...
	operator T() const
	{  return *value; }
	// .... so we need these - choose by your taste
	const T &get_value() const
	{  return *value; }
	const T &Value() const
	{  return *value; }
	
	
	const void *Id() const
	{  return static_cast<const void *>(value); }
	void *Id()
	{  return static_cast<void *>(value); }
	bool matches(const void *gp) const
	{  return !gp || gp==Id();
	}
	
	const T &operator=(const T &v)
	{  *value=v;
	   signal_changed()(Id());
	   return *value;
	}
	const Model_ref<T> &operator=(const Model_ref<T> &m)
	{  value=m.value;
	   changed=m.changed;
	   return *this;
	}
};

#endif
