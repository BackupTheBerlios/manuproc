/* $Id: Prozess.h,v 1.8 2002/01/22 09:15:55 christof Exp $ */
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

#include"Aux/SQLerror.h"
#include<string>
#include <Aux/CacheStatic.h>
#include <Aux/Handles.h>

class cH_Prozess;

class Prozess : protected HandleContent
{	// friend class ProzessHandle;
	friend class cH_Prozess;
	friend class Handle<const Prozess>;
public:
	enum ID2
	{  None=0, Drucken=1, Faerben=30, Verarbeitung=31, Aequivalenz=38,
		Weben=42, Schaeren=43, Verpacken=44, Giessen=45,
		Rollen_Jumbo=16, Rollen=46 };
	typedef ID2 ID;

	static const ID default_id=None;
        static const ID standard_id=Verarbeitung;

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

#endif
