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

#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintragBase.h>

// for pps only
namespace Gtk { class CList; }
class aktAufEintrag;

class AuftragFull : public Auftrag
{	
public:
	typedef std::vector<AufEintragBase> AufEintragBaseList;
	typedef std::vector<AufEintragBase>::iterator iterator;
	typedef std::vector<AufEintragBase>::const_iterator const_iterator;
private:
	AufEintragBaseList eintragliste;
public:
//	AuftragFull(ppsInstanz::ppsInstId _instanz, int auftragid) throw(SQLerror);
	AuftragFull(const AuftragBase& auftrag) throw(SQLerror);
	// neuen Auftrag anlegen
	AuftragFull(ppsInstanz::ppsInstId _instanz, long kundennr, int jahr=0) throw(SQLerror);
	void deleteEintrag(int zeilennr, int idx) throw(SQLerror);	
	bool existsAufEntry(int idx) const
	{ return ((uint)idx) < eintragliste.size(); }
	const AufEintragBase &getAufEntry(int idx) const 
	{ return eintragliste[idx];}
	AufEintragBase &getAufEntry(int idx)  
	{ return eintragliste[idx];}
	void setStatusEntry(int idx, AufStatVal st) {eintragliste[idx].setStatus(st);}
	void updateStk(int idx, long stk) { eintragliste[idx].updateStk(stk);}	
	void updatePreis(int idx, const Preis &pr) { eintragliste[idx].updatePreis(pr); }	
	void updateLieferdatum(int idx, const Petig::Datum &ld) 
			{ eintragliste[idx].updateLieferdatum(ld);}			
	void updateRabatt(int idx, int r) { eintragliste[idx].updateRabatt(r); }
	void split(int idx, const Petig::Datum &liefdatum, int menge)
		{ eintragliste[idx].split(menge, liefdatum);}
	
	const_iterator begin() const { return eintragliste.begin(); }
	const_iterator end() const { return eintragliste.end(); }
	iterator begin() { return eintragliste.begin(); }
	iterator end() { return eintragliste.end(); }
	size_t size() const { return eintragliste.size(); }
	
	// these do not belong here - they are part of pps
	void fillCList(Gtk::CList &list);
//	void fillCListEntry(Gtk::CList &list, int idx);
	
	// komische Funktion ... Schema ueberdenken ???
	int insertNewEntry(const aktAufEintrag &aufentry,const cH_ExtBezSchema &ebsh) throw (SQLerror);
};
#endif
