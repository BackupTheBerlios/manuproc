// $Id: bitmask.h,v 1.2 2003/08/02 22:28:52 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef MISC_BITMASK_H
#define MISC_BITMASK_H

// usually you instantiate this for an enum 
template <class E>
 class bitmask
{	long value;
	bitmask(long b) : value(b) {}
public:
	static const E none=E();
	static const E all=E(-1);
	bitmask() : value() {}
	bitmask(E b) : value(long(b)) {}
	bool operator&(E b) const
	{ return value&(1<<int(b)); }
	void operator|=(E b)
	{ value|=1<<int(b); }
	void operator+=(E b)
	{ operator|=(b); }
	void operator-=(E b)
	{ value&=~(1<<int(b)); }
	bitmask<E> operator|(E b) const
	{ return value|(1<<int(b)); }
	bitmask<E> operator+(E b) const
	{ return *this|b; }
	bitmask<E> operator-(E b) const
	{ return value&~(1<<int(b)); }
};
#endif
