// $Id: mystream.h,v 1.8 2004/05/28 09:34:24 christof Exp $
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

#if defined(__GNUC__) && __GNUC__<3
#include<strstream.h>
#include <string>

class mystream : public ostrstream
{
public:
  const std::string str(bool erase=false) 
  {  (*this) << '\0'; 
     const std::string res(this->std::ostrstream::str());
     freeze(0);
     if(erase) seekp(0); 
     return res;
  } 
};
#else
#include <sstream>

class mystream : public std::ostringstream
{
public:
  const std::string str(bool erase=false) 
  {  std::string res=std::ostringstream::str();
     if(erase) std::ostringstream::str(std::string());
     return res;
  } 
};

#endif
