// $Id: ArtikelTyp.h,v 1.4 2001/10/16 06:53:12 christof Exp $
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

#ifndef ARTIKELTYP_H
#define ARTIKELTYP_H

#include <Artikel/ArtikelBase.h>
//#include <ExtBezSchema/ExtBezSchema.h>

/* quick hack to support Types really fast,
   this should be replaced by a decent generic class/scheme */

class ArtikelTyp
{
public:
	enum typ 
	 { JumboRollen=0, Band=3, Garn=4, Spulen=5, Kartonagen=6,Sonstiges=7,
	        Lohn=8, Farben=9, Etiketten=10,
	   
	 
	   GewebtesBand=JumboRollen, GefaerbtesBand=JumboRollen, 
	   BedrucktesBand=JumboRollen, 
	   AufgemachtesBand=Band,
	   Seitenscheiben=Spulen };
private:
        typedef unsigned int ID;
	typ t;
	
public:
	ArtikelTyp(typ a) : t(a) {}
	ArtikelTyp(int a) : t((typ)a) {}
	operator typ() const { return t; }
	operator int() const { return int(t); }
	// warum nicht der default operator? CP 2001-3-1
	const ArtikelTyp &operator=(const ArtikelTyp &b) { t=b.t; return *this; }
	bool operator==(const ArtikelTyp &b) const { return t==b.t; }
	bool operator==(ArtikelTyp::typ b) const { return t==b; }
	bool operator<(const ArtikelTyp &b) const { return t<b.t; }
	
	ArtikelTyp(const ArtikelBase &ab) throw(SQLerror);
	ArtikelTyp(const ArtikelBase &ab,ID id) throw(SQLerror);
	static ArtikelTyp vonArtikel(const ArtikelBase &ab) throw(SQLerror)
	{  return ArtikelTyp(ab); }
        static std::string get_string(typ t);
};
#endif
