// $Id: delayed_reclaim.cc,v 1.2 2003/08/14 09:22:15 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski & Christof Petig
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

#include <Auftrag/AufEintrag_delayedreclaim.h>

bool AufEintrag::delayed_reclaim::active;
std::list<std::pair<cH_ppsInstanz,ArtikelBase> > 
	AufEintrag::delayed_reclaim::delayed;

void AufEintrag::delayed_reclaim::reclaim()
{  // @@@
}

AufEintrag::delayed_reclaim::delayed_reclaim()
  : nested(active)
{  if (!nested) active=true;
}

AufEintrag::delayed_reclaim::~delayed_reclaim()
{  if (!nested) 
   { reclaim(); active=false; }
}

void AufEintrag::delayed_reclaim::delay(cH_ppsInstanz inst,const ArtikelBase &ab)
{  delayed.push_back(std::pair<cH_ppsInstanz,ArtikelBase>(inst,ab));
}
