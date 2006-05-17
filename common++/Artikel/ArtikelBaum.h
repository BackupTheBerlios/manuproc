/* $Id: ArtikelBaum.h,v 1.24 2006/05/17 07:33:31 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2006 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

 bool bez_sorted; // nach artikelbezeichnung(altartikelid)
public:
 ArtikelBaum() : tiefe(NoDepth), bez_sorted() {}
 ArtikelBaum(const ArtikelBase &stamp, bool sorted=false, bool replace_alias=true) 
 	: tiefe(NoDepth), bez_sorted(sorted)
 { gather_data(stamp.Id(),replace_alias); }         

private:
 void gather_data(const ID &id,bool replace_alias) throw(SQLerror);

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
 static void new_Artikel(ArtikelBase fuer_artikel,const RohArtikel& RA);
 static void delete_Artikel(ArtikelBase fuer_artikel,ArtikelBase von_artikel);

public:
 faktor_t Faktor(const ArtikelBase &kind) throw(SQLerror);
 int Tiefe() const;
	bool istKind(const ArtikelBase &kind) const;

	static void Anlegen(const ArtikelBase &produkt,const ArtikelBase &edukt,
		Prozess::ID proz, const faktor_t &menge);
};

#endif
