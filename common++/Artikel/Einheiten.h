/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// $Id: Einheiten.h,v 1.4 2001/11/19 12:49:24 christof Exp $

// diese Datei ... sollte vielleicht demnaechst aus einer Datenbanktabelle
// erzeugt werden? Tabelle: Einheiten

#ifndef ARTIKEL_EINHEITEN_H
#define ARTIKEL_EINHEITEN_H

#include <Artikel/ArtikelBase.h>
#include <Aux/SQLerror.h>

class Einheit
{public:
	enum Einheiten { Stueck, Meter, kg, Fdkm };
	typedef enum Einheiten ID;
	static const ID default_id=Stueck;

private:
	enum Einheiten einh;

public:
	Einheit(ID e) : einh(e) {}
	ID Id() const { return einh; }
	operator std::string() const;
	operator Einheiten() const
	{ return einh;
	}
	Einheit(const ArtikelBase &ab) throw(SQLerror);
	bool operator!=(enum Einheiten b) const
	{ return einh!=b; }
	bool operator==(enum Einheiten b) const
	{ return einh==b; }
	std::string StueckEinheit() const;
	std::string MengenEinheit() const;
	std::string TeX() const;
};

// bitte nicht mehr verwenden ! (globaler Namensraum)
static const Einheit::ID EINH_STUECK=Einheit::Stueck;
static const Einheit::ID EINH_METER=Einheit::Meter;
static const Einheit::ID EINH_KG=Einheit::kg;
static const Einheit::ID EINH_FDKM=Einheit::Fdkm;

#endif
