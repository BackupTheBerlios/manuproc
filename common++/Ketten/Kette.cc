// $Id: Kette.cc,v 1.5 2002/01/22 09:15:55 christof Exp $
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


#include <Ketten/Kette.h>
#include <Aux/string0.h>
#include <strstream.h>
#include <iomanip.h>
#include <stdio.h> // snprintf0

bool Kette::Valid() const
{
 if(Maschine()) return true;  
 return false;
}




std::string Kette::barco_format() const throw()
{  std::ostrstream o;
   o << setw(3) << setfill('0') << maschine << "/" << setw(2) << schaerdatum << char(0);
   std::string ret((const char*)o.str());
   o.freeze(0);
   return ret;
}

void Kette::write(int &a, char*rdat, unsigned int sz) const throw()
{  a=maschine;
   schaerdatum.write_postgres(rdat,sz);
}

void Kette::write(char*rdat, unsigned int sz) const throw()
{  snprintf0(rdat,sz,"%03d/",maschine);
   if (sz<4) return;
   schaerdatum.write_euro(rdat+4,sz-4);
}

Kette::Kette(const char *b) throw(Petig::Datumsfehler,illegal_format)
{  if (strlen(b)<14 || (b[3]!='/' && b[3]!='-') || b[6]!='.' || b[9]!='.') 
      throw illegal_format();
   maschine=Petig::Datum::getnum((unsigned char*)b,3);
   schaerdatum.from_c_str(b+4);
}

