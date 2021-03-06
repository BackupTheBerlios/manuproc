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

#ifndef LIEF_VALUE_H
#define LIEF_VALUE_H

#include<Misc/EntryValueIntString.h>

class Lief_Value : public EntryValueIntString
{
public:
 Lief_Value(int v) : EntryValueIntString(v) {}
 Lief_Value(const std::string &s) : EntryValueIntString(s) {}

};

class cH_Lief_Value : public cH_EntryValue
{
public:

 cH_Lief_Value() : cH_EntryValue() {}
 cH_Lief_Value(const EntryValueBase *r) : cH_EntryValue(r) {}
 cH_Lief_Value(int v) : cH_EntryValue(new Lief_Value(v)) {}
 cH_Lief_Value(const std::string &s) : cH_EntryValue(new Lief_Value(s)) {}
    
};

#endif
