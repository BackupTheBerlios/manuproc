/*  pps: ManuProC's ProductionPlanningSystem
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

// $Id:


#ifndef OFFAUF_VALUE
#define OFFAUF_VALUE

#include<Aux/EntryValueIntString.h>

class OffAuf_Value : public EntryValueIntString
{
public:
 OffAuf_Value(int v) : EntryValueIntString(v) {}
 OffAuf_Value(const string &s) : EntryValueIntString(s) {}

};

class cH_OffAuf_Value : public cH_EntryValue
{
public:

 cH_OffAuf_Value() : cH_EntryValue() {}
 cH_OffAuf_Value(const EntryValueBase *r) : cH_EntryValue(r) {}
 cH_OffAuf_Value(int v) : cH_EntryValue(new OffAuf_Value(v)) {}
 cH_OffAuf_Value(const string &s) : cH_EntryValue(new OffAuf_Value(s)) {}
    
};

#endif

 