// $Id: Waehrung.h,v 1.2 2001/04/30 15:30:26 christof Exp $
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

#ifndef AUX_WAEHRUNG_H
#define AUX_WAEHRUNG_H
#include<Aux/SQLerror.h>
#include <Aux/CacheStatic.h>
#include <Aux/Pointer.h>

class Waehrung
{
public:
 typedef int WaehID;
 typedef WaehID ID;

// muesste aus Datenbank generiert werden 
 enum enum_t { DM=1, EUR, USD };
 
private:
 WaehID wid;  
 string kurz;
 string lang;
 double dmfkt;

public:
 Waehrung(WaehID id) throw(SQLerror);
 Waehrung() : wid(0), dmfkt(0) {}
 Waehrung(WaehID id, const string k, const string l, double f)
 	: wid(id), kurz(k), lang(l), dmfkt(f) {}
 WaehID Id() const { return wid; }
 
 static double Umrechnung(const Waehrung &von, const Waehrung &nach);
 const string Kurzbezeichnung() const { return kurz; }
 const string Langbezeichnung() const { return lang; }
 // das sollte natuerlich etwas ausgefeilter werden!
 const string TeXsymbol() const; //  { return Kurzbezeichnung(); }
 bool operator==(const Waehrung &b) const { return wid==b.wid; }
}; 

class cP_Waehrung : public const_Pointer<Waehrung>
{public:
	typedef Waehrung::WaehID WaehID;
private:
	// cache
	typedef CacheStatic<WaehID,Waehrung> cache_t;
	static cache_t cache;

	cP_Waehrung(const Waehrung *w) : const_Pointer<Waehrung>(w) {}
public:
	cP_Waehrung() {}
	cP_Waehrung(WaehID id);
};
#endif 
