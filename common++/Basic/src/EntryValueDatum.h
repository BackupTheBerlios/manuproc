/* $Id: EntryValueDatum.h,v 1.7 2002/06/27 07:42:50 christof Exp $ */
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

#ifndef ENTRYVALUE_DATUM_H
#define ENTRYVALUE_DATUM_H

#include <Misc/EntryValueBase.h>
#include <Misc/Datum.h>
#include <Misc/Kalenderwoche.h>

class EntryValueDatum : public EntryValueBase
{
 ManuProC::Datum datum;

public:
 EntryValueDatum(const ManuProC::Datum &v) 
 : datum(v) {}
   
 virtual bool operator==(const EntryValueBase &v) const;
 virtual bool operator<(const EntryValueBase &v) const;
 const ManuProC::Datum &Datum() const { return datum; }
 virtual int getIntVal() const { return int_NaN;}
 virtual const std::string getStrVal() const { return datum.valid()?datum.c_str():"";}
};

class cH_EntryValueDatum : public cH_EntryValue
{
public:
 cH_EntryValueDatum(const ManuProC::Datum &v) 
 : cH_EntryValue(new EntryValueDatum(v)) {}
};

class EntryValueKalenderwoche : public EntryValueBase
{
 Kalenderwoche datum;

public:
 EntryValueKalenderwoche(const Kalenderwoche &v) 
 : datum(v) {}
   
 virtual bool operator==(const EntryValueBase &v) const;
 virtual bool operator<(const EntryValueBase &v) const;
 const Kalenderwoche &KW() const { return datum; }
 virtual int getIntVal() const { return int_NaN;}
 virtual const std::string getStrVal() const ;
};

class cH_EntryValueKalenderwoche : public cH_EntryValue
{
public:
 cH_EntryValueKalenderwoche(const Kalenderwoche &v) 
 : cH_EntryValue(new EntryValueKalenderwoche(v)) {}
};


  
#endif 
