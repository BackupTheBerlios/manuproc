/* $Id: LieferscheinVoll.cc,v 1.23 2004/10/19 11:46:26 christof Exp $ */
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


#include"LieferscheinVoll.h"
#include<Misc/SQLerror.h>
#include<Misc/Transaction.h>
#include<Misc/FetchIStream.h>
#include <BaseObjects/ManuProcEintrag.h>

void LieferscheinVoll::deleteRow(LieferscheinEntry &le)
{
 LieferscheinEntry::deleteEntry(le);
 
 for(std::vector<LieferscheinEntry>::iterator i=lsentry.begin();i!=lsentry.end(); ++i)
	if((*i).Zeile()==le.Zeile())
	  {lsentry.erase(i);
	   break;
	  }
}

LieferscheinVoll::LieferscheinVoll(const cH_ppsInstanz& _instanz,int lid,bool auforder) throw(SQLerror)
: Lieferschein(_instanz,lid)
{

// std::string tabelle="artbez_"+
//       itos(ExtBezSchema::default_Typ)+"_"+
//      itos(ExtBezSchema::default_id);

 cH_ExtBezSchema exbz(ExtBezSchema::default_id,ExtBezSchema::default_Typ);
 std::string sortstr=auforder ? "refauftragid":"zeile";
 sortstr+=",artikelbezeichnung(ly.artikelid)";

// int signifikanz=1;
// for(ExtBezSchema::const_sigiterator l=exbz->sigbegin(signifikanz);
//				l!=exbz->sigend(signifikanz); ++l)
//   sortstr+=std::string(",")+l->spaltenname;
 

 std::string query("select lfrsid, zeile, artikelid, stueck, menge, palette,");
 query+=std::string("zusatzinfo, instanz, refauftragid, ")+
	  " refzeilennr, lagerid, status, text"+
	  " from lieferscheinentry ly "+ // "left join "+tabelle+" b "+
//	  " on (b.id=ly.artikelid) "+
	  " where (instanz,lfrsid) = (?,?) "+
	  " and coalesce(status,0)<>? order by "+sortstr;

 Query q(query);
 q << Instanz()->Id() << Id() << (AufStatVal)STORNO;
 q.FetchArray(lsentry);
}


void LieferscheinVoll::changeStatusOnEntries(AufStatVal st) throw(SQLerror)
{
 bool fuer_auftrag=true;
 for(std::vector<LieferscheinEntry>::iterator i=lsentry.begin();
		i!=lsentry.end(); ++i)
   (*i).changeStatus(st,fuer_auftrag);
}

