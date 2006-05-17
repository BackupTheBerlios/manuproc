// $Id: AB_next.cc,v 1.1 2006/05/17 07:34:46 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2006 Christof Petig
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
#include <Misc/exception.h>
#include <Misc/dbconnect.h>
#include <Artikel/ArtikelBezeichnung.h>

int main()
{  ManuProC::PrintUncaughtExceptions();
   ManuProC::dbconnect();
   
   ArtikelBase next=cH_ArtikelBezeichnung(ArtikelBase(230117))->Next();
   std::cout << cH_ArtikelBezeichnung(next)->Bezeichnung() << '\n';

   return 0;
}
