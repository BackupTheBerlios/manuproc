/* $Id: auto_init.h,v 1.1 2005/10/11 15:16:14 christof Exp $ */
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

#ifndef A7192_C416_47B7_88FA_D9F04254B0C9
#define A7192_C416_47B7_88FA_D9F04254B0C9

// automatically initializing integer/float/enum type

template<typename B>
class auto_init
{ B v;
public:
  auto_init() : v() {}
  auto_init(B b) : v(b) {}
  operator B() const { return v; }
  template <class T> auto_init operator+=(const T &t) { return v+=t; }
  template <class T> auto_init operator-=(const T &t) { return v-=t; }
  template <class T> auto_init operator*=(const T &t) { return v*=t; }
  template <class T> auto_init operator/=(const T &t) { return v/=t; }
  template <class T> auto_init operator%=(const T &t) { return v%=t; }
  auto_init operator++() { return ++v; }
  auto_init operator++(int) { return v++; }
};
#endif
