// $Id: ArtikelBezeichnung.h,v 1.2 2001/07/05 09:23:02 christof Exp $
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


#ifndef ARTIKELBEZEICHNUNG_H
#define ARTIKELBEZEICHNUNG_H

#include <Artikel/ArtikelBase.h>
#include <Aux/EntryValue.h>
#include <Kunde/Kunde.h>
#include <vector>
#include <string>
#include <ExtBezSchema/ExtBezSchemaHandle.h>

class cH_ArtikelBezeichnung;

class ArtikelBezeichnung : public virtual ArtikelBase, protected HandleContent
{	friend class const_Handle<ArtikelBezeichnung>;
	friend class cH_ArtikelBezeichnung;
public:
	typedef std::vector<EntryValue>::const_iterator const_iterator;
	struct dont_throw { dont_throw() {} };

private:
 	cH_ExtBezSchema schema;
	std::vector<EntryValue> value;

// ich würde operator[] bevorzugen CP
 const EntryValue getExtBezFeld(int feld) const throw(SQLerror);
 

public:
// deprecated
 ArtikelBezeichnung(const ArtikelBase &artikel,const cH_ExtBezSchema &schema) throw(SQLerror);
// use this one 
 ArtikelBezeichnung(const ArtikelBase &artikel,
 	const ExtBezSchema::ID schema=ExtBezSchema::default_id) throw(SQLerror);
 ArtikelBezeichnung(const ArtikelBase &artikel,ExtBezSchema::ID id,dont_throw dummy) throw();
 ArtikelBezeichnung(const ArtikelBase &artikel,dont_throw dummy) throw()
        : schema(1)
 { *this=ArtikelBezeichnung(artikel,ExtBezSchema::default_id,dummy); }
 ArtikelBezeichnung() : schema(ExtBezSchema::default_id) {}

/// Artikel zu der externen Bezeichnung für einen Kunden erzeugen 
/// (ID nach Bezeichnung ermitteln)
/// Vorsicht: Der Artikeltyp muss stimmen!
 ArtikelBezeichnung(int signifikanz, const std::vector<EntryValue> &values, const cH_ExtBezSchema &schema) throw(SQLerror);

 std::string Bezeichnung(int signifikanz=1, char separator=0,int felder=-1) const throw();
 
 void setzeExtBezSchema(const cH_ExtBezSchema &schema) throw(SQLerror);
 const cH_ExtBezSchema getExtBezSchema() const throw()
 {  return schema;
 }

 const_iterator begin() const
 {  const_iterator i(value.begin());
    if (i!=value.end()) return ++i;
    return i;
 }
 const_iterator end() const
 {  return value.end();
 }
 size_t size() const
 {  size_t ret(value.size());
    if (ret) return ret-1;
    return ret;
 }
 // Zuweisung nicht sinnvoll !?
 const EntryValue operator[](int feld) const throw(SQLerror)
 {  return getExtBezFeld(feld); }
 
 void setID(const ArtikelBase::ID &id) throw(SQLerror)
 {  *this=ArtikelBezeichnung(id,schema); }
};

class cH_ArtikelBezeichnung : public const_Handle<ArtikelBezeichnung>
{
public:
	typedef ArtikelBezeichnung::dont_throw dont_throw;
private:
	// cache
	struct cache_key
	{  ExtBezSchema::ID sid;
	   ArtikelBase::ID aid;
	   
	   cache_key(const ExtBezSchema::ID &s,const ArtikelBase::ID &a)
	   : sid(s), aid(a) {}
	   bool operator<(const cache_key &b) const
	   {  return sid<b.sid || (sid==b.sid && aid<b.aid); }
	};
	typedef CacheStatic<cache_key,cH_ArtikelBezeichnung> cache_t;
	static cache_t cache;
	
	cH_ArtikelBezeichnung(const ArtikelBezeichnung *b)
		: const_Handle<ArtikelBezeichnung>(b) {}
//	friend cache_t::stl_type;
	friend class std::map<cache_key, cH_ArtikelBezeichnung>;
	cH_ArtikelBezeichnung() {}

public:
	// please use variant 2
	cH_ArtikelBezeichnung(const ArtikelBase &artikel,const cH_ExtBezSchema &schema) throw(SQLerror);
	// recommended variant!
	cH_ArtikelBezeichnung(const ArtikelBase &artikel,const ExtBezSchema::ID &id) throw(SQLerror);
	// use this if you do not want to handle errors
	cH_ArtikelBezeichnung(const ArtikelBase &artikel,const ExtBezSchema::ID &id,dont_throw dummy) throw();
 	cH_ArtikelBezeichnung(const ArtikelBase &artikel,dont_throw dummy) throw()
	{ *this=cH_ArtikelBezeichnung(artikel,ExtBezSchema::default_id,dummy); }

/// Artikel zu der externen Bezeichnung für einen Kunden erzeugen 
/// (ID nach Bezeichnung ermitteln)
	cH_ArtikelBezeichnung(int signifikanz, const std::vector<EntryValue> &values, const cH_ExtBezSchema &schema) throw(SQLerror)
		: const_Handle<ArtikelBezeichnung>(new ArtikelBezeichnung(signifikanz,values,schema))
	{}
/// default ctor
	class Default {};
	// call it like: cH_ArtikelBezeichnung(cH_ArtikelBezeichnung::Default());
	cH_ArtikelBezeichnung(const Default &d) 
		: const_Handle<ArtikelBezeichnung>(new ArtikelBezeichnung())
	{}
};

#endif
