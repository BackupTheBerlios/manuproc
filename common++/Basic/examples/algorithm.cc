// $Id: algorithm.cc,v 1.4 2004/07/06 12:54:56 christof Exp $
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

#include <Misc/algorithm.h>
#include <iostream>
#include <iterator>

#if defined(__GNUC__) && __GNUC__>=3 && (__GNUC_MINOR__>1 || !__GNUC_MINOR__)
namespace std{ // hey, I did not invent Koenig lookup
#endif

std::ostream &operator<<(std::ostream &o,const std::pair<unsigned,unsigned> &p)
{  return o << p.first << ':' << p.second;
}

#if defined(__GNUC__) && __GNUC__>=3 && (__GNUC_MINOR__>1 || !__GNUC_MINOR__)
}
#endif

int main()
{  typedef std::map<unsigned,unsigned> m_t;
   m_t m;
   m[1]=2;
   m[2]=4;
   m[4]=16;
   std::copy(m.begin(),m.end(),std::ostream_iterator<std::pair<unsigned,unsigned> >
       (std::cout,"\t"));
   std::cout << '\n';
   insert_between(m,2u)=3;
   std::copy(m.begin(),m.end(),std::ostream_iterator<std::pair<unsigned,unsigned> >
       (std::cout,"\t"));
   std::cout << '\n';
   insert_between(m,2u)=2;
   std::copy(m.begin(),m.end(),std::ostream_iterator<std::pair<unsigned,unsigned> >
       (std::cout,"\t"));
   std::cout << '\n';
   return 0;
}
