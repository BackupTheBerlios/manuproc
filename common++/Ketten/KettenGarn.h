/* $Id: KettenGarn.h,v 1.12 2004/05/26 09:01:24 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
class ArtikelGang;
#include <Misc/compiler_ports.h>

struct KettenGarn_MT {

        int index;
        
        // verletzt das relationale Modell: nur von index abhängig!
        int kettenzahl;
        int laenge;
        
        int zeile;
	int faeden;
	ArtikelBase art;
        unsigned wiederholung_anz;
public:
	typedef ArtikelBase::ID ID;

	KettenGarn_MT() 
	        : index(),kettenzahl(),laenge(),zeile(),faeden(), art() {}
	KettenGarn_MT(int i,int z,int k,int f,ArtikelBase id,int l,unsigned w)
	        : index(i),kettenzahl(k),laenge(l), zeile(z),faeden(f), art(id),
	          wiederholung_anz(w) {}
	bool operator==(const KettenGarn_MT &b) const throw();
	bool operator<(const KettenGarn_MT &b) const throw();
	          
// this is for convenience only	
	__deprecated const cH_ArtikelBezeichnung Bezeichnung(const cH_ExtBezSchema &h) const throw()
	{  return cH_ArtikelBezeichnung(art,h); }

   void __deprecated set_Index(int i) {index=i;}
   int Index() const {return index;}
   ArtikelBase Artikel() const {return art;}
   bool Valid() const {return Id()&&kettenzahl&&faeden;}


	__deprecated const ID &Id() const {  return art.Id(); }
	int Zeile() const throw() {  return zeile; }
	int Faeden() const throw() {  return faeden; }
   int Kettenzahl() const throw() { return kettenzahl;}
   int Laenge() const throw() { return laenge;}
   void setFaeden(int l) {faeden=l;}
//   std::string Wiederholung() const {return wiederholung;}
   unsigned int Wiederholung_anzahl() const { return wiederholung_anz; }

//   void setWiederholung(const ArtikelGang &AG,std::string s)  throw(SQLerror);
//   void setKombiniert(const Kettscheibe::st_kombi &AG1,const ArtikelGang &AG2,int Index2)  throw(SQLerror);
//   void deleteKombiniert(const Kettscheibe::st_kombi &AG1,const ArtikelGang &AG2,int Index2)  throw(SQLerror);

//   void modify(const ArtikelGang & AG,int kettenzahl,int faeden,int faden_laenge,ArtikelBase garn);
};

//extern std::ostream& operator<<(std::ostream &o,const ArtikelGang &ag);

typedef KettenGarn_MT KettenGarn;
#endif
