// $Id: Lieferart.h,v 1.4 2006/06/26 07:53:02 christof Exp $
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


#ifndef KUNDE_LIEFERART_HH
#define KUNDE_LIEFERART_HH
#include <string>
#include <BaseObjects/ManuProcHandle.h>
#include <Misc/SQLerror.h>
#include <Misc/Datum.h>
#include <Misc/multi_lang.h>
#include <Misc/compiler_ports.h>
#include <Misc/Query.h>
#include <vector>
#include <Misc/CacheStatic.h>

class cH_Kunde;

class Lieferart : public ManuProcHandle<>
{
private:
    std::string bezeichnung;
    int textid;

    friend Query::Row &NOTGCC295(::)operator>>(Query::Row &is, Lieferart &za);
    
public:
    Lieferart(ID _id) throw (SQLerror) ;
    
    ID Id() const {  return entityid; }
    const std::string Bezeichnung() const {return bezeichnung;}
    int textId() const { return textid; }
    
    void TeX_out(std::ostream &os,
		const cH_Kunde k,
		MultiL_Dict &mld) const;

    bool operator==(const Lieferart& b) const
              {return Id()==b.Id();} 
    bool operator<(const Lieferart& b) const
                {return Id()<b.Id();} 		
};

class cH_Lieferart : public Handle<const Lieferart>
{	// cache
        typedef CacheStatic<Lieferart::ID,cH_Lieferart> cache_t;
        static cache_t cache;
        cH_Lieferart(const Lieferart *p) : Handle<const Lieferart>(p) {}

	friend class std::map<Lieferart::ID,cH_Lieferart>;
        cH_Lieferart() {}
public:
	typedef Lieferart::ID ID;
	cH_Lieferart(ID nr);
};

#endif
