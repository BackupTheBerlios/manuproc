/* $Id: EntryValueBool.h,v 1.1 2002/11/22 15:53:52 christof Exp $ */
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

#ifndef ENTRYVALUEBOOL_H
#define ENTRYVALUEBOOL_H

#include <Misc/EntryValueBase.h>
#include <Misc/itos.h>

class EntryValueBool : public EntryValueBase
{
 bool val;
 
public:

 EntryValueBool() : val(false) {}
 EntryValueBool(bool v):val(v) {}
   
 virtual int getIntVal() const { return int_NaN;}
 virtual const std::string getStrVal() const 
   { if(val) return "Ja"; else return "Nein";}
};

class cH_EntryValueBool : public cH_EntryValue
{
public:
 cH_EntryValueBool(bool v) : cH_EntryValue(new EntryValueBool(v)) {}
};
  
#endif // ENTRYVALH

