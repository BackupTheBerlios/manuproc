/* $Id: Model_ref_bvector.h,v 1.3 2003/10/23 12:38:49 christof Exp $ */
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

#ifndef MANUPROC_MODEL_REF_BVECTOR_H
#define MANUPROC_MODEL_REF_BVECTOR_H
#include <BaseObjects/Model.h>
#include <vector>

typedef std::vector<bool>::reference bvector_reference;
typedef std::vector<bool>::iterator bvector_iterator;
typedef std::vector<bool>::reference bvector_item;

class Model_ref<bvector_item>
{	bvector_iterator value;
	SigC::Signal1<void,bvector_iterator> *changed;
public:
	Model_ref(const bvector_iterator &v, SigC::Signal1<void,bvector_iterator> &sig)
	: value(v), changed(&sig) {}
// dangerous - depends on internal representation
// please use  Model_ref<bvector_item>(v.begin()+n,signal);
#if 0
	Model_ref(const bvector_reference &v, SigC::Signal1<void,bvector_iterator> &sig)
	: value(v._M_p,v._M_mask), changed(&sig) {}
#endif
	Model_ref() : value(), changed() {}

	bool valid() const { return value!=bvector_iterator() && changed; }
	bool operator!() const { return !valid(); }
	SigC::Signal1<void,bvector_iterator> &signal_changed() const
	{  return *changed; }
	// g++ 2.95 does not use this ...
	operator bvector_reference() const
	{  return *value; }
	// .... so we need these - choose by your taste
	bvector_reference get_value() const
	{  return *value; }
	bvector_reference Value() const
	{  return *value; }
	
	bvector_iterator Id() const
	{  return value; }
	bool matches(const bvector_iterator &i) const
	{  return value==bvector_iterator() || value==i;
	}
	
	const bvector_reference operator=(bool v)
	{  *value=v;
	   signal_changed()(Id());
	   return *value;
	}
	const Model_ref<bvector_item> &operator=(const Model_ref<bvector_item> &m)
	{  value=m.value;
	   changed=m.changed;
	   return *this;
	}
};

#endif
