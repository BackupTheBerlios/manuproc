/* $Id: EntryValueSort.h,v 1.3 2004/01/20 08:04:44 christof Exp $ */
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

#ifndef ENTRYVALUE_DATUM_H
#define ENTRYVALUE_DATUM_H

#include <Misc/EntryValueBase.h>

LIBMPC_BASE_API class EntryValueSort : public EntryValueBase
{cH_EntryValue sort,value;

public:
 EntryValueSort(const cH_EntryValue &s,const cH_EntryValue &v) 
 : sort(s), value(v) {}
   
 virtual bool operator==(const EntryValueBase &v) const;
 virtual bool operator<(const EntryValueBase &v) const;
 virtual int getIntVal() const { return value->getIntVal(); }
 virtual const std::string getStrVal() const { return value->getStrVal();}
};

LIBMPC_BASE_API class EntryValueReverseSort : public EntryValueBase
{cH_EntryValue sort,value;

public:
 EntryValueReverseSort(const cH_EntryValue &s,const cH_EntryValue &v) 
 : sort(s), value(v) {}
   
 virtual bool operator==(const EntryValueBase &v) const;
 virtual bool operator<(const EntryValueBase &v) const;
 virtual int getIntVal() const { return value->getIntVal(); }
 virtual const std::string getStrVal() const { return value->getStrVal();}
};

class cH_EntryValueSort : public cH_EntryValue
{
public:
 cH_EntryValueSort(const cH_EntryValue &s,const cH_EntryValue &v) 
 : cH_EntryValue(new EntryValueSort(s,v)) {}
};

class cH_EntryValueReverseSort : public cH_EntryValue
{
public:
 cH_EntryValueReverseSort(const cH_EntryValue &s,const cH_EntryValue &v) 
 : cH_EntryValue(new EntryValueReverseSort(s,v)) {}
};

#endif 
