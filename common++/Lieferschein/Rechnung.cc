// $Id: Rechnung.cc,v 1.20 2004/01/26 20:03:02 jacek Exp $
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

RechnungBase::geldbetrag_t Rechnung::MwStProz=MWSTPROZ;

void Rechnung::setRngArt(const RngArt &art) throw(SQLerror)
{
  Query("update rechnung set rngart=? where rngid=?")
  	<< char(art) << Id();
   SQLerror::test(__FILELINE__);
// must be called separately due to bool argument not known here;
//  if(art==RART_GUT) convert_to_gutschrift();
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

 if(kunde->Id()==kunde->Rngan())
   {
    if(kunde->MwSt())
      stsatz=MWSTPROZ;
   }
 else
  {
   cH_Kunde rngkd(kunde->Rngan());
   if(rngkd->MwSt())
     stsatz=MWSTPROZ;
   }


 if(with_update_on_db)
  {
   Query("update rechnung set betrag = ?, steuersatz=?"
	" where rngid = ?") << betrag << stsatz << Id();
   SQLerror::test(__FILELINE__);
  }
 return betrag;
}


