/* $Id: EntryValueTimeStamp.h,v 1.1 2003/03/19 08:27:03 christof Exp $ */
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

#ifndef ENTRYVALUE_TIMESTAMP_H
#define ENTRYVALUE_TIMESTAMP_H

#include <Misc/EntryValueBase.h>
#include <Misc/TimeStamp.h>

class EntryValueTimeStamp : public EntryValueBase
{
 ManuProC::TimeStamp ts;

public:
 EntryValueTimeStamp(const ManuProC::TimeStamp &v) 
 : ts(v) {}
   
 virtual bool operator==(const EntryValueBase &v) const;
 virtual bool operator<(const EntryValueBase &v) const;
 const ManuProC::TimeStamp &TimeStamp() const { return ts; }
 virtual int getIntVal() const { return int_NaN;}
 virtual const std::string getStrVal() const;
};

class cH_EntryValueTimeStamp : public cH_EntryValue
{
public:
 cH_EntryValueTimeStamp(const ManuProC::TimeStamp &v) 
 : cH_EntryValue(new EntryValueTimeStamp(v)) {}
};

#endif 
