// $Id: TraceNV.h,v 1.2 2005/03/17 10:48:26 christof Exp $
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

// this file provides a ::NV(name,value) shortcut for ManuProC::NameValue

#ifndef CXX_AUX_TRACENV_H
#define CXX_AUX_TRACENV_H

#include <Misc/Trace.h>

template <class T> 
 struct ManuProC::NameValue_s<T> NV(const std::string &n, const T &v)
 { return ManuProC::NameValue_s<T>(n,v); }

// NV1(a+b) gives NV("a+b",a+b)
#define NV1(x) NV(#x,x)

#endif
