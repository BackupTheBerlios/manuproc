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
	AuftragFull(const AuftragBase& auftrag) throw(SQLerror);
	// neuen Auftrag anlegen
	AuftragFull(Auftrag::Anlegen _instanz, long kundennr, int jahr=0) throw(SQLerror);
	
	void deleteEintrag(int zeilennr, int idx) throw(SQLerror);	
	const AufEintragBase &getAufEntry(int idx) const 
	        { return eintragliste[idx];}
	AufEintragBase &getAufEntry(int idx)  
        	{ return eintragliste[idx];}
	AufEintragBase getAufEntry_by_znr(int znr)  
        	{ return eintragliste[get_index_by_znr(znr)];}
        int get_index_by_znr(int znr);
	void split(int idx, const Petig::Datum &liefdatum, int menge)
		{ eintragliste[idx].split(menge, liefdatum);}
	
	const_iterator begin() const { return eintragliste.begin(); }
	const_iterator end() const { return eintragliste.end(); }
	iterator begin() { return eintragliste.begin(); }
	iterator end() { return eintragliste.end(); }
	size_t size() const { return eintragliste.size(); }
	
	// these do not belong here - they are part of pps
	void fillCList(Gtk::CList &list);
	
	int insertNewEntry(unsigned long int bestellt,
               Petig::Datum lieferdatum, ArtikelBase::ID artid,
               AufStatVal status,
               Preis preis=Preis(), fixedpoint<2> rabatt=fixedpoint<2>(0)) throw (SQLerror);

        void setStatusAuftragFull(AufStatVal status) throw(SQLerror);
};
#endif
