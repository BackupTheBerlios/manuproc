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

#ifndef AUFTRAG
#define AUFTRAG
#include"auftragbase.h"
#include"auftragentry.h"
#include"termsplidial.hh"
#include<Auftrag/auftrag_status.h>

#include<Aux/Datum.h>
#include<Aux/Ausgabe_neu.h>
#include<string>
#include<gtk--/clist.h>

extern termsplidial *splitdial;

class Auftrag
{	int auftragid;
	int kundennr;
	Petig::Datum datum;
	AufStatVal status;
	string youraufnr;
	string bemerkung;
	int wrkstatus;
	string tmpstr;
	int jahrgang;
	AufEintragList eintragliste;
	cP_Waehrung waehrung;
	
public:

	Auftrag(int auftragid) throw(SQLerror);
        Auftrag(long kundennr, int jahr=0) throw(SQLerror);
        void newArtWrkar(aktAufEintrag &entry) throw(SQLerror);
        void deleteAuftrag() throw(SQLerror);
	void deleteEintrag(int zeilennr, int idx) throw(SQLerror);	
	int insertNewEntry(aktAufEintrag &aufentry,const cH_ExtBezSchema &ebsh);
	void fillCList(Gtk::CList &list);
	void fillCListEntry(Gtk::CList &list, int idx);
	int getIdFromYourAufId(const char *youraufid) throw(SQLerror);
	void setBemerkung(const string &bem) throw(SQLerror);
	void setYourAufNr(const string &yanr) throw(SQLerror);
	void setStatusAuftrag(AufStatVal st) throw(SQLerror);
	void setJahrgang(int jahr) throw(SQLerror);
	
	bool existsAufEntry(int idx) const
	{ return ((uint)idx) < eintragliste.size(); }
	const AufEintrag &getAufEntry(int idx) const 
	{ return eintragliste[idx];}
	AufEintrag &getAufEntry(int idx)  
	{ return eintragliste[idx];}
        const Petig::Datum &getDatum() const { return datum; } 
	AufStatVal getStatus() const { return status; }
	int getAuftragid() const { return auftragid; }
	string getAuftragidToStr() const
	{ return Formatiere(auftragid,0,6,"","",'0'); }
	string getYourAufNr() const { return youraufnr;}
	string getBemerkung() const { return bemerkung;}
	bool isNew() const { return wrkstatus==INSERTED;}
	void setWrkStatus(int status) {wrkstatus=status;}
	void setStatusEntry(int idx, AufStatVal st) {eintragliste[idx].setStatus(st);}
	void updateStk(int idx, long stk) { eintragliste[idx].updateStk(stk);}	
	void updatePreis(int idx, const Preis &pr) { eintragliste[idx].updatePreis(pr); }	
	void updateLieferdatum(int idx, const Petig::Datum &ld) 
			{ eintragliste[idx].updateLieferdatum(ld);}			
	void updateRabatt(int idx, int r) { eintragliste[idx].updateRabatt(r); }
	void split(int idx, const Petig::Datum &liefdatum, int menge)
		{ eintragliste[idx].split(menge, liefdatum);}
	string getJahrgangStr() const { return Formatiere(jahrgang,0,4,"",""); }
	int getJahrgang() const { return jahrgang; }
	Kunde::ID getKundennr() const { return kundennr; }
	cP_Waehrung getWaehrung() const { return waehrung; }
};        

#endif
