/* $Id: EntryValueIntString.h,v 1.4 2001/01/30 20:44:47 cvs_christof Exp $ */
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
 string strval;

public:

 EntryValueIntString() : intval(int_NaN), strval("-")
   {}
 EntryValueIntString(int v);
 EntryValueIntString(const string &s);
   
 operator const string() const { return strval;}
 operator int() const { return intval;}
 
 int getIntVal() const { return intval;}
 const string getStrVal() const { return strval;}
};

#endif // ENTRYVALH

