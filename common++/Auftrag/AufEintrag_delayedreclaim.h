/* $Id: AufEintrag_delayedreclaim.h,v 1.2 2003/08/14 09:22:15 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef CXX_AUFTRAG_AUFEINTRAG_DELRECL_H
#define CXX_AUFTRAG_AUFEINTRAG_DELRECL_H

#include <Auftrag/AufEintrag.h>

class AufEintrag::delayed_reclaim
{  static bool active;
   static std::list<std::pair<cH_ppsInstanz,ArtikelBase> > delayed;
   
   bool nested;
   
   void reclaim();
public:
   delayed_reclaim();
   ~delayed_reclaim();
   
   void delay(cH_ppsInstanz inst,const ArtikelBase &ab);
   static bool Active() { return active; }
};

#endif
