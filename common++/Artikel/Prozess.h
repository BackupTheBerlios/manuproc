/* $Id: Prozess.h,v 1.16 2003/01/29 13:56:00 christof Exp $ */
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

#ifndef PROZESS_H
#define PROZESS_H

#include<Misc/SQLerror.h>
#include<string>
#include <Misc/CacheStatic.h>
#include <Misc/Handles.h>
#include <DynamicEnums/DynamicEnums.h>
#include <DynamicEnums/DefaultValues.h>

class cH_Prozess;

namespace ProzessID = ManuProC::DynamicEnums::Prozesse;

class Prozess : public HandleContent
{	// friend class ProzessHandle;
	friend class cH_Prozess;
	friend class Handle<const Prozess>;
public:
	typedef ManuProC::DynamicEnums::Prozesse::enum_t ID;

	static const ID default_id=ProzessID::None;
   static const ID standard_id=ManuProC::DefaultValues::Prozess;

private:
 ID prozessid;
 float meterprostk;
 std::string label;
 std::string text;

public:
 Prozess(ID pid) throw(SQLerror);
 Prozess() : prozessid(default_id), meterprostk(0)
 {}
 // wird diese Routine noch verwendet? CP 2000-08-16
 // wurde in ProzessBox verwendet - jetzt nicht mehr
private: // wir werden sehen ;-)
 Prozess(ID pid,float _meterprostk,const std::string typ,const std::string _text)
 	: prozessid(pid), meterprostk(_meterprostk), label(typ), text(_text)
 {}
public:
 float getMtrProStk() const { return meterprostk;}
 const std::string getTyp() const { return label; }
 const std::string getText() const { return text; }
 ID getProzessID() const { return prozessid; }
 // I prefer this name, CP
 ID Id() const { return prozessid; }

 bool operator==(const Prozess& b) const {return Id()==b.Id();}
 bool operator!=(const Prozess& b) const {return Id()!=b.Id();}
 bool operator==(const ID b) const {return Id()==b;}
 bool operator!=(const ID b) const {return Id()!=b;}
};

class cH_Prozess : public Handle<const Prozess>
{
private:
	// cache
	typedef CacheStatic<Prozess::ID,cH_Prozess> cache_t;
	static cache_t cache;
	cH_Prozess(const Prozess *p) : Handle<const Prozess>(p) {}
//	friend cache_t::stl_type;
	friend class std::map<Prozess::ID, cH_Prozess>;
	cH_Prozess() {}
public:
	static const Prozess::ID default_pid=Prozess::default_id;
	cH_Prozess(Prozess::ID pid);
};

class FetchIStream;
FetchIStream& operator>>(FetchIStream&,cH_Prozess&);
FetchIStream &operator>>(FetchIStream &is, Prozess::ID &v);

#endif
