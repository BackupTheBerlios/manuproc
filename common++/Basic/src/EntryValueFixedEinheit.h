/* $Id: EntryValueFixedEinheit.h,v 1.2 2002/11/13 08:13:07 christof Exp $ */
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

#ifndef ENTRYVALUE_FIXEDEINHEIT_H
#define ENTRYVALUE_FIXEDEINHEIT_H

#include <Misc/EntryValueBase.h>
#include <Misc/fixedpoint.h>
#include <Misc/Ausgabe_neu.h>
#include <typeinfo> // for bad_cast

template <int decimals=2,class Ftype=double,class Itype=long> 
	class EntryValueFixedEinheit : public EntryValueBase
{
 fixedpoint<decimals,Ftype,Itype> wert;
 std::string einheit;

public:
 EntryValueFixedEinheit(const fixedpoint<decimals,Ftype,Itype> &v, const std::string &e) 
 : wert(v), einheit(e) {}
   
 virtual bool operator==(const EntryValueBase &v) const
 {  try
   {  const EntryValueFixedEinheit<decimals,Ftype,Itype> &b
   		=dynamic_cast<const EntryValueFixedEinheit<decimals,Ftype,Itype> &>(v);
      return einheit==b.einheit && wert==b.wert;
   } catch (std::bad_cast &e)
   {  return false;
   }
 }
 virtual bool operator<(const EntryValueBase &v) const
 { try
   {  const EntryValueFixedEinheit<decimals,Ftype,Itype> &b
   		=dynamic_cast<const EntryValueFixedEinheit<decimals,Ftype,Itype> &>(v);
      return wert<b.wert || einheit<b.einheit;
   } catch (std::bad_cast &e)
   {  return getIntVal()<v.getIntVal();
   }
 }

 const fixedpoint<decimals,Ftype,Itype> &Wert() const { return wert; }
 virtual int getIntVal() const { return wert.as_int();}
 virtual const std::string getStrVal() const 
 {  if (!wert) return "";
    return Formatiere(wert)+einheit; 
 }
};

template <int decimals=2,class Ftype=double,class Itype=long> 
   class cH_EntryValueFixedEinheit : public cH_EntryValue
{
public:
 cH_EntryValueFixedEinheit(const fixedpoint<decimals,Ftype,Itype> &v, const std::string &e) 
 : cH_EntryValue(new EntryValueFixedEinheit<decimals,Ftype,Itype>(v,e)) {}
};
  
#endif 
