/* $Id: ProvAbrechnung.h,v 1.4 2003/01/08 09:46:58 christof Exp $ */
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


#ifndef PROVABRECHNUNG
#define PROVABRECHNUNG
#include <Misc/fixedpoint.h>
#include <string>
#include <BaseObjects/ManuProcEntity.h>
#include <Misc/Datum.h>
#include <Misc/SQLerror.h>
#include <Vertrieb/ProvAbrechnungEntry.h>
#include <DynamicEnums/DefaultValues.h>
#include <Misc/FetchIStream.h>

class ProvAbrechnung : public ManuProcEntity<>
{
 Kunde::ID verkaeufer;
 ManuProC::Datum datum;
 ManuProC::Datum bis;
 std::string notiz;
 std::vector<ProvAbrechnungEntry*> entries;
 bool entries_valid;
   
public:

 ProvAbrechnung() : verkaeufer(Kunde::none_id),entries_valid(false) {}
 ProvAbrechnung(const Kunde::ID verk, const ID abrnr) throw(SQLerror);
 void loadEntries() throw(SQLerror);
 ManuProcEntity<>::ID Id() const { return entityid; }
 Kunde::ID VerkNr() const { return verkaeufer; }
 const ManuProC::Datum getDatum() const { return datum; }
 const ManuProC::Datum getBis() const { return bis; }

 std::vector<ProvAbrechnungEntry*>::const_iterator begin() const
 	{ return entries.begin(); }
 std::vector<ProvAbrechnungEntry*>::const_iterator end() const
 	{ return entries.end(); } 	

 static ProvAbrechnung* newProvAbrechnung(const Kunde::ID verkid,
 		ProvAbrechnung::ID abrnr, ManuProC::Datum bis,
 		const std::string _notiz) throw(SQLerror);
 static const ProvAbrechnung::ID getNextAbrNr(const Kunde::ID verk) throw(SQLerror);


private:
 typedef struct {int rngid;
	 int zeilennr;
	 float provsatz;
	} FetchStruct;
	
 friend FetchIStream &operator>>(FetchIStream &is, FetchStruct &st);

};



#endif
