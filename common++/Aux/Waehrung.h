// $Id: Waehrung.h,v 1.16 2003/05/09 20:56:30 christof Exp $
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
#include <ManuProCConfig.h>
#include <DynamicEnums/DynamicEnums.h>
#include<Misc/SQLerror.h>
#include <Misc/CacheStatic.h>
#include <Misc/Pointer.h>

namespace WaehrungID = ManuProC::DynamicEnums::Waehrung;

class Waehrung
{
public:
 typedef WaehrungID::enum_t ID;
 static const ID default_id=WaehrungID::EUR;
 static const ID none_id=WaehrungID::None;
 typedef ID WaehID; // older variant
 
private:
 ID wid;  
 std::string kurz,tex;
 std::string lang;
 double faktor;

public:
 Waehrung(ID id) throw(SQLerror);
 Waehrung() : wid(default_id), faktor(0) {}
 Waehrung(ID id, const std::string k, const std::string l, const std::string tx, double f)
 	: wid(id), kurz(k), tex(tx), lang(l), faktor(f) {}
 ID Id() const { return wid; }
 void setFkt(const double f) { faktor=f; }
 
 // finde ich praktischer als Id() MAT. ich nicht! CP
 ID get_enum() const { return wid; } 
 
 static double Umrechnung(const Waehrung &von, const Waehrung &nach)
 { return von.faktor/nach.faktor; }
 const std::string Kurzbezeichnung() const { return kurz; }
 const std::string Langbezeichnung() const { return lang; }
 const std::string TeXsymbol() const  { return tex; }
 bool operator==(const Waehrung &b) const { return wid==b.wid; }
 bool operator!=(const Waehrung &b) const { return wid!=b.wid; }
}; 

// Vorsicht !!! Dass dies ein cP ist, funktioniert nur wegen des Caches
// und der Tatsache dass sich praktisch nie etwas hier dran aendern wird
class cP_Waehrung : public Pointer<const Waehrung>
{public:
	typedef Waehrung::ID ID;
	typedef Waehrung::ID WaehID; // old variant
private:
	// cache
	typedef CacheStatic<ID,Waehrung> cache_t;
	static cache_t cache;

public:
	cP_Waehrung() {}
	cP_Waehrung(ID id);
	cP_Waehrung(const Waehrung *w) : Pointer<const Waehrung>(w) {}
};

// damit (fast) egal ist, wie es realisiert ist 
typedef cP_Waehrung cH_Waehrung;
#endif 
