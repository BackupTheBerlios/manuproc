// $Id: Person.cc,v 1.7 2002/06/20 06:29:53 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#include "Kunde/Person.h"
#include "Kunde/Kunde.h"
#include "Kunde/Telefon.h"
#include <list>

cH_Person::cache_t cH_Person::cache;  

cH_Person::cH_Person(Person::ID id)
{  
   cH_Person *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   {   
    *this=cH_Person(new Person(id));
      cache.Register(id,*this);   
   }
}   


std::string Person::Kontakt(const TelArt& art, ManuProcEntity::ID kid,
		bool forcereload) const
{
 if(kontakt.valid() && !forcereload)
   return kontakt.get_first_kontakt(art,kid,Id());
 kontakt.reload(kid,Id());
 return kontakt.get_first_kontakt(art,kid,Id());   
}


const std::list<cH_Telefon> Person::getTelefon(ManuProcEntity::ID kundenid,
			bool forcereload) const
{
 vector<cH_Telefon>::iterator i;
 std::list<cH_Telefon> tl;
 
 if(!kontakt.valid() || forcereload)
   kontakt.reload(kundenid,Id());

 for(i=kontakt.begin(); i!=kontakt.end(); ++i)
  {
   if((*i)->getKunde() == kundenid ||
      kundenid==ManuProcEntity::none_id)
     tl.push_back(*i);
  }
 
 return tl;
}



