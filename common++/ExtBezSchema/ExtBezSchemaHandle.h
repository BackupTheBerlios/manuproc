// $Id: ExtBezSchemaHandle.h,v 1.1 2001/04/23 08:11:58 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef EXTBEZSCHEMAHANDLE_H
#define EXTBEZSCHEMAHANDLE_H
#include <ExtBezSchema/ExtBezSchema.h>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>

// Handle class to make passing ExtBezSchemas faster and easier
// (java like reference counting)

class cH_ExtBezSchema : public const_Handle<ExtBezSchema>
{	
	// cache
	struct CacheIndex
	{	ExtBezSchema::ID id;
		ArtikelTyp typ;
		
		CacheIndex(const ExtBezSchema::ID &i,const ArtikelTyp &t)
		: id(i), typ(t) {}
		bool operator<(const CacheIndex &b) const
		{  return typ<b.typ || (typ==b.typ && id<b.id); }
	};
	typedef CacheStatic<CacheIndex,cH_ExtBezSchema> cache_t;
	static cache_t cache;
	cH_ExtBezSchema(const ExtBezSchema *s)
		: const_Handle<ExtBezSchema>(s) {}
	friend cache_t::stl_type;
	cH_ExtBezSchema() {} // for the map
public:
	cH_ExtBezSchema(ExtBezSchema::ID eid
		,const ArtikelTyp &t=ExtBezSchema::default_Typ) throw(SQLerror);
};

#endif
