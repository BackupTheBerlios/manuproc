/* $Id: LieferscheinVoll.cc,v 1.17 2004/02/12 15:18:18 jacek Exp $ */
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
  (Query(std::string("select lfrsid, zeile, artikelid, stueck, menge, palette, "
	  "zusatzinfo, instanz, refauftragid, refzeilennr, lagerid, status"
	  " from lieferscheinentry ly "
	  " where (instanz,lfrsid) = (?,?) order by ")+
		  (auforder ? "refauftragid,zeile":"zeile")) 
  	<< Instanz()->Id() << Id())
  	.FetchArray(lsentry);
}


void LieferscheinVoll::changeStatusOnEntries(AufStatVal st) throw(SQLerror)
{
 bool fuer_auftrag=true;
 for(std::vector<LieferscheinEntry>::iterator i=lsentry.begin();
		i!=lsentry.end(); ++i)
   (*i).changeStatus(st,*this,fuer_auftrag);
}


