/* $Id: EntryValueBase.h,v 1.15 2004/01/20 08:04:44 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef ENTRY_VALUEBASE_H
#define ENTRY_VALUEBASE_H
#include <Misc/Handles.h>
#include <string>
#include <Misc/compiler_ports.h>
#include <Misc/libManuProC_Base_dll.h>

class EntryValueBase : public HandleContent
// opaque Data Type which is accessible as an int value or as a string
{
public:
 static LIBMPC_BASE_API const int int_NaN=-1;
 static LIBMPC_BASE_API const double double_NaN=-1;
 virtual operator const std::string() const { return getStrVal(); }
 virtual const std::string getStrVal() const { return std::string(); }
 virtual bool operator==(const EntryValueBase &v) const;
 virtual bool operator<(const EntryValueBase &v) const;
 ~EntryValueBase() {}

// CP: I don't like these functions here, they should get replaced by the 
// correct dynamic_cast construction, we can't add a function here for every 
// type we define
// a string and operator< are needed for proper display, these are not needed

 __deprecated virtual double getDoubleVal() const { return double_NaN; }
 __deprecated virtual bool getBoolVal() const { return false; }
 __deprecated virtual operator double() const { return getDoubleVal(); }
 __deprecated virtual operator bool() const;

// CP: I'm not happy with these either 
// but they offered nice sorting fallback in the past
 /*__deprecated*/ virtual operator int() const { return getIntVal(); }
 /*__deprecated*/ virtual int getIntVal() const { return int_NaN; }
};

class cH_EntryValue : public Handle<const EntryValueBase>
{protected:
// cH_EntryValue() {}
 cH_EntryValue(const EntryValueBase *r) : Handle<const EntryValueBase>(r){}
public:
 cH_EntryValue() 
 : Handle<const EntryValueBase>(new EntryValueBase())
 {}
 ~cH_EntryValue() {}
};

#endif // ENTRYVALH
