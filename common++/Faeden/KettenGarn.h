/* $Id: KettenGarn.h,v 1.11 2004/07/06 14:27:33 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef _989D4D79_BC5D_4151_BF8F_5DC6C209D019
#define _989D4D79_BC5D_4151_BF8F_5DC6C209D019
#include <vector>
#include <Ketten/ArtikelGang.h>
#include <Faeden/Faden.hh>

struct KS_Garn
{       unsigned zeile;
     	unsigned faeden;
	ArtikelBase material;
        unsigned wiederholungen; // vernünftigere Datenstruktur!!!

        bool valid() const {return !!Artikel()&&faeden;}
// old routines
   const ArtikelBase &Artikel() const {return material;}
	int Zeile() const throw() {  return zeile; }
	int Faeden() const throw() {  return faeden; }
   unsigned Wiederholung_anzahl() const { return wiederholungen; }
   KS_Garn() : zeile(), faeden(), wiederholungen(1) {}
};

struct Kettscheibe
{	int index;
        int laenge;
        int kettenzahl;
        std::vector<ArtikelGang> artikel;
        std::vector<KS_Garn> faeden;
        std::vector<Wiederholung> wiederholungen;
        unsigned fadenzahl; // errechnet
        unsigned max_fadenzahl; 

        Kettscheibe() : index(), laenge(), kettenzahl(), fadenzahl(), 
            max_fadenzahl() 
        {}
// old routines
   int Index() const {return index;}
   int Kettenzahl() const throw() { return kettenzahl;}
   int Laenge() const throw() { return laenge;}
   
   static std::vector<Kettscheibe> Load(const std::vector<ArtikelGang> &ag,unsigned laenge);
   static std::vector<Kettscheibe> Load(const ArtikelGang &ag,unsigned laenge);
};

#if 0
#include <Artikel/ArtikelBezeichnung.h>
#include <Faeden/Kettscheibe.h>
class ArtikelGang;

struct KettenGarn_CP {

        int index; // dies ist nicht relationale Normalform!
        int laenge;
        int kettenzahl;

        int zeile;
     	int faeden;
	ArtikelBase art;
        int wiederholungen; // vernünftigere Datenstruktur!!!
public:
	typedef ArtikelBase::ID ID;

	KettenGarn_CP() 
	        : index(),laenge(),kettenzahl(),zeile(),faeden(), art(),wiederholungen(1) {}
	KettenGarn_CP(int i,int z,int k,int f,ArtikelBase id,int l,int w) 
	        : index(i),laenge(l),kettenzahl(k), zeile(z),faeden(f), art(id),
	          wiederholungen(w) {}
	bool operator==(const KettenGarn_CP &b) const throw();
	bool operator<(const KettenGarn_CP &b) const throw();
	          
// this is for convenience only	
	__deprecated const cH_ArtikelBezeichnung Bezeichnung(const cH_ExtBezSchema &h) const throw()
	{  return cH_ArtikelBezeichnung(art,h); }

   void set_Index(int i) {index=i;}
   int Index() const {return index;}
   ArtikelBase Artikel() const {return art;}
   bool Valid() const {return !!Artikel()&&kettenzahl&&faeden;}

	__deprecated const ID &Id() const {  return art.Id(); }
	int Zeile() const throw() {  return zeile; }
	int Faeden() const throw() {  return faeden; }
   int Kettenzahl() const throw() { return kettenzahl;}
   int Laenge() const throw() { return laenge;}
   void setFaeden(int l) {faeden=l;}
   __deprecated std::string Wiederholung() const {return ""; } // wiederholung;}
   unsigned int Wiederholung_anzahl() const { return wiederholungen; }
   
   static std::vector<KettenGarn_CP> Load(const ArtikelGang &ag,unsigned laenge);
};
//extern std::ostream& operator<<(std::ostream &o,const ArtikelGang &ag);

typedef KettenGarn_CP KettenGarn;
#endif

#endif
