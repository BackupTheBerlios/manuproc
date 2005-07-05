// $Id: Rechnung.cc,v 1.24 2005/07/05 17:03:39 jacek Exp $
/*  libcommonc++: ManuProC's main OO library
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


#include "Rechnung.h"
#include <Misc/FetchIStream.h>
#include <Misc/itos.h>
#include <Misc/Transaction.h>
#include <Auftrag/auftrag_status.h>

RechnungBase::geldbetrag_t Rechnung::MwStProz=MWSTPROZ;

void Rechnung::setRngArt(const RngArt &art) throw(SQLerror)
{
 Transaction tr;
 
  Query("update rechnung set rngart=? where rngid=?")
  	<< char(art) << Id();
   SQLerror::test(__FILELINE__);
 
  if(art==RART_STORNO)
    {
     std::vector<int> lfids;
     ppsInstanz::ID insid(ppsInstanzID::Kundenauftraege);
     
     Query q("select lfrsid from lieferschein where rngid=? "
     		" and instanz=?");
     q << Id() << insid;
     q.FetchArray(lfids);
     
     for(std::vector<int>::iterator i=lfids.begin();
     		i!=lfids.end(); ++i)
     	{ 
         Query("update lieferscheinentry set status="
		"(case when lagerid is not null then ? else ? end) where "
         	" (lfrsid,instanz)=(?,?) and status=?")
         	<< (AufStatVal)OPEN 
		<< (AufStatVal)UNCOMMITED
		<< (*i) 
		<< insid 
		<< (AufStatVal)CLOSED;
         	
         Query("update lieferschein set rngid=null where (lfrsid,instanz)"
         	" = (?,?) and rngid=?")
    	   << (*i) << insid << Id();
    	}
    }
 tr.commit();
}

void Rechnung::convert_to_gutschrift(bool lager_buchung) throw(SQLerror)
{
  Transaction tr;
  Query("update rechnungentry set preis=preis*-1 where rngid=?")
  	<< Id();     
   SQLerror::test(__FILELINE__);

#ifdef MABELLA_EXTENSIONS
  if(lager_buchung)
    {
     Query("insert into fw_lager_buchung "
	"(artikelid,menge,datum,aktion,pid,lfrsid) "
	"(select artikelid,stueck,now(),'L',1,lfrsid from rechnungentry"
	" where rngid=?)")
	<< Id();
    }
#endif
  tr.commit();
}

// zu Zahlender Betrag, inkl Zuschl., etc.
Rechnung::geldbetrag_t Rechnung::Endbetrag(bool with_update_on_db=false) 
const throw(SQLerror)
{
 bool brutto=false;
 int stsatz=0;

 Preis::geldbetrag_out betrag=Betrag(brutto);
 
 if(entsorgung)
   betrag = betrag.as_float() * (1.0 + ENTSSATZ);

// if(kunde->Id()==kunde->Rngan())
//   {
    if(kunde->MwSt(Id()))
      stsatz=MWSTPROZ;
//   }
// else
//  {
//   cH_Kunde rngkd(kunde->Rngan());
//   if(rngkd->MwSt())
//     stsatz=MWSTPROZ;
//   }


 if(with_update_on_db)
  {
   Query("update rechnung set betrag = ?, steuersatz=?"
	" where rngid = ?") << betrag << stsatz << Id();
   SQLerror::test(__FILELINE__);
  }
 return betrag;
}


