// $Id: ppsInstanz.h,v 1.15 2001/10/05 15:42:00 christof Exp $
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

#ifndef PPSINSTANZCLASS
#define PPSINSTANZCLASS
#include <string>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>


class ppsInstanz : public HandleContent
{
public:
 enum ppsInstId {INST_NONE,INST_KNDAUF=1,INST_FAERB,INST_DRUCK,INST_WEB,
        INST_SCHAER,INST_SPRITZ,INST_GARNEINKAUF,INST_BANDLAGER,
        INST_ROHLAGER};
 
private: 
 ppsInstId instid; 
 std::string name;
 
 void get_name();
 
public:
 typedef ppsInstId ID;
 static const ID default_id=INST_NONE;
 
 ppsInstanz(ppsInstId iid) : instid(iid) {get_name();}
 ppsInstanz() : instid(INST_NONE) {}

// absolet void set_Instanz(ppsInstId iid) {instid=iid; get_name();}
 ppsInstId Id() const { return instid; }
 std::string get_Name() const {return name;}
 
 operator ppsInstId () const {return instid;}
 void set(ppsInstId i) {instid=i;}
  
};

class cH_ppsInstanz : public Handle<const ppsInstanz>
{
     typedef CacheStatic<ppsInstanz::ID,cH_ppsInstanz> cache_t;
     static cache_t cache;
     cH_ppsInstanz(const ppsInstanz *s) : Handle<const ppsInstanz>(s) {};
     friend class std::map<ppsInstanz::ID,cH_ppsInstanz>;
     cH_ppsInstanz(){};
  public:
     cH_ppsInstanz(ppsInstanz::ID iid);

     bool operator==(const cH_ppsInstanz &b) const {return (*this)->Id()==b->Id();}
};

#endif
