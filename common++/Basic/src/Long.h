/* $Id: Long.h,v 1.4 2002/05/09 12:46:00 christof Exp $ */
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

#ifndef LONG_H
#define LONG_H

class Long
{	long l;
public:
        Long() : l(0) {}
        Long(long l2) : l(l2) {}
        operator long() const { return l; }
        template <class T> Long operator+=(const T &t) { return l+=t; }
        template <class T> Long operator-=(const T &t) { return l-=t; }
        template <class T> Long operator*=(const T &t) { return l*=t; }
        template <class T> Long operator/=(const T &t) { return l/=t; }
        template <class T> Long operator%=(const T &t) { return l%=t; }
        Long operator++() { return ++l; }
        Long operator++(int) { return l++; }
};

#endif
