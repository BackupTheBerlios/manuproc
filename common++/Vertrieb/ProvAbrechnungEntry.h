/* $Id: ProvAbrechnungEntry.h,v 1.12 2006/11/16 15:32:04 christof Exp $ */
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


#ifndef PROVABRECHNUNGENTRY
#define PROVABRECHNUNGENTRY

#include <Misc/fixedpoint.h>
#include <Misc/SQLerror.h>
#include <BaseObjects/ManuProcEntity.h>
#include <Lieferschein/RechnungEntry.h>
#include <Lieferschein/Rechnung.h>
#include <Artikel/Preis.h>
#include <Kunde/Kunde.h>

class ProvAbrechnungEntry : public ManuProcEntity<>
{
public:
 typedef enum {MAIN_ENTRY=1, SECOND_ENTRY=2, MAIN_ENTRY_MIT_ANTEIL=3,
               AUX_ENTRY=4} EntryRang;
 
private: 

 RechnungEntry rngentry;
 Preis::geldbetrag_t umsatz;
 fixedpoint<2> provsatz;
 Preis::geldbetrag_t provision;
 fixedpoint<2> anteil; // anteil in %
 EntryRang aux_idx; // aux_idx in prov_entry
 const Kunde::ID verknr;
 float rng_rabatt; 	// Gesamtrabattsatz auf der Rechnung

public:

 const fixedpoint<2> ProvSatz() const { return provsatz; }
 const Preis::geldbetrag_t Provision() const { return provision; } 
 const Preis::geldbetrag_t Umsatz() const { return umsatz; } 
 const fixedpoint<2> Anteil() const { return anteil; } 
 const EntryRang Rang() const { return aux_idx; }

 ProvAbrechnungEntry() : rngentry(RechnungEntryBase()),
 	provsatz(0.0), provision(0.0),
 	anteil(100.0),
 	aux_idx(MAIN_ENTRY),
 	verknr(Kunde::none_id),
 	rng_rabatt(0.0) {}
 	
// ProvAbrechnungEntry(const ManuProcEntity<>::ID _abrnr, 
// 	const Kunde::ID _verknr, const RechnungEntry::ID _rngid,
//	int zeilennr) throw(SQLerror); 	
	
 ProvAbrechnungEntry(const ManuProcEntity<>::ID _abrnr, 
 	const Kunde::ID _verknr, const RechnungEntry::ID _rngid,
	int zeilennr, fixedpoint<2> _psatz,
	float rng_rabattsatz,
	const fixedpoint<2> _ateil,
	EntryRang _er) throw(SQLerror); 		

 virtual ~ProvAbrechnungEntry() {};

 std::string getRngToStr() const { return rngentry.getRngidToStr(); }
 float getRngRabattsatz() const { return rng_rabatt; }
 const RechnungEntry& RngEntry() const { return rngentry; }
 
 ManuProcEntity<>::ID Id() const { return entityid;} 		     
 static void newAbrechnungEntries(
 	const ManuProcEntity<>::ID _abrnr, 
 	const Kunde::ID _verknr, const Rechnung &rng,
 	const EntryRang _er) throw(SQLerror);
 int RgZeilennr() const { return rngentry.Zeile(); }
 RechnungBase::ID RgId() const { return rngentry.Id(); }
 Kunde::ID VerkNr() const { return verknr; }
};

#endif
