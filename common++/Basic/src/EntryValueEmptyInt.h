/* $Id: EntryValueEmptyInt.h,v 1.6 2002/06/24 07:35:40 christof Exp $ */
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

#ifndef ENTRYVALUEEMPTYINT_H
#define ENTRYVALUEEMPTYINT_H

#include <Misc/EntryValueBase.h>
#include <Misc/itos.h>

class EntryValueEmptyInt : public EntryValueBase
{
 int intval;
 std::string strval;
 
public:

 EntryValueEmptyInt() : intval(int_NaN) {}
 EntryValueEmptyInt(int v):intval(v)
     { if (v!=0) strval=itos(v); else strval=""; }
   
 virtual int getIntVal() const { return intval;}
 virtual const std::string getStrVal() const { return strval;}
};

class cH_EntryValueEmptyInt : public cH_EntryValue
{
public:
 cH_EntryValueEmptyInt(int v) : cH_EntryValue(new EntryValueEmptyInt(v)) {}
};
  
#endif // ENTRYVALH

