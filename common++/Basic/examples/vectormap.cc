// $Id: vectormap.cc,v 1.3 2003/09/16 21:42:18 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/vectormap.h>
#include <iostream>
#include <string>
#include <iterator>

#if defined(__GNUC__) && __GNUC__>=3 && __GNUC_MINOR__>1
namespace std{ // hey, I did not invent Koenig lookup
#endif

std::ostream &operator<<(std::ostream &o,const std::pair<std::string,std::string> &p)
{  return o << p.first << ':' << p.second;
}

#if defined(__GNUC__) && __GNUC__>=3 && __GNUC_MINOR__>1
}
#endif

int main()
{ {vectormap_u<std::string,std::string> v;

   v["A"]="b";
   v["B"]="c";
   std::copy(v.begin(),v.end(),std::ostream_iterator<std::pair<std::string,std::string> >
   		(std::cout,"\t"));
   std::cout << '\n';
   v["A"]="d";
   v["0"]="e";
   std::copy(v.begin(),v.end(),std::ostream_iterator<std::pair<std::string,std::string> >
   		(std::cout,"\t"));
   std::cout << '\n';
  }
   std::cout << '\n';
  {vectormap_s<std::string,std::string> v;

   v["B"]="c";
   v["0"]="e";
   v["A"]="d";
   v["1"]="1";
   std::copy(v.begin(),v.end(),std::ostream_iterator<std::pair<std::string,std::string> >
   		(std::cout,"\t"));
   std::cout << '\n';
  }
}
