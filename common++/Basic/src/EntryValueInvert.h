/* $Id: EntryValueInvert.h,v 1.4 2004/02/10 10:51:54 christof Exp $ */
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

#ifndef ENTRYVALUE_INVERT_H
#define ENTRYVALUE_INVERT_H

#include <Misc/EntryValueBase.h>

template <typename T>
 class EntryValueInvert : public EntryValueBase
{Handle<const T> obj;

public:
 EntryValueInvert(const Handle<const T> &t) : obj(t) {}
   
 virtual bool operator==(const EntryValueBase &v) const
 {  return obj->operator==(v); 
 }
 virtual bool operator<(const EntryValueBase &v) const
 {  return v.operator(*obj);
// if(obj->operator==(v)) return false;
// return !obj->operator<(v); 
 }
 virtual int getIntVal() const 
 { return obj->getIntVal(); 
 }
 virtual const std::string getStrVal() const 
 { return obj->getStrVal();
 }
};

struct cH_EntryValueInvert : public cH_EntryValue
{
 template <typename T>
  cH_EntryValueInvert(const Handle<const T> &t) 
 : cH_EntryValue(new EntryValueInvert<T>(t)) {}
 template <typename T>
  cH_EntryValueInvert(const T * const t) 
 : cH_EntryValue(new EntryValueInvert<T>(t)) {}
};

#endif 
