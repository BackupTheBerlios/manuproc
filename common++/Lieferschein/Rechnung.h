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
#include<Aux/Waehrung.h>
#include<Aux/SQLerror.h>
#include<Aux/Datum.h>
#include<Kunde/Kunde.h>
#include <vector>
#include "LieferscheinBase.h"
#include "Zahlungsart.hh"
#include <Aux/Handles.h>

class Rechnung : public RechnungBase
{
public:
 typedef enum {RART_RNG='R',RART_GUT='G',RART_NONE=0} RngArt; 
 
private:
 cH_Kunde kunde;
 RngArt rngart;
 Petig::Datum zahlziel;
 cH_Zahlungsart zahlungsart;
 Petig::Datum rgdatum;
 cP_Waehrung waehrung;
 rabatt_t rabatt;
 bool bezahlt;

public:
        
 Rechnung(int rid) throw(SQLerror);
 Rechnung(const cH_Kunde k,int jahr=0) throw(SQLerror);
 Rechnung() : RechnungBase(none_id), kunde(Kunde::none_id), 
              rngart(RART_NONE), zahlziel(Petig::Datum::today()),
              zahlungsart(Zahlungsart::none_id),
              rgdatum(Petig::Datum::today()),waehrung(0),
              rabatt(0),bezahlt(false) {} 

 const Kunde::ID KdNr() const {return kunde->Id();}
 const cH_Kunde getKunde() const { return kunde; }
 
// std::string RngArt() const {return rngart; } 
 const RngArt rngArt() const {return rngart; } 
 rabatt_t Rabatt() const { return rabatt; }
 geldbetrag_t Betrag() const throw(SQLerror);
 
 cP_Waehrung getWaehrung() const { return waehrung; }
 void setzeWaehrung(const cP_Waehrung &w) throw(SQLerror);
 ExtBezSchema::ID getSchema() { return kunde->getSchemaId(); }
 bool Bezahlt() const { return bezahlt; }
 void addLieferschein(LieferscheinBase::ID lfrsid) throw(SQLerror);
 void deleteLieferschein(LieferscheinBase::ID lfrsid) throw(SQLerror);
 void setze_Rabatt(rabatt_t r) throw(SQLerror);
 const Petig::Datum getDatum() const { return rgdatum; }
 const Petig::Datum getZahlziel() const { return zahlziel; }
 void setze_Datum(Petig::Datum rgdatum) throw(SQLerror);
 cH_Zahlungsart getZahlungsart() const {return zahlungsart;}
 void setze_Zahlungsart(cH_Zahlungsart z)   throw(SQLerror);
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
