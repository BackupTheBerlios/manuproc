/* $Id: MyValue.h,v 1.4 2002/07/05 12:36:56 christof Exp $ */
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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



#ifndef KOMPONENTENEXAMP_MYVALUE
#define KOMPONENTENEXAMP_MYVALUE

#include<Misc/EntryValueIntString.h>

class MyValue : public EntryValueIntString
{

public:
 MyValue(int v) : EntryValueIntString(v) {}
 MyValue(const std::string &s) : EntryValueIntString(s) {}


};

class cH_MyValue : public cH_EntryValue
{
public:

 cH_MyValue() : cH_EntryValue() {}
 cH_MyValue(const EntryValueBase *r) : cH_EntryValue(r) {}
 cH_MyValue(int v) : cH_EntryValue(new MyValue(v)) {}
 cH_MyValue(const std::string &s) : cH_EntryValue(new MyValue(s)) {}

};


#endif

