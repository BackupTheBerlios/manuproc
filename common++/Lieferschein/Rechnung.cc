// $Id: Rechnung.cc,v 1.13 2003/10/08 16:22:58 jacek Exp $
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

void Rechnung::setRngArt(const RngArt &art) throw(SQLerror)
{
  Transaction tr;
  std::string a(1,art);
  std::string query="update rechnung set rngart='"+a+"' where rngid="
      +itos(Id());
  Query::Execute(query);
  SQLerror::test(__FILELINE__);

// must be called separately due to bool argument not known here;
//  if(art==RART_GUT) convert_to_gutschrift();

  tr.commit();
}

void Rechnung::convert_to_gutschrift(bool lager_buchung) throw(SQLerror)
{
  Transaction tr;
  std::string query="update rechnungentry set preis=preis*-1 "
                    " where rngid="+itos(Id());     
  Query::Execute(query);
  SQLerror::test(__FILELINE__);

#ifdef MABELLA_EXTENSIONS
  if(lager_buchung)
    {
     query="insert into fw_lager_buchung "
	"(artikelid,menge,datum,aktion,pid,lfrsid) "
	"(select artikelid,stueck,now(),'L',1,lfrsid from rechnungentry"
	" where rngid="+itos(Id());
     query=query+")"; 
     Query::Execute(query);
     SQLerror::test(__FILELINE__);
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

 fixedpoint<2> betrag=Betrag(brutto);
 
 if(entsorgung)
   betrag = betrag.as_float() * (1.0 + ENTSSATZ);

 if(kunde->MwSt())
    stsatz=MWSTSATZ;

 if(with_update_on_db)
  {
   Query("update rechnung set betrag = ?, steuersatz=?"
	" where rngid = ?") << betrag << stsatz << Id();
   SQLerror::test(__FILELINE__);
  }
 return betrag;
}


