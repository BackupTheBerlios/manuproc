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

#ifndef AUFTRAG_H
#define AUFTRAG_H
#include <Auftrag/auftrag_status.h> // fuer AufStatVal & Co
#include <Auftrag/AuftragBase.h>
#include <Aux/Datum.h>
#include <string>
#include <Aux/Waehrung.h>
#include <Aux/SQLerror.h>
#include <Kunde/Kunde.h> // vielleicht auch Basisklasse erzeugen

class Auftrag : public AuftragBase
{
protected:
	int kundennr;
	Petig::Datum datum;
	AufStatVal status;
	std::string youraufnr;
	std::string bemerkung;
	enum wrkstatus_t {  LOADED, INSERTED } wrkstatus;
	int jahrgang;
	cP_Waehrung waehrung;
private:
	std::string tmpstr;
	
public:
//	Auftrag(ppsInstanz::ppsInstId instanz,int auftragid) throw(SQLerror);
	Auftrag(const AuftragBase& auftrag) throw(SQLerror);
	// neuen Auftrag anlegen
        Auftrag(ppsInstanz::ppsInstId instanz, long kundennr, int jahr=0) throw(SQLerror);
//        void newArtWrkar(aktAufEintrag &entry) throw(SQLerror);
        void deleteAuftrag() throw(SQLerror);
	void deleteEintrag(int zeilennr) throw(SQLerror);
	int getIdFromYourAufId(const char *youraufid) throw(SQLerror);
	void setBemerkung(const std::string &bem) throw(SQLerror);
	void setYourAufNr(const std::string &yanr) throw(SQLerror);
	void setStatusAuftrag(AufStatVal st) throw(SQLerror);
	void setJahrgang(int jahr) throw(SQLerror);
	void setWaehrung(cP_Waehrung w) throw(SQLerror);
	
        const Petig::Datum &getDatum() const { return datum; } 
	AufStatVal getStatus() const { return status; }
	int getAuftragid() const { return auftragid; }
        ppsInstanz Instanz() const {return instanz;}
	std::string getAuftragidToStr() const;
	std::string getYourAufNr() const { return youraufnr;}
	std::string getBemerkung() const { return bemerkung;}
	bool isNew() const { return wrkstatus==INSERTED;}
	void setWrkStatus(wrkstatus_t status) {wrkstatus=status;}
	std::string getJahrgangStr() const;
	int getJahrgang() const { return jahrgang; }
	Kunde::ID getKundennr() const { return kundennr; }
	cP_Waehrung getWaehrung() const { return waehrung; }
};        

#endif
