/* $Id: EntryValueEmptyInt.h,v 1.10 2003/04/23 09:19:20 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <Misc/EntryValueIntString.h>
#include <Misc/compiler_ports.h>

class EntryValueEmptyInt : public EntryValueIntString
{
 double dval; // absolute nonsense - is this an int or is it a double?

private: // never never use this nonsense, use a EV_fixedpoint
 EntryValueEmptyInt(double d);
 
public:
 EntryValueEmptyInt(int v) : EntryValueIntString(v)
     { if (!v) strval=""; }
};

class cH_EntryValueEmptyInt : public cH_EntryValue
{
public:
 cH_EntryValueEmptyInt(int v) : cH_EntryValue(new EntryValueEmptyInt(v)) {}
};
  
#endif // ENTRYVALH

