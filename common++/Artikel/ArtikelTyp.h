// $Id: ArtikelTyp.h,v 1.1 2001/04/23 08:11:58 christof Exp $
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

#ifndef ARTIKELTYP_H
#define ARTIKELTYP_H

#include <Artikel/ArtikelBase.h>
#include <Aux/CacheStatic.h>

/* quick hack to support Types really fast,
   this should be replaced by a decent generic class/scheme */

class ArtikelTyp
{
public:
	enum typ 
	{ GewebtesBand, GefaerbtesBand, BedrucktesBand, AufgemachtesBand,
	  Garn, Seitenscheiben, Verpackungsmaterial, Sonstiges };
private:
	typ t;
	
	typedef CacheStatic<ArtikelBase::ID,typ> cache_t;
	static cache_t cache;
public:
	ArtikelTyp(typ a) : t(a) {}
	ArtikelTyp(int a) : t((typ)a) {}
	operator typ() const { return t; }
	operator int() const { return int(t); }
	// warum nicht der default operator? CP 2001-3-1
	const ArtikelTyp &operator=(const ArtikelTyp &b) { t=b.t; return *this; }
	
	static ArtikelTyp vonArtikel(const ArtikelBase &ab);
};
#endif
