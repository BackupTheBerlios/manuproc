/* $Id: LieferscheinList.cc,v 1.10 2003/02/03 15:14:38 christof Exp $ */
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



#include"LieferscheinList.h"

/*
std::ostream &operator<<(std::ostream &o,const LieferscheinList &l)
{  for(unsigned int i=0; i<l.sellist.size(); ++i)
	o << l.sellist[i].Id() << "\n"; 
   return o;
}
*/
void LieferscheinList::reset()
{
 sellist.erase(sellist.begin(),sellist.end());
 liefzeilen.erase(liefzeilen.begin(),liefzeilen.end());
 instanz=ppsInstanz::default_id;
 vom=ManuProC::Datum(1,1,1970);
 bis=ManuProC::Datum::today();
 kundenid=ManuProcEntity<>::none_id;
 artikelid=ArtikelBase::none_id;
}

