/* $Id: Auftrag.h,v 1.22 2003/06/04 11:11:39 jacek Exp $ */
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
#include <Misc/Datum.h>
#include <string>
#include <Misc/Waehrung.h>
#include <Misc/SQLerror.h>
#include <Kunde/Kunde.h> // vielleicht auch Basisklasse erzeugen
#include <Auftrag/AufEintragBase.h>

// ein Teil dieser Funktionen sollte nach AuftragBase (vor allem der SQL Teil)

class Auftrag : public AuftragBase
{
protected:
	Kunde::ID kundennr;
	Kunde::ID verknr;	
	ManuProC::Datum datum;
	AufStatVal status;
	std::string youraufnr;
	std::string bemerkung;
	mutable std::string notiz;
	mutable bool notiz_valid:1;
	cP_Waehrung waehrung;
   rabatt_t auftragsrabatt;
   ManuProC::Datum zahlziel;
   cH_Zahlungsart zahlart;
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
        
        struct AuftragCopy
        { Kunde::ID kundennr;
          int bestellt;
          std::string youraufnr;
          AuftragCopy(const Kunde::ID kid, int b, const std::string yanr):
          	kundennr(kid), bestellt(b), youraufnr(yanr) {}
          ManuProC::Datum liefdate;
        };
        
	Auftrag(const AuftragBase& auftrag) throw(SQLerror);
	// neuen Auftrag anlegen
        Auftrag(Anlegen2 Auftragsnr, long kundennr, const std::string yaufnr="") throw(SQLerror);
        Auftrag(Anlegen instanz, long kundennr, const std::string yaufnr="") throw(SQLerror);
   
        Auftrag::ID Copy(AuftragCopy &ac) throw(SQLerror);
        
        void deleteAuftrag() throw(SQLerror);
	void deleteEintrag(int zeilennr) throw(SQLerror);
	int getIdFromYourAufId(const char *youraufid) throw(SQLerror);
	void setBemerkung(const std::string &bem) throw(SQLerror);
	void setYourAufNr(const std::string &yanr) throw(SQLerror);
	void setStatusAuftrag_(AufStatVal st) throw(SQLerror);
	void setWaehrung(cP_Waehrung w) throw(SQLerror);
	void setRabatt(const rabatt_t auftragsrabatt) throw(SQLerror);
	void Zahlziel(const ManuProC::Datum &zziel) throw(SQLerror);
	void Zahlart(cH_Zahlungsart zart) throw(SQLerror);
	void Notiz(const std::string n) throw(SQLerror);
	const std::string Notiz() const throw(SQLerror);
	
   const ManuProC::Datum &getDatum() const { return datum; } 
   const ManuProC::Datum &Zahlziel() const { return zahlziel; }
   const cH_Zahlungsart Zahlart() const { return zahlart; }   
   
	AufStatVal getStatus() const { return status; }
   ppsInstanz::ID Instanz() const {return instanz->Id();}
	std::string getAuftragidToStr() const;
	std::string getYourAufNr() const { return youraufnr;}
	std::string getBemerkung() const { return bemerkung;}
	Kunde::ID getKundennr() const { return kundennr; }
	Kunde::ID getVerknr() const { return verknr; }	
	void setVerknr(const Kunde::ID) throw(SQLerror);
	cP_Waehrung getWaehrung() const { return waehrung; }
   rabatt_t getAuftragsRabatt() const {return auftragsrabatt;}

   AufEintragBase push_back(const mengen_t bestellt, 
                const ManuProC::Datum lieferdatum, const ArtikelBase& artikel,
                const AufStatVal status,int uid,const bool setInstanzAuftraege,
                const Preis& preis=Preis(),const rabatt_t aufeintragsrabatt=0,
                const cH_PreisListe &preisliste=PreisListe::none_id) const throw(SQLerror);

};        

#endif
