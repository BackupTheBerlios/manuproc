/* $Id: EntryValueIntString.h,v 1.6 2001/12/19 11:02:08 christof Exp $ */
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

#ifndef ENTRYVALUEINTSTRING_H
#define ENTRYVALUEINTSTRING_H

#include <Aux/EntryValueBase.h>

class EntryValueIntString : public EntryValueBase
{
 int intval;
 std::string strval;

public:

 EntryValueIntString() : intval(int_NaN), strval("-")
   {}
 EntryValueIntString(int v);
 EntryValueIntString(const std::string &s);
   
 virtual int getIntVal() const { return intval;}
 virtual const std::string getStrVal() const { return strval;}
 virtual bool getBoolVal() const { return strval=="t";}
};

class cH_EntryValueIntString : public cH_EntryValue
{
public:
 cH_EntryValueIntString() : cH_EntryValue(new EntryValueIntString()) {}
 cH_EntryValueIntString(int v) : cH_EntryValue(new EntryValueIntString(v)) {}
 cH_EntryValueIntString(const std::string &s) : cH_EntryValue(new EntryValueIntString(s)) {}
};
  
#endif // ENTRYVALH

