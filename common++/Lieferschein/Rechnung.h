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
#include"RechnungEntry.h"
#include<Aux/SQLerror.h>
#include<Aux/Datum.h>
#include<Kunde/Kunde.h>
#include <vector>


class Rechnung : public RechnungBase
{
public:
 typedef enum {RART_RNG='R',RART_GUT='G',RART_NONE=0} RngArt; 
 
private:
 cH_Kunde kunde;
 RngArt rngart;
 Petig::Datum zahlziel;
 Petig::Datum rgdatum;
 std::vector<RechnungEntry> REntries;
 cP_Waehrung waehrung;
 fixedpoint<2> rabatt;
 bool bezahlt;

public:
 typedef std::vector<RechnungEntry>::const_iterator const_iterator;
 typedef std::vector<RechnungEntry>::iterator iterator;
        
 Rechnung(int rid) throw(SQLerror);
 Rechnung(const cH_Kunde k,int jahr=0) throw(SQLerror);

 const Kunde::ID KdNr() const {return kunde->Id();}
 const cH_Kunde getKunde() const { return kunde; }
 
// std::string RngArt() const {return rngart; } 
 const RngArt rngArt() const {return rngart; } 
 fixedpoint<2> Rabatt() const { return rabatt; }
 
 const_iterator begin() const { return REntries.begin();}
 const_iterator end() const { return REntries.end();}
 iterator begin() { return REntries.begin();}
 iterator end() { return REntries.end();}
 size_t size() const { return REntries.size();}
 cP_Waehrung getWaehrung() const { return waehrung; }
 void setzeWaehrung(const cP_Waehrung &w) throw(SQLerror);
 ExtBezSchema::ID getSchema() { return kunde->getSchema()->Id(); }
 void deleteRow(const RechnungEntry &re);
 bool Bezahlt() const { return bezahlt; }
 void addLieferschein(LieferscheinBase::ID lfrsid) throw(SQLerror);
 void deleteLieferschein(LieferscheinBase::ID lfrsid) throw(SQLerror);
 void setze_Rabatt(fixedpoint<2> r) throw(SQLerror);
 const Petig::Datum getDatum() const { return rgdatum; }
};

#endif
