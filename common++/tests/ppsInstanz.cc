// $Id: ppsInstanz.cc,v 1.6 2003/03/31 08:14:39 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#include <Instanzen/ppsInstanz.h>
#include <vector>
#include <Misc/dbconnect.h>
#include <map>

int main()
{
  ManuProC::dbconnect_nt();
  cH_ppsInstanz I(ppsInstanzID::Weberei);
  std::map<cH_ppsInstanz,int> V;
  V[cH_ppsInstanz(ppsInstanzID::Schaererei)]=1;
  V[cH_ppsInstanz(ppsInstanzID::Spritzgiesserei)]=2;
  V[cH_ppsInstanz(ppsInstanzID::Druckerei)]=5;
  V[cH_ppsInstanz(ppsInstanzID::Weberei)]=6;
  V[cH_ppsInstanz(ppsInstanzID::Kundenauftraege)]=3;
  V[cH_ppsInstanz(ppsInstanzID::Faerberei)]=4;

  for(std::map<cH_ppsInstanz,int>::const_iterator i=V.begin();i!=V.end();++i)
   {
    cout << i->first->Sortierung() <<'\n';
   }
}

