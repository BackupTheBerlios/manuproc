// $Id: ArtikelStamm.h,v 1.7 2002/02/08 21:47:14 christof Exp $
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
#include <Aux/ppsInstanz.h>
#include <Artikel/Einheiten.h>

class ArtikelStamm
{	// we can't include ExtBezSchema.h, so we duplicate what we need
	typedef int ExtBezSchema_ID;
	static const ExtBezSchema_ID ExtBezSchema_default_ID=1;
	
	struct payload_t
	{  ArtikelTyp::typ typ,interntyp;
	   cH_ppsInstanz bestellen_bei;
	   ExtBezSchema_ID defaultschema;
	   Einheit einh;
	   
	   payload_t() 
	   : typ((ArtikelTyp::typ)0), interntyp((ArtikelTyp::typ)0),
	                 bestellen_bei(cH_ppsInstanz(ppsInstanz::INST_NONE)), 
	                 defaultschema(ExtBezSchema_default_ID),
	                 einh(Einheit::Stueck) {}
	};
	
	ArtikelBase art;
	payload_t payload;
	
	typedef CacheStatic<ArtikelBase::ID,payload_t> cache_t;
	static cache_t cache;
public:
	ArtikelStamm(const ArtikelBase &ab) throw (SQLerror);
	ArtikelTyp::typ Warengruppe() const
	{  return payload.typ; }
	ArtikelTyp::typ InternWarengruppe() const
	{  return payload.interntyp; }
	ArtikelTyp::typ Warengruppe(ExtBezSchema_ID id) const
	{  return id==ExtBezSchema_default_ID?payload.interntyp:payload.typ; }
	cH_ppsInstanz BestellenBei() const
	{  return payload.bestellen_bei; }
	int defaultSchema() const
	{  return payload.defaultschema; }
	Einheit getEinheit() const
	{  return payload.einh; }


        static void setEinheit(const ArtikelBase &artikel,const Einheit e);
        static void set_BestellenBei(const ArtikelBase &artikel,const ppsInstanz::ID instanz);
};
#endif
