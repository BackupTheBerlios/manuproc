// $Id: JumboLager.cc,v 1.6 2002/11/27 10:32:15 christof Exp $
/*  pps: ManuProC's production planning system
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

#include "JumboLager.h"


void JumboLager::auftragsentry_verwaltung(const Jumbo_LogTyp typ,const ArtikelBase &artikel,const AuftragBase::mengen_t &menge,const int uid) const
{
  if(typ==Auslagern || typ==InventurMinus )
   {
     raus_aus_lager(artikel,menge,uid);
   }
  else if(typ==Einlagern || typ==Zurueck || typ==InventurPlus ||
          typ==Einlagern_L || typ==Rest_L || typ==Rest)
     rein_ins_lager(artikel,menge,uid);
}

