/* $Id: Rechnung.h,v 1.28 2003/06/19 12:48:11 jacek Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef RECHNUNG__
#define RECHNUNG__

#include"RechnungBase.h"
#include<Misc/Waehrung.h>
#include<Misc/SQLerror.h>
#include<Misc/Datum.h>
#include<Kunde/Kunde.h>
#include <vector>
#include "LieferscheinBase.h"
#include <Kunde/Zahlungsart.h>
#include <Misc/Handles.h>

class Rechnung : public RechnungBase
{
private:
 cH_Kunde kunde;
 RngArt rngart;
 ManuProC::Datum zahlziel;
 ManuProC::Datum rgdatum;
 cP_Waehrung waehrung;
 rabatt_t rabatt;
 cH_Zahlungsart zahlungsart;
 bool bezahlt;
 bool entsorgung;
 mutable std::string notiz;
 mutable bool notiz_valid;
 fixedpoint<5> kurs;  
 int einzug_refnr;
  
public:
        
 Rechnung(ID rid) throw(SQLerror);
 Rechnung(const cH_Kunde k,int jahr=0) throw(SQLerror);
 Rechnung() : RechnungBase(none_id), kunde(Kunde::none_id), 
              rngart(RART_NONE), zahlziel(ManuProC::Datum::today()),
              rgdatum(ManuProC::Datum::today()),waehrung(0),
              rabatt(0),zahlungsart(Zahlungsart::none_id),bezahlt(false),
              entsorgung(false),notiz_valid(false),kurs(1)
#ifdef MABELLA_EXTENSIONS
		,einzug_refnr(0)
#endif
               {} 

 const Kunde::ID KdNr() const {return kunde->Id();}
 const cH_Kunde getKunde() const { return kunde; }
 
// std::string RngArt() const {return rngart; } 
 const RngArt rngArt() const {return rngart; } 
 void setRngArt(const RngArt &art) throw(SQLerror);
 void convert_to_gutschrift() throw(SQLerror);
 rabatt_t Rabatt() const { return rabatt; }

// don't know where is using this and i assume a bug in it. 
 geldbetrag_t Betrag(bool brutto=false) const throw(SQLerror);
 
 cP_Waehrung getWaehrung() const { return waehrung; }
 void setzeWaehrung(const cP_Waehrung &w) throw(SQLerror);
 ExtBezSchema::ID getSchema() { return kunde->getSchemaId(); }
 bool Bezahlt() const { return bezahlt; }
 void setBezahlt(bool _bezahlt) throw(SQLerror);
 void addLieferschein(const LieferscheinBase &lfrs) throw(SQLerror);
 void deleteLieferschein(const LieferscheinBase &lfrs) throw(SQLerror);
 void setze_Rabatt(rabatt_t r) throw(SQLerror);
 const ManuProC::Datum getDatum() const { return rgdatum; }
 const ManuProC::Datum getZahlziel() const { return zahlziel; }
 void setze_Datum(ManuProC::Datum rgdatum) throw(SQLerror);
 void setze_Zahlziel(ManuProC::Datum zziel) throw(SQLerror);
 cH_Zahlungsart getZahlungsart() const {return zahlungsart;}
 void setze_Zahlungsart(cH_Zahlungsart z)   throw(SQLerror);
 bool Entsorgung() const { return entsorgung;}
 const std::string Notiz() const throw(SQLerror);
 void Notiz(const std::string _notiz) throw(SQLerror);
 int getEinzugRefnr() const { return einzug_refnr; }
 int size() const throw(SQLerror);
};

/*
class H_Rechnung : public Handle<Rechnung>
{
 friend class std::map<int,H_Rechnung> ;
protected:
public:
 H_Rechnung(): Handle<Rechnung> (new Rechnung()) {}
 H_Rechnung(Rechnung *r) : Handle<Rechnung>(r){}
 H_Rechnung(Rechnung::ID i) : Handle<Rechnung>(new Rechnung(i)){}
};
*/

#endif
