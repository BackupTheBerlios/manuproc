/* $Id: EntryValueBase.h,v 1.5 2001/10/01 12:55:40 christof Exp $ */
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
#include <Aux/Handles.h>
#include <string>

class EntryValueBase : public HandleContent
// opaque Data Type which is accessible as an int value or as a string
{
public:
 static const int int_NaN=-1;
 virtual operator const std::string() const { return getStrVal(); }
 virtual operator int() const { return getIntVal(); }
 virtual operator bool() const;
 
 virtual int getIntVal() const { return 0; }
 virtual const std::string getStrVal() const { return "?"; }
 virtual bool operator==(const EntryValueBase &v) const;
 virtual bool operator<(const EntryValueBase &v) const;
 ~EntryValueBase() {}
};

class cH_EntryValue : public const_Handle<EntryValueBase>
{protected:
// cH_EntryValue() {}
 cH_EntryValue(const EntryValueBase *r) : const_Handle<EntryValueBase>(r){}
public:
 cH_EntryValue() 
 : const_Handle<EntryValueBase>(new EntryValueBase())
 {}
 ~cH_EntryValue() {}
};

#endif // ENTRYVALH
