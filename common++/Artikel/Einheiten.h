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

// $Id: Einheiten.h,v 1.2 2001/04/30 15:30:25 christof Exp $

// diese Datei ... sollte vielleicht demnaechst aus einer Datenbanktabelle
// erzeugt werden? Tabelle: Einheiten

#ifndef ARTIKEL_EINHEITEN_H
#define ARTIKEL_EINHEITEN_H

#include <Artikel/ArtikelBase.h>
#include <Aux/SQLerror.h>

enum Einheiten
{  EINH_STUECK, EINH_METER, EINH_KG, EINH_FDKM };

class Einheit
{	enum Einheiten einh;
public:
	typedef enum Einheiten ID;
	
	Einheit(ID e) : einh(e) {}
	operator string() const;
	operator Einheiten() const
	{ return einh;
	}
	Einheit(const ArtikelBase &ab) throw(SQLerror);
	bool operator!=(enum Einheiten b) const
	{ return einh!=b; }
	bool operator==(enum Einheiten b) const
	{ return einh==b; }
	string StueckEinheit() const;
	string MengenEinheit() const;
	string TeX() const;
};

#endif
