/* $Id: ArtikelGang.h,v 1.8 2002/09/18 08:58:34 christof Exp $ */
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

#ifndef ARTIKELGANG_HH
#define ARTIKELGANG_HH
#include <Artikel/ArtikelBase.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Ketten/Kettscheibe.h>
#include <Ketten/KettenGarn.h>
//class Kettscheibe;

class ArtikelGang {
	int gaenge;
	ArtikelBase art;
   mutable std::list<Kettscheibe> kettscheiben;
//   mutable int index; // Kettenindex für die Kombinierte Kette

public:
	typedef ArtikelBase::ID ID;
	ArtikelGang(int g,ArtikelBase id,bool load_garn=false,int kettlaenge=0) : gaenge(g), art(id)
	      { if(load_garn) load_Garn(kettlaenge);  }
	ArtikelGang() : gaenge(0), art(0) {}
	bool operator==(const ArtikelGang &b) const throw()
	{  return Id()==b.Id() && Gaenge()==b.Gaenge() ;}
	bool operator<(const ArtikelGang &b) const throw()
	{  return Id()<b.Id() || (Id()==b.Id() && Gaenge()<b.Gaenge()); }

// this is for convenience only	
	const cH_ArtikelBezeichnung Bezeichnung() const throw()
	{  return cH_ArtikelBezeichnung(art); }

   ArtikelBase Artikel() const {return art;}
   bool Valid() const {if(Id()&&gaenge) return true; return false;}

	const ID &Id() const {  return art.Id(); }
	int Gaenge() const throw() {  return gaenge; }
   void setKombinierteKette(int index,Kettscheibe::st_kombi b) const ;
   std::list<Kettscheibe>& getKettscheiben() {return kettscheiben;}
   std::string KombiniertMit_c_str(const KettenGarn& garn) const;
   std::vector<Kettscheibe::st_kombi> KombiniertMit(const KettenGarn& garn) const;

   void save_Garn(const KettenGarn& garn/*,std::list<ArtikelGang> KombiArtikel*/) const  throw(SQLerror);
   void delete_Garn(const KettenGarn& garn) const  throw(SQLerror);
   std::vector<KettenGarn> load_Garn(int kettlaenge) const;

   enum e_wiederholung{Next,Same};
   std::string getWiederholung(const e_wiederholung &ew) const;
};

extern std::ostream& operator<<(std::ostream &o,const ArtikelGang &ag);
#endif
