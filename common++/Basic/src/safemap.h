// $Id: safemap.h,v 1.8 2003/06/04 09:22:59 christof Exp $
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

#ifndef MANUPROC_MISC_SAFEMAP_H
#define MANUPROC_MISC_SAFEMAP_H
#include <map>
//#include <cstdio>

// The reason for this class is that the execution order of static
// class initializers is unpredictable.
// Since many static ctors used the std::map before it was constructed I wrapped
// it in this class.

template <class _Key,class _Tp> 
 class safemap
{public:
  typedef _Key key_type;
  typedef _Tp data_type;
  typedef _Tp mapped_type;
private:
  typedef std::map<_Key,_Tp> _Rep_type;
public:
  typedef typename _Rep_type::value_type value_type;
  typedef typename _Rep_type::pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;

private:
	_Rep_type *m;
	
	void init() const 
	{ if (!m) const_cast<safemap<_Key,_Tp>*>(this)->m=new _Rep_type; }
public:
	// NOTICE: member functions might got called before we
	//         enter the ctor, NEVER touch m unconditionally here
	safemap() { init(); } 
	~safemap() { delete m; m=0; }
	
  iterator begin() { init(); return m->begin(); }
  const_iterator begin() const { init(); return m->begin(); }
  iterator end() { init(); return m->end(); }
  const_iterator end() const { init(); return m->end(); }
  reverse_iterator rbegin() { init(); return m->rbegin(); }
  const_reverse_iterator rbegin() const { init(); return m->rbegin(); }
  reverse_iterator rend() { init(); return m->rend(); }
  const_reverse_iterator rend() const { init(); return m->rend(); }
  bool empty() const { init(); return m->empty(); }
  void clear() { init(); m->clear();}
  size_type size() const { init(); return m->size(); }
  size_type max_size() const { init(); return m->max_size(); }
  _Tp& operator[](const key_type& k) { init(); return (*m)[k]; }
  iterator find(const key_type& __x) { init(); return m->find(__x); }
  void erase(iterator i) { init(); m->erase(i); }

  // if your default constructor is private, friend this class  
  typedef _Rep_type stl_type;
};
#endif
