// $Id: PreisListe.h,v 1.12 2003/09/15 11:20:21 jacek Exp $
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


#ifndef KUNDE_PREISLISTE_HH
#define KUNDE_PREISLISTE_HH

#include<Misc/SQLerror.h>
#include <Misc/Handles.h>
#include <Misc/CacheStatic.h>
#include <BaseObjects/ManuProcHandle.h>
#include <Misc/Waehrung.h>
#include <Misc/fixedpoint.h>

class PreisListe : public ManuProcHandle<>
{
public:
 typedef fixedpoint<2> rabatt_t;
 typedef enum { PL_UNKNOWN='U', PL_EINKAUF='E', PL_VERKAUF='V'} PlArt;
private:
 std::string name;
 cP_Waehrung waehrung;
 mutable bool rabatt_gesetzt;
 mutable rabatt_t rabatt;
 PlArt art;

public:
  PreisListe(ID id) throw(SQLerror);
  PreisListe() : rabatt_gesetzt(false),rabatt(0),art(PL_UNKNOWN)  {}
  ID Id() const { return entityid; } 
  const std::string Name() const { return name; }
  static ID createPreisliste(const std::string &name) throw(SQLerror);
  bool festerRabatt() const { return rabatt_gesetzt; }
  const rabatt_t getRabatt() const { return rabatt;}
  const cP_Waehrung getWaehrung() const { return waehrung; }
  void setRabatt(bool fest, rabatt_t rab) throw(SQLerror);
  PlArt Art() const { return art; }
};


class cH_PreisListe : public Handle<const PreisListe>
{	
  typedef CacheStatic<int,cH_PreisListe> cache_t;
  static cache_t cache;
  friend class std::map<int,cH_PreisListe>;
  static PreisListe default_object;
  static bool cH_PreisListe::default_object_ref;
public:
  cH_PreisListe() : Handle<const PreisListe>(&default_object) {}
  cH_PreisListe(const PreisListe *p) : Handle<const PreisListe>(p) {}
  cH_PreisListe(PreisListe::ID id);
};

#endif
