// $Id: inbetween.h,v 1.2 2003/10/06 10:08:10 christof Exp $
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

#ifndef MPB_INBETWEEN_H
#define MPB_INBETWEEN_H

// SQL semantic: [a;b]
template <class T>
 bool between(const T &v,const T &a, const T&b)
{  return a <= v && v <= b;
}

template <class T>
 bool in(const T &v,const T &a, const T&b)
{  return v==a || v==b;
}
template <class T>
 bool in(const T &v,const T &a, const T&b, const T &c)
{  return v==a || v==b || v==c;
}
template <class T>
 bool in(const T &v,const T &a, const T&b, const T &c, const T &d)
{  return v==a || v==b || v==c || v==d;
}
template <class T>
 bool in(const T &v,const T &a, const T&b, const T &c, const T &d, const T &e)
{  return v==a || v==b || v==c || v==d || v==e;
}
template <class T>
 bool in(const T &v,const T &a, const T&b, const T &c, const T &d, const T &e, const T &f)
{  return v==a || v==b || v==c || v==d || v==e || v==f;
}
template <class T>
 bool in(const T &v,const T &a, const T&b, const T &c, const T &d, const T &e, const T &f, const T &g)
{  return v==a || v==b || v==c || v==d || v==e || v==f || v==g;
}

#endif
