/* $Id: ArtikelBaum.h,v 1.3 2001/10/01 12:55:40 christof Exp $ */
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
#include <string>
#include <vector>
#include <Aux/SQLerror.h>
#include <Artikel/ArtikelBase.h>
#include <Aux/fixedpoint.h>
#include <Aux/ppsInstanz.h>

// besserer Name: ArtikelZusammensetzung

class ArtikelBaum : public virtual ArtikelBase
{
public:
	struct RohArtikel
	{  ID			rohartikel;
	   // in Einheit(rohartikel)/Einheit(ArtikelBase)
	   // ist ein Faktor
	   fixedpoint<5>	menge; 
	   
	   // eigentlich relativ uninteressant?
	   cH_Prozess		erzeugung;
	   
	   RohArtikel() : rohartikel(0), menge(0), 
	                  erzeugung(Prozess::default_id) 
	   {}
      RohArtikel(ID _rohartikel,fixedpoint<5> _menge)
              :rohartikel(_rohartikel),menge(_menge),
               erzeugung(Prozess::default_id)
      {}

      bool operator==(const RohArtikel &b) const {return (*this).rohartikel==b.rohartikel;}
      bool operator<(const RohArtikel &b) const {return (*this).rohartikel<b.rohartikel;}
      
	   // obsolete, compatibility only!
	   RohArtikel(const ID &_altartikel,cH_Prozess proz)
	   	: rohartikel(_altartikel), 
	   	  menge(proz->getMtrProStk()), 
	   	  erzeugung(proz)
	   {}
	};
private:
	typedef vector<RohArtikel> zusammensetzung_t;
public:
	typedef zusammensetzung_t::const_iterator const_iterator;

private:
	zusammensetzung_t zusammensetzung;
        void gleiche_Faeden_summieren();

 	// cache
	typedef CacheStatic<ID,zusammensetzung_t> cache_t;
	static cache_t cache;

public:
 ArtikelBaum() {}
 ArtikelBaum(const ArtikelBase &stamp)
 {  setID(stamp.Id()); }

// hier passiert alles, ungeschickter Name
 void setID(const ID &id) throw(SQLerror);
 
 size_t size() const
 {  return zusammensetzung.size(); }
 const_iterator begin() const
 {  return zusammensetzung.begin(); }
 const_iterator end() const
 {  return zusammensetzung.end(); }

 static void UnCache(const ArtikelBase &stamp);

// Für einen neuen Artikel:
 static void new_Artikel(ArtikelBase fuer_artikel,const RohArtikel& RA);
 static void delete_Artikel(ArtikelBase fuer_artikel,ArtikelBase von_artikel);


// -------------------------------------------
// ab hier alles veraltet, INFORMATIONSVERLUST !!!

 ArtikelBaum(const ID &stamp,const ID &_altartikel,cH_Prozess proz)
  : ArtikelBase(stamp)
 {  if (_altartikel) zusammensetzung.push_back(RohArtikel(_altartikel,proz)); }

// diese Funktion ist nun Bloedsinn, da nicht uebergeben wird, auf welcher 
// Ebene zu stoppen ist, ersetzen. Abgesehen davon muesste sie einen
// Vektor zurueckgeben!
 const ID RohArtikelID() const
 {  if (!ParentArtikelID()) return Id();
    // Rekursion !!!
    return ArtikelBaum(ParentArtikelID()).RohArtikelID();
 }
 const ID ParentArtikelID() const
 {  if (zusammensetzung.size()==0) return 0;
    return zusammensetzung[0].rohartikel;
 }

 const cH_Prozess getErzeugendenProzess() const
 {  if (zusammensetzung.size()==0) return cH_Prozess(Prozess::default_id);
    return zusammensetzung[0].erzeugung;
 }

 fixedpoint<5> Stueckgroesse() const
 {  if (zusammensetzung.size()==0) return 0;
    return zusammensetzung[0].menge;
 }

};

#endif
