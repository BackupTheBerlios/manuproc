/* $Id: ProvAbrechnung.cc,v 1.6 2003/07/17 10:07:03 jacek Exp $ */
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

#include <Vertrieb/ProvAbrechnung.h>
#include <Misc/FetchIStream.h>

const ProvAbrechnung::ID ProvAbrechnung::getNextAbrNr(
	const Kunde::ID verkid, const ManuProC::Datum vom) throw(SQLerror)
{
 ProvAbrechnung::ID abrid;
 int jahr=vom.Jahr();

 Query("select coalesce(max(provnr),?*100+0)+1 from prov_abrechnung"
 	" where verknr=? and date_part('y',vom)=?")
	<< jahr << itos(verkid) << jahr >> abrid;

 return abrid;
}

const ManuProC::Datum ProvAbrechnung::getNextVomDate(const Kunde::ID verkid) throw(SQLerror)
{
 ManuProC::Datum newvom;
 
 Query("select coalesce(max(bis)+1,date('1970-01-01')) from prov_abrechnung"
       " where verknr=?")
	<< itos(verkid) >> newvom; 


 return newvom;
}


fixedpoint<2> ProvAbrechnung::getAuszahlung() const
{
 fixedpoint<2> summe=0;

 for(std::vector<ProvAbrechnungEntry*>::const_iterator i=entries.begin();
	i!=entries.end(); ++i)
    summe+=(*i)->Provision();   

 cH_Kunde vk(verkaeufer);
 if(!vk->Auslaender())
   summe = summe * (fixedpoint<2>(1) + ((float)MWST/100));

 return summe; 

}


