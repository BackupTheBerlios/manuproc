/* $Id: ArtikelBaum.h,v 1.20 2003/05/20 10:01:33 christof Exp $ */
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

#include <Artikel/Prozess.h>
#include <Auftrag/AufEintragBase.h>
#include <string>
#include <vector>
#include <Misc/SQLerror.h>
#include <Artikel/ArtikelBase.h>
#include <Misc/fixedpoint.h>
#include <Instanzen/ppsInstanz.h>

// besserer Name: ArtikelZusammensetzung

class ArtikelBaum : public virtual ArtikelBase
{
        mutable int tiefe;
        static const int NoDepth=-1;
public:
	typedef fixedpoint<ManuProC::Precision::ArtikelBaumFaktor> faktor_t;
	struct RohArtikel
	{  ArtikelBase			rohartikel;
	   // in Einheit(rohartikel)/Einheit(ArtikelBase)
	   // ist ein Faktor
	   faktor_t	menge; 
	   
	   // eigentlich relativ uninteressant?
	   cH_Prozess		erzeugung;
	   
	   RohArtikel() : menge(0), 
	                  erzeugung(Prozess::default_id) 
	   {}
      RohArtikel(ArtikelBase _rohartikel,faktor_t _menge,Prozess::ID proz)
              :rohartikel(_rohartikel),menge(_menge),
               erzeugung(proz)
      {}

      bool operator==(const RohArtikel &b) const {return (*this).rohartikel==b.rohartikel;}
      bool operator<(const RohArtikel &b) const {return (*this).rohartikel<b.rohartikel;}

	};
private:
	typedef std::vector<RohArtikel> zusammensetzung_t;
public:
	typedef zusammensetzung_t::const_iterator const_iterator;
	typedef zusammensetzung_t::iterator iterator;

private:
	zusammensetzung_t zusammensetzung;
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
   void gleiche_Faeden_summieren();
#endif
 	// cache
	typedef CacheStatic<ID,zusammensetzung_t> cache_t;
	static cache_t cache;

public:
 ArtikelBaum() : tiefe(NoDepth) {}
 ArtikelBaum(const ArtikelBase &stamp) : tiefe(NoDepth)
         {  setID(stamp.Id()); }

private:
// hier passiert alles, ungeschickter Name
 void setID(const ID &id) throw(SQLerror);

public:
 bool empty() const
 {  return zusammensetzung.empty(); }
 size_t size() const
 {  return zusammensetzung.size(); }
 const_iterator begin() const
 {  return zusammensetzung.begin(); }
 const_iterator end() const
 {  return zusammensetzung.end(); }
 iterator begin() 
 {  return zusammensetzung.begin(); }
 iterator end() 
 {  return zusammensetzung.end(); }
 const RohArtikel operator[](int i) {return zusammensetzung[i];}
 void Sort(); // stable sort 

 static void UnCache(const ArtikelBase &stamp);

// Hrmpf, warum sind das keine Members?
// Weil sie dann mehr Datanbankzugriffe machen würden als nötig, MAT
// Für einen neuen Artikel:
 static void new_Artikel(int uid,ArtikelBase fuer_artikel,const RohArtikel& RA);
 static void delete_Artikel(int uid,ArtikelBase fuer_artikel,ArtikelBase von_artikel);
private:
//ALter Code static void delete_from_zuordnung(int uid,ArtikelBase alt_artikel,ArtikelBase kind_artikel); 
//Alter Code static void reduceChildren(int uid,const AufEintrag& AEB,const AufEintrag& oldAEB,AufEintragBase::mengen_t menge);
//Alter Code static void create_in_zuordnung(int uid,ArtikelBase alt_artikel,ArtikelBase kind_artikel,fixedpoint<5> RohMenge); 

public:
 faktor_t Faktor(const ArtikelBase &kind) throw(SQLerror);
 int Tiefe() const;
	bool istKind(const ArtikelBase &kind) const;
};

#endif
