/* $Id: Verfuegbarkeit.h,v 1.3 2003/12/04 08:01:37 christof Exp $ */
/*  pps: ManuProC's ProductionPlanningSystem
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

#ifndef AUFTRAG_VERFUEGBARKEIT_H
#define AUFTRAG_VERFUEGBARKEIT_H

#include <Auftrag/AuftragBase.h>
//#include <Artikel/ArtikelBase.h>
#include <map>
class AufEintrag;

namespace Verfuegbarkeit {

typedef AuftragBase::mengen_t mengen_t;

struct Mengen
{	mengen_t geliefert,
		ungeplant,
		geplant,
		vorraetig,
		error;
	mengen_t summe() const;
};

struct mapindex
{	cH_ppsInstanz inst;
	ArtikelBase art;
	bool operator<(const mapindex &b) const
	{	return inst<b.inst || (inst==b.inst && art<b.art); }
	mapindex(const cH_ppsInstanz &i,const ArtikelBase &a)
		: inst(i),art(a) {}
};

typedef std::map<mapindex,Mengen> map_t;

void verfuegbar(const AufEintrag &ae, map_t &result, mengen_t menge=0, mengen_t offset=0);
// Gegenrichtung
void wozu_benoetigt(const AufEintrag &ae, map_t &result, mengen_t menge=0, mengen_t offset=0);
}
#endif
