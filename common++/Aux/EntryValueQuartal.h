/* $Id: EntryValueQuartal.h,v 1.1 2002/11/22 15:53:52 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef ENTRYVALUE_QUARTAL_H
#define ENTRYVALUE_QUARTAL_H

#include <Misc/EntryValueBase.h>
#include <Misc/Datum.h>
#include <Aux/itos.h>

class EntryValueQuartal : public EntryValueBase
{
 int quartal;
 int jahr;
 
public:
 EntryValueQuartal(const ManuProC::Datum &v) 
 : quartal(v.Quartal()),jahr(v.Jahr()) {}
   
 virtual bool operator==(const EntryValueBase &v) const;
 virtual bool operator<(const EntryValueBase &v) const;
 virtual int getIntVal() const { return jahr*10000+quartal;}
 virtual const std::string getStrVal() const 
 	{ return itos(quartal)+".Q/"+itos(jahr);}
};

class cH_EntryValueQuartal : public cH_EntryValue
{
public:
 cH_EntryValueQuartal(const ManuProC::Datum &v) 
 : cH_EntryValue(new EntryValueQuartal(v)) {}
};

  
#endif 
