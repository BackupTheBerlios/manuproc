/* $Id: EntryValueIntGermanString.h,v 1.2 2004/01/20 08:04:44 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef ENTRYVALUEINTGERMANSTRING_H
#define ENTRYVALUEINTGERMANSTRING_H

#include <Misc/EntryValueIntString.h>

LIBMPC_BASE_API class EntryValueIntGermanString : public EntryValueIntString
{

public:

 EntryValueIntGermanString() {}
 EntryValueIntGermanString(int v) : EntryValueIntString(v) {}
 EntryValueIntGermanString(const std::string &s) : EntryValueIntString(s) {}

 virtual bool operator<(const EntryValueBase &v) const;
   
};

class cH_EntryValueIntGermanString : public cH_EntryValue
{
public:
 cH_EntryValueIntGermanString() : cH_EntryValue(new EntryValueIntGermanString()) {}
 cH_EntryValueIntGermanString(int v) : cH_EntryValue(new EntryValueIntGermanString(v)) {}
 cH_EntryValueIntGermanString(const std::string &s) : cH_EntryValue(new EntryValueIntGermanString(s)) {}
};
  
#endif // ENTRYVALH

