/* $Id: EntryValueFixed.h,v 1.3 2002/05/06 13:41:22 christof Exp $ */
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

#ifndef ENTRYVALUE_FIXED_H
#define ENTRYVALUE_FIXED_H

#include <Aux/EntryValueBase.h>
#include <Aux/fixedpoint.h>
#include <Aux/Ausgabe_neu.h>
#include <typeinfo> // for bad_cast

template <int decimals=2,class Ftype=double,class Itype=long,bool shorten=true> 
	class EntryValueFixed : public EntryValueBase
{
 fixedpoint<decimals,Ftype,Itype> wert;

public:
 EntryValueFixed(const fixedpoint<decimals,Ftype,Itype> &v) 
 : wert(v) {}
   
 virtual bool operator==(const EntryValueBase &v) const
 {  try
   {  const EntryValueFixed<decimals,Ftype,Itype> &b
   		=dynamic_cast<const EntryValueFixed<decimals,Ftype,Itype,shorten> &>(v);
      return wert==b.wert;
   } catch (std::bad_cast &e)
   {  return false;
   }
 }
 virtual bool operator<(const EntryValueBase &v) const
 { try
   {  const EntryValueFixed<decimals,Ftype,Itype> &b
   		=dynamic_cast<const EntryValueFixed<decimals,Ftype,Itype,shorten> &>(v);
      return wert<b.wert;
   } catch (std::bad_cast &e)
   {  return getIntVal()<v.getIntVal();
   }
 }

 const fixedpoint<decimals,Ftype,Itype> &Wert() const { return wert; }
 virtual int getIntVal() const { return int(wert);}
 virtual const std::string getStrVal() const 
 { if (shorten) return Formatiere_short(wert);
   else return Formatiere(wert); 
 }
};

template <int decimals=2,class Ftype=double,class Itype=long,bool shorten=true> 
   class cH_EntryValueFixed : public cH_EntryValue
{
public:
 cH_EntryValueFixed(const fixedpoint<decimals,Ftype,Itype> &v) 
 : cH_EntryValue(new EntryValueFixed<decimals,Ftype,Itype,shorten>(v)) {}
};
  
#endif 
