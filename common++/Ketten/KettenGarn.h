/* $Id: KettenGarn.h,v 1.6 2002/09/02 13:04:03 christof Exp $ */
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

#ifndef KETTENGARN_HH
#define KETTENGARN_HH
#include <Artikel/ArtikelBezeichnung.h>
#include <Ketten/Kettscheibe.h>

class KettenGarn {

        int index;
        int zeile;
        int kettenzahl;
     	  int faeden;
	     ArtikelBase art;
        int laenge;
        int kombiniert;
        std::string wiederholung;
public:
	typedef ArtikelBase::ID ID;

	KettenGarn() 
	        : index(0),zeile(0),kettenzahl(0),faeden(0), art(0),laenge(0),
	         kombiniert(Kettscheibe::nicht_kombinierte_kette) {}
	KettenGarn(int i,int z,int k,int f,ArtikelBase id,int l,int k2,std::string w) 
	        : index(i), zeile(z),kettenzahl(k),faeden(f), art(id),laenge(l),
	          kombiniert(k2),wiederholung(w) {}
	bool operator==(const KettenGarn &b) const throw()
	{  return Index()==b.Index() && 
	          Artikel()==b.Artikel() &&
	          Faeden()==b.Faeden() &&
	          Kettenzahl()==b.Kettenzahl() &&
	          Laenge()==b.Laenge() ;}
	bool operator<(const KettenGarn &b) const throw()
	{  return (Index()<b.Index()) || 
	          (Index()==b.Index() && Artikel()<=b.Artikel() );}
	          
// this is for convenience only	
	const cH_ArtikelBezeichnung Bezeichnung(const cH_ExtBezSchema &h) const throw()
	{  return cH_ArtikelBezeichnung(art,h); }

   void set_Index(int i) {index=i;}
   int Index() const {return index;}
   ArtikelBase Artikel() const {return art;}
   bool Valid() const {if(Id()&&kettenzahl&&faeden) return true; return false;}


	const ID &Id() const {  return art.Id(); }
	int Zeile() const throw() {  return zeile; }
	int Faeden() const throw() {  return faeden; }
   int Kettenzahl() const throw() { return kettenzahl;}
   int Laenge() const throw() { return laenge;}
   int Kombiniert() const {return kombiniert;}
   std::string Wiederholung() const {return wiederholung;}

   void setWiederholung(const ArtikelGang &AG,std::string s)  throw(SQLerror);
};

//extern std::ostream& operator<<(std::ostream &o,const ArtikelGang &ag);
#endif
