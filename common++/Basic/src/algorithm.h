/* $Id: algorithm.h,v 1.2 2004/07/06 12:53:25 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

// miscallaneous algorithms

#ifndef FE065553_90DA_4CA9_930F_BE358AFBDFC4
#define FE065553_90DA_4CA9_930F_BE358AFBDFC4

#include <map>
#include <Misc/compiler_ports.h>

// insert a new element at where into a map, 
// moving up all existing (following) elements as needed

template <class K,class P>
 P &insert_between(std::map<K,P> &cont, const K &where, bool init=true)
{  NOTGCC295(typename) std::map<K,P>::iterator i=cont.find(where);
   if (i==cont.end()) return cont[where];
   K away=where;
   ++away;
   insert_between(cont,away,false)=i->second;
   // initialize it
   if (init) i->second=P();
   return i->second;
}

#endif
