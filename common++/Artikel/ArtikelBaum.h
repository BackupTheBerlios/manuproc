/* $Id: ArtikelBaum.h,v 1.1 2001/04/23 08:11:58 christof Exp $ */
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

#ifndef ARTIKELBAUM_H
#define ARTIKELBAUM_H

#include"Artikel/Prozess.h"
#include<string>
#include<vector>
#include"Aux/SQLerror.h"
#include <Artikel/ArtikelBase.h>

class ArtikelBaum : public virtual ArtikelBase
{
private:
 ID altartikel;

protected:
 cH_Prozess prozess;
 
 	// cache
	struct cache_payload
	{  ID altartikel;
	   Prozess::ID pid;
	   
	   cache_payload(const ID &i,const Prozess::ID &p)
	   : altartikel(i), pid(p) {}
	   cache_payload() : altartikel(0), pid(0) {}
	};
	typedef CacheStatic<ID,cache_payload> cache_t;
	static cache_t cache;

public:
 ArtikelBaum()
  : prozess(Prozess::default_id)
 {}
 ArtikelBaum(const ArtikelBase &stamp);
 ArtikelBaum(const ID &stamp,const ID &_altartikel,cH_Prozess proz)
  : ArtikelBase(stamp), altartikel(_altartikel), prozess(proz)
 {}

 void setID(const ID &id) throw(SQLerror);

 const ID getRohArtid() const
 {  return RohArtikelID(); }
 const ID RohArtikelID() const;

public:
 const cH_Prozess &getErzeugendenProzess() const
 {  return prozess; }
 const ID &ParentArtikelID() const
 {  return altartikel; }
 
 /// gibt die Stückgröße des Artikels zurück (Meter/Stück) oder 0
 float Stueckgroesse() const;
};

#endif
