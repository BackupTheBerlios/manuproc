// $Id: ArtikelStamm.h,v 1.1 2001/08/20 08:27:13 christof Exp $
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

#ifndef ARTIKELSTAMM_H
#define ARTIKELSTAMM_H

#include <Artikel/ArtikelBase.h>
#include <Artikel/ArtikelTyp.h>
#include <Aux/CacheStatic.h>

class ArtikelStamm
{	
	struct payload_t
	{  ArtikelTyp::typ typ;
	   int bestellen_bei;
	   int defaultschema;
	   
	   payload_t() : typ((ArtikelTyp::typ)0),  bestellen_bei(1), defaultschema(1) {}
	};
	
	ArtikelBase art;
	payload_t payload;
	
	typedef CacheStatic<ArtikelBase::ID,payload_t> cache_t;
	static cache_t cache;
public:
	ArtikelStamm(const ArtikelBase &ab) throw (SQLerror);
	ArtikelTyp::typ Warengruppe() const
	{  return payload.typ; }
	int BestellenBei() const
	{  return payload.bestellen_bei; }
	int defaultSchema() const
	{  return payload.defaultschema; }
};
#endif
