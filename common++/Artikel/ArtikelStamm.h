// $Id: ArtikelStamm.h,v 1.18 2003/03/19 14:12:22 jacek Exp $
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
#include <Misc/CacheStatic.h>
#include <Instanzen/ppsInstanz.h>
#include <Artikel/Einheiten.h>
#include <Misc/UniqueValue.h>

class ArtikelStamm
{	// we can't include ExtBezSchema.h, so we duplicate what we need
	typedef int ExtBezSchema_ID;
	static const ExtBezSchema_ID ExtBezSchema_default_ID=1;
	static const int NO_CHECK=-1; // Wert für mindbest;	 
	
	struct payload_t
	{  ArtikelTyp::ID typ,interntyp;
	   cH_ppsInstanz bestellen_bei;
	   ExtBezSchema_ID defaultschema;
	   Einheit einh;
           int mindbest;	   
	   
	   payload_t() 
	   : typ((ArtikelTyp::ID)0), interntyp((ArtikelTyp::ID)0),
	                 bestellen_bei(cH_ppsInstanz(ppsInstanzID::None)), 
	                 defaultschema(ExtBezSchema_default_ID),
	                 einh(Einheit::default_id),mindbest(NO_CHECK) {}
	};
	
	ArtikelBase art;
	payload_t payload;
	
	typedef CacheStatic<ArtikelBase::ID,payload_t> cache_t;
	static cache_t cache;
public:
	
	static const UniqueValue::value_t trace_channel;

	ArtikelStamm(const ArtikelBase &ab) throw (SQLerror);
	ArtikelTyp::ID Warengruppe() const
	{  return payload.typ; }
	ArtikelTyp::ID InternWarengruppe() const
	{  return payload.interntyp; }
	ArtikelTyp::ID Warengruppe(ExtBezSchema_ID id) const
	{  return id==ExtBezSchema_default_ID?payload.interntyp:payload.typ; }
private:
friend class ppsInstanz;
friend class Artikeleingabe;
friend class AufArtKonsistenz;
	cH_ppsInstanz BestellenBei() const
	{  return payload.bestellen_bei; }
public:
//	ArtikelBase::ID Id() const { return art.Id();}
	int defaultSchema() const
	{  return payload.defaultschema; }
	Einheit getEinheit() const
	{  return payload.einh; }


        static void setEinheit(const ArtikelBase &artikel,const Einheit e);
        static void set_BestellenBei(const ArtikelBase &artikel,const ppsInstanz::ID instanz);
	static void setAktive(const ArtikelBase &artikel, bool ak) throw(SQLerror);
	bool getAktive() const throw(SQLerror);        
	bool getCheckBest() const { return payload.mindbest!=NO_CHECK; }
	int getMindBest() const { return payload.mindbest; }
	void setMindBest(int mb) throw(SQLerror);
	
	
};
#endif
