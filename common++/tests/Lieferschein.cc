// $Id: Lieferschein.cc,v 1.7 2003/01/08 09:46:58 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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
#include <Kunde/Kunde.h>
#include <Misc/exception.h>
#include <Misc/dbconnect.h>
#include <Lieferschein/LieferscheinVoll.h>
//#include <Instanzen/ppsInstanz.h>

int main()
{  ManuProC::PrintUncaughtExceptions();
   try{
   ManuProC::dbconnect();

   cH_ppsInstanz I(ppsInstanzID::Kundenauftraege);
   cH_LieferscheinVoll LV=cH_LieferscheinVoll(I,20000);

   }catch(SQLerror &e){std::cout << e<<'\n';}
   return 0;
}
