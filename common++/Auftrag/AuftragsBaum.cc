// $Id: AuftragsBaum.cc,v 1.7 2002/07/05 12:35:01 christof Exp $
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

#include <Auftrag/AuftragsBaum.h>
#include <Auftrag/AufEintrag.h>
#include <Auftrag/AufEintragZu.h>
#include <list>

AuftragsBaum::AuftragsBaum(const AufEintragBase aeb,bool kinder)
{
 std::list<AufEintragZu::st_reflist> tv=AufEintragZu(aeb).get_Referenz_list(aeb,kinder);
 std::list<AufEintragZu::st_reflist> tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (std::list<AufEintragZu::st_reflist>::iterator i=tv.begin();i!=tv.end();++i)
   {
     st_AuftragsBlatt AuftragsBlatt(i->AEB,i->Art,i->Menge);
     auftragsbaum.push_back(AuftragsBlatt);
     tvxx=AufEintragZu(i->AEB).get_Referenz_list(i->AEB,kinder);
     tv.erase(i);
     goto reloop;
   }
}

