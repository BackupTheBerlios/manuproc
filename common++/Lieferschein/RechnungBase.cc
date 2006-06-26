/* $Id: RechnungBase.cc,v 1.5 2006/06/26 07:53:03 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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


#include"RechnungBase.h"
#include <Misc/Trace.h>

Kunde::ID RechnungBase::getVerknr() const throw(SQLerror)
{
 if(verknr!=Kunde::none_id) return verknr;

 Query("select verknr from rechnung where rngid=?")
	<< Id() >> Query::Row::MapNull(verknr,Kunde::none_id);
 return verknr;
}

void RechnungBase::setVerknr(const Kunde::ID vid) const throw(SQLerror)	
{
 Query("update rechnung set verknr=? where rngid=?")
	<< Query::NullIf(vid,Kunde::none_id) << Id();
 verknr=vid;
}



void RechnungBase::setze_Zahlziel(ManuProC::Datum zziel) throw(SQLerror)
{
 Query("update rechnung set zahlziel=? where rngid=?")
	<< zziel << Id();
}

const UniqueValue::value_t RechnungBase::trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channel_e("DEBUG_RECHNUNG",RechnungBase::trace_channel);
