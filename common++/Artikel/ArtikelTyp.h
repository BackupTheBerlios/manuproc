// $Id: ArtikelTyp.h,v 1.8 2002/05/06 13:41:22 christof Exp $
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

#include <ManuProCConfig.h>
#include <Artikel/ArtikelBase.h>
#include <DynamicEnums/DynamicEnums.h>

namespace ArtikelTypID = ManuProC::DynamicEnums::ArtikelTyp;

class ArtikelTyp
{
public:
	typedef ManuProC::DynamicEnums::ArtikelTyp::enum_t ID;
	static const ID default_ID=ID(0);

private:
	ID t;
	
public:
	ArtikelTyp(ID a) : t(a) {}
	ArtikelTyp(int a) : t((ID)a) {}
	operator ID() const { return t; }
	operator int() const { return int(t); }
	ID Id() const { return t; }

	bool operator==(const ArtikelTyp &b) const { return t==b.t; }
	bool operator==(ID b) const { return t==b; }
	bool operator<(const ArtikelTyp &b) const { return t<b.t; }
	
	ArtikelTyp(const ArtikelBase &ab) throw(SQLerror);
	ArtikelTyp(const ArtikelBase &ab,ID id) throw(SQLerror);

	static ArtikelTyp vonArtikel(const ArtikelBase &ab) throw(SQLerror)
	{  return ArtikelTyp(ab); }
        static std::string get_string(ID t);
};
#endif
