/* $Id: EntryValueEmptyInt.h,v 1.9 2002/10/24 14:06:49 thoma Exp $ */
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
 double dval;
 std::string strval;
 
public:

 EntryValueEmptyInt() : intval(int_NaN), dval(double_NaN) {}
 EntryValueEmptyInt(int v):intval(v), dval(double_NaN)
     { if (v!=0) strval=itos(v); else strval=""; }
 EntryValueEmptyInt(double v):intval(int_NaN), dval(v)
     { if (v!=0) strval=dtos(v); else strval=""; }
   
 virtual int getIntVal() const { return intval;}
 virtual double getDoubleVal() const { return dval;}
 virtual const std::string getStrVal() const { return strval;}
};

class cH_EntryValueEmptyInt : public cH_EntryValue
{
public:
 cH_EntryValueEmptyInt(int v) : cH_EntryValue(new EntryValueEmptyInt(v)) {}
};
  
#endif // ENTRYVALH

