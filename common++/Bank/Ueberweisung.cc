/* $Id: Ueberweisung.cc,v 1.2 2003/04/24 15:38:04 jacek Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#include "Ueberweisung.h"

Ueberweisung::Ueberweisung(ManuProcEntity<>::ID uid) throw(SQLerror)
: ManuProcEntity<ID>(uid),euro(true),vzweck(std::vector<std::string>(4))
{
 std::string query="select eigenes_konto, eigene_blz, dest_konto, dest_blz, "
 	" faellig, betrag, rechnungsbetrag, auftraggeber, "
 	" empfaenger, eingegeben_am, eingegebendurch, "
 	" erfolgt_am, erfolgtdurch, kundennr, "
 	" verwendungszweck0,verwendungszweck1,verwendungszweck2,verwendungszweck3, "
 	" belegdatum, belegnr, buchungsjournal, buchungskonto "
 	" from ueberweisungen where id=?";
 	
 Query(query) << uid >> ownkonto >> ownblz >> konto >> blz 
 	>> FetchIStream::MapNull(faelligam,ManuProC::Datum())
 	>> betrag 
 	>> FetchIStream::MapNull(belegbetrag,0) 
 	>> auftraggeber >> empfaenger
 	>> erfasstam >> erfasstdurch 
 	>> FetchIStream::MapNull(erfolgtam,ManuProC::Datum()) 
 	>> FetchIStream::MapNull(erfolgtdurch,0)
 	>> FetchIStream::MapNull(kundennr,Kunde::none_id) 	
 	>> vzweck[0] 
 	>> FetchIStream::MapNull(vzweck[1],"") 	 	
 	>> FetchIStream::MapNull(vzweck[2],"") 	 	
 	>> FetchIStream::MapNull(vzweck[3],"") 	 	
 	>> FetchIStream::MapNull(belegdatum,ManuProC::Datum())  	
 	>> FetchIStream::MapNull(belegnr,ManuProcEntity<>::none_id) 	 	
 	>> FetchIStream::MapNull(buchungsjournal,ManuProcEntity<>::none_id) 	 	 	
 	>> FetchIStream::MapNull(buchungskonto,ManuProcEntity<>::none_id);

}

Ueberweisung::Ueberweisung()
: ManuProcEntity<ID>(ManuProcEntity<>::none_id),
 blz(0),
 konto(0),
 ownblz(0),
 ownkonto(0),
 betrag(0),
 belegbetrag(0),
 euro(true),
 kundennr(Kunde::none_id),
 erfasstdurch(0),
 erfolgtdurch(0),
 belegnr(ManuProcEntity<>::none_id),
 vzweck(std::vector<std::string>(4)),
 buchungskonto(0),
 buchungsjournal(0)
{}


Ueberweisung &Ueberweisung::newUeberweisung() throw(SQLerror)
{




}



std::string Ueberweisung::Vzweck(int feldnr)
{
 if(feldnr<0 || feldnr>3)
   return vzweck[0]+vzweck[1]+vzweck[2]+vzweck[3];
   
 return vzweck[feldnr];
}




