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

#include <iostream>
#include <Misc/FetchIStream.h>
#include <Misc/dbconnect.h>
#include <Misc/ExtraColumns.h>
#include <cassert>

int main()
{ FetchIStream::Fake fk("200"),fk2;
  std::cout << fk.FetchMap<int>() << '\t' << fk2.FetchMap<int>() << '\n';
  
  ManuProC::dbconnect();
 {ExtraColumns ec("kunden","kundennr");
  ec << 1;
  assert(ec.good());
  std::cout << ec.get<int>("planumsatz") 
        << ',' << ec.get<std::string>("strasse") 
        << ',' << ec.get<std::string>("not_there","missing") 
        << '\n';
 }
  
 {ExtraColumns ec("kunden","kundennr");
  ec << 1;
  std::cout << ec.get<std::string>("firma") << '\n';
 }
 {ExtraColumns ec("kunden","kundennr");
  ec << 1;
 }
  ManuProC::dbdisconnect();
  return 0;
}
