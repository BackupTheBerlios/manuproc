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

// ein Teil dieser Funktionen sollte nach AuftragBase (vor allem der SQL Teil)

class Auftrag : public AuftragBase
{
protected:
	int kundennr;
	Petig::Datum datum;
	AufStatVal status;
	std::string youraufnr;
	std::string bemerkung;
	cP_Waehrung waehrung;
private:
	std::string tmpstr;
	
public:
	// Diese Strukturen dienen dazu, das Anlegen explizit anzufordern
        struct Anlegen
        {  ppsInstanz::ID instanz;
           explicit Anlegen(ppsInstanz::ID id) : instanz(id) {}
           operator ppsInstanz::ID() const { return instanz; }
        };
        struct Anlegen2
        {  AuftragBase AB;
           explicit Anlegen2(const AuftragBase& _AB) : AB(_AB) {}
           operator AuftragBase() const { return AB; }
        };
	Auftrag(const AuftragBase& auftrag) throw(SQLerror);
	// neuen Auftrag anlegen
        Auftrag(Anlegen2 Auftragsnr, long kundennr) throw(SQLerror);
        Auftrag(Anlegen instanz, long kundennr) throw(SQLerror);
        void deleteAuftrag() throw(SQLerror);
	void deleteEintrag(int zeilennr) throw(SQLerror);
	int getIdFromYourAufId(const char *youraufid) throw(SQLerror);
	void setBemerkung(const std::string &bem) throw(SQLerror);
	void setYourAufNr(const std::string &yanr) throw(SQLerror);
	void setStatusAuftrag_(AufStatVal st) throw(SQLerror);
	void setWaehrung(cP_Waehrung w) throw(SQLerror);
	
        const Petig::Datum &getDatum() const { return datum; } 
	AufStatVal getStatus() const { return status; }
        ppsInstanz::ID Instanz() const {return instanz->Id();}
	std::string getAuftragidToStr() const;
	std::string getYourAufNr() const { return youraufnr;}
	std::string getBemerkung() const { return bemerkung;}
	Kunde::ID getKundennr() const { return kundennr; }
	cP_Waehrung getWaehrung() const { return waehrung; }
};        

#endif
