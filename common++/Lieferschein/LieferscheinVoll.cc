/* $Id: LieferscheinVoll.cc,v 1.6 2002/05/06 13:41:23 christof Exp $ */
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
#include<Aux/SQLerror.h>
#include<Aux/Transaction.h>
#include<Aux/FetchIStream.h>

void LieferscheinVoll::deleteRow(const LieferscheinEntry &le)
{

 for(std::vector<LieferscheinEntry>::iterator i=lsentry.begin();i!=lsentry.end(); ++i)
	if((*i).Zeile()==le.Zeile())
	  {LieferscheinEntry::deleteEntry(le);
	   lsentry.erase(i);
	   break;
	  }
}

LieferscheinVoll::LieferscheinVoll(const cH_ppsInstanz& _instanz,int lid,bool auforder) throw(SQLerror)
: Lieferschein(_instanz,lid)
{
 std::string qstr =
  "select ly.artikelid, ly.zeile, coalesce(ly.stueck,0), "
  " coalesce(ly.menge,0), coalesce(ly.palette,0), coalesce(youraufnr,''),"
  "coalesce(ly.zusatzinfo,'f'), coalesce(ly.refauftragid,0),"
  " coalesce(ly.refzeilennr,0)"
  " from lieferscheinentry ly "
  " left join auftrag a on (ly.refauftragid,ly.instanz) = (a.auftragid,a.instanz)"
  " where (ly.instanz,ly.lfrsid) = ("+itos(Instanz())+","+itos(Id())+") order by "+
  (auforder ? "ly.refauftragid,ly.zeile":"ly.zeile");
  
 int artikelid,stueck,palette,zeile;
 int refauftrag,refaufzeile;
 std::string yourauftrag;
 float menge;
 bool zusatzinfo;
  
 Transaction tr;
 
 Query query(qstr);
 FetchIStream is;

 while((query>>is).good())
   {is >> artikelid >> zeile >> stueck >> menge >> palette 
   	>> yourauftrag >> zusatzinfo >> refauftrag >> refaufzeile;
   
    lsentry.push_back(LieferscheinEntry(Instanz(),Id(),zeile,artikelid,
		stueck,menge,palette,yourauftrag,zusatzinfo,
		AufEintragBase(Instanz(),refauftrag,refaufzeile)));
   }
 SQLerror::test("LieferscheinVoll::LieferscheinVoll",100);

 tr.close();
}



