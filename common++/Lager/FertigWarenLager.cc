/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "FertigWarenLager.h"
#include <Misc/Transaction.h>
#include <Misc/TraceNV.h>

std::pair<Zeitpunkt_new,int> FertigWarenLager::letzteInventur()
{
 
 Query q("select fw.datum, fw.bestand from "+tabelle+" fw where fw.datum="
	" (select max(lb.datum) from "+tabelle+" lb where lb.aktion=?"
	"  and lb.artikelid=fw.artikelid) and fw.artikelid=?");

 q << char(FertigWaren::eInventur) << fw.Artikel().Id();

 SQLerror::test(__FILELINE__,100);
 if(q.Code()==100)  
   {
      Zeitpunkt_new oldDate(ManuProC::Datum(1,1,1970));
      return std::pair<Zeitpunkt_new,int>(oldDate,0);
   }

 Zeitpunkt_new z;
 int bestand;

 q.Fetch() >> z >> bestand;

 return std::pair<Zeitpunkt_new,int>(z,bestand);

} 



int FertigWarenLager::Bestand(ManuProC::Datum date)
{
  std::pair<Zeitpunkt_new,int> inventur=letzteInventur(); 


  Query q("select coalesce(sum(menge),0) from "+tabelle+
	" where artikelid=? and datum > ? and datum <= "
	" (?||' 23:59:59')::timestamp and aktion != 'I'");

  q << fw.Artikel().Id()
    << inventur.first
    << date;

  int STK;
  q.Fetch() >> STK; 
 
  return STK + inventur.second ;
}

std::vector<class LagerInhalt> FertigWarenLager::LagerInhalt_
  		(const ArtikelBase& artikel) const
{
 std::string q="select artikelid,1,0,bestand,0 from artikel_bestand b";
 if(artikel.valid()) q+=" where b.artikelid="+itos(artikel.Id());

 std::vector<class LagerInhalt> V;
 Query(q).FetchArray(V);

 return V;
}




void FertigWarenLager::Buchen(FertigWaren::e_buchen buchen,
		const ProductionContext &ctx)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
	NV("buchen",int(buchen)),NV("artikel",fw.Artikel()),
	NV("menge",fw.Stk(buchen)),
     	NV("aktion",char(fw.Aktion())));
 assert(fw.Artikel().Id() != ArtikelBase::none_id);

 Zeitpunkt_new z;
 
 Query("select now()") >> z;

 Transaction tr;

 Query q("insert into "+tabelle+
      	" (artikelid,menge,datum,aktion,pid,lfrsid)"
        " values (?,?,?,?,?,?)");

  fw.setZeit(z);
  
 q << fw.Artikel().Id()
   << fw.Stk(buchen) 	// buchen (Rein, Raus) legt das Vorzeichen fest
   << fw.Zeit()
   << char(fw.Aktion())
   << fw.getUserID()
   << Query::NullIf(fw.Lfrsid(),ManuProcEntity<>::none_id);

 SQLerror::test(__FILELINE__);   

 if(buchen == FertigWaren::Rein)
   rein_ins_lager(fw.Artikel(),fw.Stk(FertigWaren::AsIs),
					ctx.fuer_auftrag,ctx);
 else
 if(buchen == FertigWaren::Raus)
   raus_aus_lager(fw.Artikel(),fw.Stk(FertigWaren::AsIs),
					ctx.fuer_auftrag,ctx);
 else
 if(buchen == FertigWaren::WiederRein)
   wiedereinlagern(fw.Artikel(), fw.Stk(FertigWaren::Rein));

 fw.setStk(fw.Stk(buchen));

 tr.commit();
}


void FertigWarenLager::Inventur()
{
 assert(fw.Artikel().Id() != ArtikelBase::none_id);

 Zeitpunkt_new z;
 
 Query("select now()") >> z;

 Transaction tr;

 int alte_menge = Bestand();

 Query q("insert into  "+tabelle+
      	" (artikelid,menge,datum,aktion,pid,bestand)"
        " values (?,?,?,?,?,?)");

 fw.setZeit(z);
  
 int buchmenge=fw.Stk(FertigWaren::AsIs)-alte_menge;
 fw.setBestand(fw.Stk(FertigWaren::AsIs));

 q << fw.Artikel().Id()
   << buchmenge
   << fw.Zeit()
   << char(FertigWaren::eInventur)
   << fw.getUserID()
   << fw.Bestand();


 SQLerror::test(__FILELINE__);   


 if(buchmenge > 0)
   rein_ins_lager(fw.Artikel(),fw.Stk(FertigWaren::Rein),false);
 else
 if(buchmenge < 0)
   raus_aus_lager(fw.Artikel(),fw.Stk(FertigWaren::Raus),false);

 fw.setStk(buchmenge);

 tr.commit();
}

