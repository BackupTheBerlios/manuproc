// $Id: Lieferart.cc,v 1.1 2003/04/08 13:15:38 jacek Exp $
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

#include "Lieferart.h"
#include <Kunde/Kunde.h>
#include <Misc/Ausgabe_neu.h>

cH_Lieferart::cache_t cH_Lieferart::cache;

cH_Lieferart::cH_Lieferart(Lieferart::ID id)
{  cH_Lieferart *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   { 
    *this=cH_Lieferart(new Lieferart(id));
      cache.Register(id,*this);
   }
}

void Lieferart::TeX_out(std::ostream &os,
		const cH_Kunde k,
		MultiL_Dict &mld) const
{
 os << mld.MLT((MultiL_Dict::LangTXT)textid) << ".\\\\\n";
}

