// $Id: Person.cc,v 1.2 2002/03/20 07:43:31 christof Exp $
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


/*
cH_Person::cH_Person(Person::ID id)
{
 *this=cH_Person(new Person(id));
}
*/
/*
H_Person::H_Person(Person::ID id)
{
 *this=H_Person(new Person(id));
}
*/
