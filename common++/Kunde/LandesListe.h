// $Id: LandesListe.h,v 1.2 2002/05/06 13:41:23 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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


#ifndef KUNDE_LANDESLISTE_HH
#define KUNDE_LANDESLISTE_HH

#include <Aux/SQLerror.h>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>
#include <BaseObjects/ManuProcEntity.h>


class LandesListe : public ManuProcEntity
{
private:
 std::string name,lkz;

public:
  LandesListe(ID id) throw(SQLerror);
  LandesListe() {}
  ID Id() const { return entityid; } 
  const std::string Name() const { return name; }
  const std::string LKZ() const { return lkz; }

};


class cH_LandesListe : public Handle<const LandesListe>
{	
  typedef CacheStatic<int,cH_LandesListe> cache_t;
  static cache_t cache;
  friend class std::map<int,cH_LandesListe>;
  static LandesListe default_object;
  static bool cH_LandesListe::default_object_ref;
public:
  cH_LandesListe() : Handle<const LandesListe>(&default_object) {}
  cH_LandesListe(const LandesListe *p) : Handle<const LandesListe>(p) {}
  cH_LandesListe(LandesListe::ID id);
};


#endif
