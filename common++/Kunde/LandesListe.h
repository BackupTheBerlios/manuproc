// $Id: LandesListe.h,v 1.8 2003/04/24 13:44:37 jacek Exp $
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

#include <Misc/SQLerror.h>
#include <Misc/Handles.h>
#include <Misc/CacheStatic.h>
#include <BaseObjects/ManuProcHandle.h>


class LandesListe : public ManuProcHandle<>
{
private:
 std::string name,lkz;
 bool eu;

public:
  LandesListe(ID id) throw(SQLerror);
  LandesListe() : eu(false) {}
  ID Id() const { return entityid; } 
  const std::string Name() const { return name; }
  const std::string LKZ() const { return lkz; }
  bool Auslaender() const { return (lkz!="D"); }
  bool EU() const { return eu; }

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
