/* $Id: ArtikelGang.h,v 1.6 2002/07/15 15:37:53 christof Exp $ */
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

class ArtikelGang {
//        int index;
	int gaenge;
	ArtikelBase art;

public:
	typedef ArtikelBase::ID ID;
//Braucht man das noch? MAT	ArtikelGang(int g,ID id) : index(0), gaenge(g), art(id) {}
	ArtikelGang(int g,ArtikelBase id) : gaenge(g), art(id) {}
	ArtikelGang() : gaenge(0), art(0) {}
//	ArtikelGang(int i,int g,ArtikelBase id) : index(i), gaenge(g), art(id) {}
	bool operator==(const ArtikelGang &b) const throw()
	{  return Id()==b.Id(); }

// this is for convenience only	
	const cH_ArtikelBezeichnung Bezeichnung() const throw()
	{  return cH_ArtikelBezeichnung(art); }

//        void set_Index(int i) {index=i;}
//        int Index() const {return index;}
        ArtikelBase Artikel() const {return art;}
        bool Valid() const {if(Id()&&gaenge) return true; return false;}

	const ID &Id() const
	{  return art.Id(); }
	int Gaenge() const throw()
	{  return gaenge; }
};

extern std::ostream& operator<<(std::ostream &o,const ArtikelGang &ag);
#endif
