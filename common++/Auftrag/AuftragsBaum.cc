// $Id: AuftragsBaum.cc,v 1.10 2003/02/14 09:53:53 christof Exp $
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

// ich vermute dass der Artikel hier auch interessant ist ...
AuftragsBaum::AuftragsBaum(const AufEintragBase aeb,bool kinder)
{
 AufEintragZu::list_t tv=AufEintragZu::get_Referenz_list(aeb,kinder,AufEintragZu::list_Artikel);
 AufEintragZu::list_t tvxx;
reloop:
 tv.splice(tv.end(),tvxx);
 for (AufEintragZu::list_t::iterator i=tv.begin();i!=tv.end();++i)
   {
     st_AuftragsBlatt AuftragsBlatt(i->AEB,i->Art,i->Menge);
     auftragsbaum.push_back(AuftragsBlatt);
     tvxx=AufEintragZu::get_Referenz_list(i->AEB,kinder,AufEintragZu::list_Artikel);
     tv.erase(i);
     goto reloop;
   }
}

