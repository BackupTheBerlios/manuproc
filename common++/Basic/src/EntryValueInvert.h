/* $Id: EntryValueInvert.h,v 1.5 2006/01/10 09:28:51 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG
 *  Copyright (C) 2006 Christof Petig
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
// Invert the sort order of a given Value

class EntryValueInvert : public EntryValueBase
{ cH_EntryValue obj;

public:
 EntryValueInvert(cH_EntryValue const& t) : obj(t) {}
   
 virtual bool operator==(const EntryValueBase &v) const
 { return obj->operator==(v);
 }
 virtual bool operator<(const EntryValueBase &v) const
 { if (obj->operator==(v)) return false;
   return !obj->operator<(v); 
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
  cH_EntryValueInvert(cH_EntryValue const& t) 
  : cH_EntryValue(new EntryValueInvert(t)) {}
};

#endif 
