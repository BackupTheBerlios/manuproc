// $Id: Rechnung.cc,v 1.8 2002/10/24 14:14:30 christof Exp $
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
#include <Aux/FetchIStream.h>
#include <Aux/itos.h>
#include <Aux/Transaction.h>

void Rechnung::setRngArt(const RngArt &art) throw(SQLerror)
{
  Transaction tr;
  std::string a(1,art);
  std::string query="update rechnung set rngart='"+a+"' where rngid="
      +itos(Id());
  Query::Execute(query);
  SQLerror::test(__FILELINE__);
  if(art==RART_GUT) convert_to_gutschrift();
  tr.commit();
}

void Rechnung::convert_to_gutschrift() throw(SQLerror)
{
  Transaction tr;
  std::string query="update rechnungentry set preis=preis*-1 "
                    " where rngid="+itos(Id());     
  Query::Execute(query);
  SQLerror::test(__FILELINE__);

#ifdef MABELLA_EXTENSIONS
  query="insert into fw_lager_buchung "
	"(artikelid,menge,datum,aktion,pid,lfrsid) "
	"(select artikelid,stueck,now(),'L',1,lfrsid from rechnungentry"
	" where rngid="+itos(Id());
  query=query+")"; 
  Query::Execute(query);
  SQLerror::test(__FILELINE__);
#endif
  tr.commit();
}

