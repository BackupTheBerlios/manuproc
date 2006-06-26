/* $Id: Ueberweisung.cc,v 1.11 2006/06/26 07:53:01 christof Exp $ */
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
#include <Misc/Transaction.h>
#include <pwd.h>
#include <sys/types.h>
        
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
 	
 Query q(query);

 q << uid;
 SQLerror::test(__FILELINE__);
 
 q >> ownkonto >> ownblz >> konto >> blz 
 	>> Query::Row::MapNull(faelligam,ManuProC::Datum())
 	>> betrag 
 	>> Query::Row::MapNull(belegbetrag,0) 
 	>> auftraggeber >> empfaenger
 	>> erfasstam >> erfasstdurch 
 	>> Query::Row::MapNull(erfolgtam,ManuProC::Datum()) 
 	>> Query::Row::MapNull(erfolgtdurch,-1)
 	>> Query::Row::MapNull(kundennr,Kunde::none_id) 	
 	>> vzweck[0] 
 	>> Query::Row::MapNull(vzweck[1],"") 	 	
 	>> Query::Row::MapNull(vzweck[2],"") 	 	
 	>> Query::Row::MapNull(vzweck[3],"") 	 	
 	>> Query::Row::MapNull(belegdatum,ManuProC::Datum())  	
 	>> Query::Row::MapNull(belegnr,ManuProcEntity<>::none_id) 	 	
 	>> Query::Row::MapNull(buchungsjournal,ManuProcEntity<>::none_id) 	 	 	
 	>> Query::Row::MapNull(buchungskonto,ManuProcEntity<>::none_id);
 	
 struct passwd *pw;
 
 pw=getpwuid(erfolgtdurch);
 erfolgtdurch_name=pw ? pw->pw_name : "";
 pw=getpwuid(erfasstdurch);	 
 erfasstdurch_name=pw ? pw->pw_name : ""; 	

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


Ueberweisung *Ueberweisung::newUeberweisung(const InsertStr &is) throw(SQLerror)
{
 Transaction tr;
 
 Ueberweisung::ID ui;
 
 Query("select nextval('ueberweisung_seq')") >> ui;
 SQLerror::test(__FILELINE__);
 
 Query("insert into ueberweisungen "
 	" (id,eigenes_konto,eigene_blz,dest_konto,dest_blz,"
 	"  betrag,auftraggeber,empfaenger,verwendungszweck0,"
 	"  verwendungszweck1,"
 	"  verwendungszweck2,"
 	"  verwendungszweck3,"
 	"  eingegeben_am,eingegebendurch,faellig) values "
 	" (?,?,?,?,?,"
 	"  ?,substr(trim(?),1,27),"
 	"  substr(trim(?),1,27)," 	
 	"  substr(trim(?),1,27),"
 	"  substr(trim(?),1,27)," 	
 	"  substr(trim(?),1,27),"
 	"  substr(trim(?),1,27)," 	
 	"  now(),?,?)") << ui << 
 	(long long unsigned int)(is.ownkonto) << 
 	is.ownblz << 
 	(long long unsigned int)(is.konto) << 
 	is.blz <<
 	is.betrag << is.auftraggeber << is.empfaenger <<
 	is.vzweck[0] << is.vzweck[1] << is.vzweck[2] << is.vzweck[2] <<
 	is.erfasstdurch << is.faelligam;
 SQLerror::test(__FILELINE__);
 
 Ueberweisung *u = new Ueberweisung(ui);
 SQLerror::test(__FILELINE__); 
 
 tr.commit();
 
 return u;
}



std::string Ueberweisung::Vzweck(int feldnr)
{
 if(feldnr<0 || feldnr>3)
   return vzweck[0]+vzweck[1]+vzweck[2]+vzweck[3];
   
 return vzweck[feldnr];
}



#include <unistd.h>
#include <sys/types.h>


void Ueberweisung::Storno() throw(SQLerror)
{
 if(erfolgtam.valid()) return;
 
 ManuProC::Datum invalid(1,1,1970,false);
 
 Transaction tr;
 
 Query("update ueberweisungen set erfolgt_am=?, "
 	" erfolgtdurch=? where id=?") << invalid << getuid() << Id();
 SQLerror::test(__FILELINE__);
 	
 tr.commit();
}


void Ueberweisung::setErfolgt(int diskettennr) throw(SQLerror)
{
 Transaction tr;
 
 Query("update ueberweisungen set erfolgt_am=now(), disk_refnr=?, "
 	" erfolgtdurch=? where id=?") << diskettennr << getuid() << Id();
 SQLerror::test(__FILELINE__);
 	
 tr.commit();
}



void Ueberweisung::Update(const InsertStr &is) throw(SQLerror)
{
 Transaction tr;
 ManuProC::Datum jetzt(ManuProC::Datum::today());
 ManuProC::Datum faellig(is.faelligam);

 if(!faellig.valid())
   faellig=ManuProC::Datum(1,1,1970,false);
  
 Query("update ueberweisungen set"
 	" dest_konto=?, dest_blz=?,"
 	"  betrag=?,"
 	"  empfaenger=substr(trim(?),1,27),"
 	"  verwendungszweck0=substr(trim(?),1,27),"
 	"  verwendungszweck1=substr(trim(?),1,27),"
 	"  verwendungszweck2=substr(trim(?),1,27),"
 	"  verwendungszweck3=substr(trim(?),1,27),"
 	"  eingegeben_am=?,"
 	"  eingegebendurch=?,"
 	"  faellig=?"
 	" where id=?") 
 	<<  (long long unsigned int) is.konto 
 	<< is.blz 
 	<< is.betrag
 	<< is.empfaenger
 	<< is.vzweck[0]<<is.vzweck[1]<<is.vzweck[2]<<is.vzweck[3]
 	<< jetzt << erfasstdurch
 	<<  Query::NullIf(is.faelligam,ManuProC::Datum(1,1,1970,false))
 	<< Id();
 SQLerror::test(__FILELINE__);

 konto=is.konto;
 blz=is.blz;
 betrag=is.betrag;
 vzweck=is.vzweck;
 erfasstdurch=is.erfasstdurch;
 erfasstam=jetzt;
 empfaenger=is.empfaenger;
 faelligam=is.faelligam;
  
 tr.commit();
}

