/* $Id: AuftragFull.h,v 1.33 2006/08/03 11:17:40 christof Exp $ */
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
#include <Auftrag/AufEintrag.h>

// for pps only
namespace Gtk { class CList; }
class aktAufEintrag;

class AuftragFull : public Auftrag
{	
public:
	typedef std::vector<AufEintrag> AufEintragList;
	typedef std::vector<AufEintrag>::iterator iterator;
	typedef std::vector<AufEintrag>::const_iterator const_iterator;
	typedef std::vector<AufEintrag>::const_reverse_iterator const_reverse_iterator;
private:
	AufEintragList eintragliste;
	std::string sort_clausel;

// die gef�llt mir aber gar nicht (inkonsistente parameter etc) CP
	void split(int idx,const ManuProC::Datum &liefdatum, int menge);
//		{ eintragliste[idx].split(menge, liefdatum);}
public:
	AuftragFull(const AuftragBase& auftrag,bool with_storno=true) throw(SQLerror);
	AuftragFull(const AuftragBase& auftrag,std::string sortierug,bool with_storno=true) throw(SQLerror);	
	// neuen Auftrag anlegen
	AuftragFull(Auftrag::Anlegen _instanz, long kundennr, 
			const std::string yaufnr="") throw(SQLerror);
	
	void deleteEintrag(int zeilennr, int idx) throw(SQLerror);
	const AufEintrag &getAufEntry(int idx) const 
	        { return eintragliste[idx];}
	AufEintrag &getAufEntry(int idx)  
        	{ return eintragliste[idx];}
	AufEintrag getAufEntry_by_znr(int znr)  
        	{ return eintragliste[get_index_by_znr(znr)];}
        int get_index_by_znr(int znr);
	
	const_iterator begin() const { return eintragliste.begin(); }
	const_iterator end() const { return eintragliste.end(); }
	const_reverse_iterator rbegin() const { return eintragliste.rbegin(); }
	const_reverse_iterator rend() const { return eintragliste.rend(); }
	iterator begin() { return eintragliste.begin(); }
	iterator end() { return eintragliste.end(); }
	size_t size() const { return eintragliste.size(); }
	
	void loadEntries(bool with_storno=true) throw(SQLerror);
	
   AufEintragBase push_back(AuftragBase::mengen_t bestellt,
               ManuProC::Datum lieferdatum, ArtikelBase artid,
               AufStatVal status,
               Preis preis=Preis(), rabatt_t rabatt=rabatt_t(0),
               const cH_PreisListe &preisliste=PreisListe::none_id,
               const ArtikelBase& component_from=ArtikelBase()) throw (SQLerror);
   void push_back(AufEintrag aeb);
               
   bool existsEntry(const ArtikelBase::ID art, AufStatVal stat,
        		ManuProC::Datum lieferdatum) throw(SQLerror);               
   bool existsEntry(const ArtikelBase::ID art, AufStatVal stat) throw(SQLerror);

   void setStatusAuftragFull(AufStatVal status) throw(SQLerror);
};

class Tag;
void serialize(Tag &dest, AuftragFull const& a,bool bestaetigung=true);
#endif
