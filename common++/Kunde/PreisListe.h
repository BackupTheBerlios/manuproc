// $Id: PreisListe.h,v 1.1 2002/03/20 07:46:26 christof Exp $
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

#include<Aux/SQLerror.h>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>


class PreisListe : public HandleContent
{
public:
     typedef long int ID;
	
private:
 ID id;	
 std::string name;
 static const ID _illegal=-1; 

public:
  static const ID none_id=_illegal;
  PreisListe(ID id) throw(SQLerror);
  PreisListe() : id(_illegal) {}
  ID Id() const { return id; } 
  const std::string Name() const { return name; }
  static void createPreisliste(unsigned int nr,const std::string &name) throw(SQLerror);
  static unsigned int nextval() throw(SQLerror);
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
