/* $Id: ProvAbrechnung.cc,v 1.2 2002/12/03 08:46:02 christof Exp $ */
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
#include <Aux/FetchIStream.h>

const ProvAbrechnung::ID ProvAbrechnung::getNextAbrNr(const Kunde::ID verkid) throw(SQLerror)
{
 ProvAbrechnung::ID abrid;
 
 std::string qu="select coalesce(max(provnr),0)+1 from prov_abrechnung"
 	" where verknr="+itos(verkid);
 	
 Query(qu).FetchOne(abrid);	 

 return abrid;
}
