/* $Id: Lieferschein.h,v 1.6 2001/12/05 07:55:59 christof Exp $ */
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

#ifndef LIEFERSCHEIN
#define LIEFERSCHEIN

#include"LieferscheinBase.h"
#include"LieferscheinEntry.h"
#include<Aux/SQLerror.h>
#include<Aux/Datum.h>
#include<Kunde/Kunde.h>
#include <vector>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>

class Lieferschein : public LieferscheinBase, public HandleContent
{
 Petig::Datum lsdatum;
 cH_Kunde kunde;
 int rngid;
 int paeckchen, pakete;
 Petig::Datum geliefertam;
 int dpdliefnr;

 public:
        
 	Lieferschein(const LieferscheinBase &source)
 		: LieferscheinBase(source), kunde(Kunde::none_id)  {}
 		
 	Lieferschein() : LieferscheinBase(none_id),
 			lsdatum(Petig::Datum::today()),
 			kunde(Kunde::none_id),rngid(0),
 			paeckchen(0),pakete(0),
 			geliefertam(Petig::Datum::today()),
 			dpdliefnr(0) {}
 			
 	Lieferschein(int lid) throw(SQLerror);
	Lieferschein(const LieferscheinBase &lsbase,
			const Petig::Datum &_lsdatum,
			int _kdnr, int _rngid, int _paeckchen, int _pakete,
			const Petig::Datum &_geliefertam,int _dpdlnr); 
			
 Lieferschein(cH_Kunde k,int jahr=0) throw(SQLerror);
			
 void setDPDlnr(int dpdlnr) const throw(SQLerror);
 void setDPDDatum() const throw(SQLerror);
			
 int Pakete() const {return pakete;}			
 int Paeckchen() const {return paeckchen;}			
 
 int KdNr() const {return kunde->Id();}		
 const cH_Kunde &getKunde() const {return kunde; }	
 const Petig::Datum LsDatum() const { return lsdatum; }
 int RngNr() const { return rngid; }  
 ExtBezSchema::ID getSchema() const { return kunde->getSchema()->Id(); }
 int maxZnr() throw(SQLerror);
 const Petig::Datum getDatum() const { return geliefertam; }
 void setDatum(const Petig::Datum &d) throw(SQLerror);
};


class cH_Lieferschein : public Handle<const Lieferschein>
{
 friend class std::map<int,cH_Lieferschein> ;
protected:
 cH_Lieferschein() {}
public:
 cH_Lieferschein(Lieferschein *r) : Handle<const Lieferschein>(r){}
 cH_Lieferschein(Lieferschein::ID i) : Handle<const Lieferschein>(new Lieferschein(i)){}
};

class H_Lieferschein : public Handle<Lieferschein>
{
 friend class std::map<int,H_Lieferschein> ;
protected:
public:
 H_Lieferschein(): Handle<Lieferschein> (new Lieferschein()) {}
 H_Lieferschein(Lieferschein *r) : Handle<Lieferschein>(r){}
 H_Lieferschein(Lieferschein::ID i) : Handle<Lieferschein>(new Lieferschein(i)){}
};

/*
class cached_Lieferschein : public cH_Lieferschein
{
   cH_Lieferschein lieferschein;

   typedef CacheStatic<int,cH_Lieferschein> cache_t;
   static cache_t cache;
 public:
   cached_Lieferschein(int lid) ;
};

cached_Lieferschein::cache_t cached_Lieferschein::cache;

cached_Lieferschein::cached_Lieferschein(int lid)
{
      cH_Lieferschein *cached(cache.lookup(lid));
      if (cached) { lieferschein = *cached; return; }
      lieferschein = new Lieferschein(lid);
      
      cache.Register(lid,lieferschein);
}
*/
#endif
