// $Id: Anrede.h,v 1.4 2002/05/09 12:46:00 christof Exp $
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


#ifndef KUNDE_ANREDE_HH
#define KUNDE_ANREDE_HH

#include<Aux/SQLerror.h>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>
#include <BaseObjects/ManuProcEntity.h>

class Anrede : public ManuProcEntity
{
private:
 std::string anrede;
 std::string branrede;

public:
 Anrede(ID id) throw(SQLerror);
 Anrede() : ManuProcEntity(none_id) {}
 ID Id() const { return entityid; } 
// const std::string Name() const { return name; }
 const std::string getAnrede() const { return anrede; }
 const std::string getBrAnrede() const { return branrede; }
};


class cH_Anrede : public Handle<const Anrede>
{	
  typedef CacheStatic<int,cH_Anrede> cache_t;
  static cache_t cache;
  friend class std::map<int,cH_Anrede>;
  static Anrede default_object;
  static bool cH_Anrede::default_object_ref;
public:
  cH_Anrede() : Handle<const Anrede>(&default_object) {}
  cH_Anrede(const Anrede *p) : Handle<const Anrede>(p) {}
  cH_Anrede(Anrede::ID id);
};


#endif
