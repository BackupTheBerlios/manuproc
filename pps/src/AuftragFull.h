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

#ifndef AUFTRAGFULL_H
#define AUFTRAGFULL_H

#include "Auftrag.h"
#include "AufEintrag.h"

namespace Gtk { class CList; }

class AuftragFull : public Auftrag
{	AufEintragList eintragliste;
public:
	AuftragFull(int auftragid) throw(SQLerror);
	AuftragFull(long kundennr, int jahr=0) throw(SQLerror);
	void fillCList(Gtk::CList &std::list);
	void fillCListEntry(Gtk::CList &std::list, int idx);
	void deleteEintrag(int zeilennr, int idx) throw(SQLerror);	
	bool existsAufEntry(int idx) const
	{ return ((uint)idx) < eintragliste.size(); }
	const AufEintrag &getAufEntry(int idx) const 
	{ return eintragliste[idx];}
	AufEintrag &getAufEntry(int idx)  
	{ return eintragliste[idx];}
	void setStatusEntry(int idx, AufStatVal st) {eintragliste[idx].setStatus(st);}
	void updateStk(int idx, long stk) { eintragliste[idx].updateStk(stk);}	
	void updatePreis(int idx, const Preis &pr) { eintragliste[idx].updatePreis(pr); }	
	void updateLieferdatum(int idx, const ManuProC::Datum &ld) 
			{ eintragliste[idx].updateLieferdatum(ld);}			
	void updateRabatt(int idx, int r) { eintragliste[idx].updateRabatt(r); }
	void split(int idx, const ManuProC::Datum &liefdatum, int menge)
		{ eintragliste[idx].split(menge, liefdatum);}
	// komische Funktion ... Schema ueberdenken ???
	int insertNewEntry(const aktAufEintrag &aufentry,const cH_ExtBezSchema &ebsh) throw (SQLerror);
};
#endif
