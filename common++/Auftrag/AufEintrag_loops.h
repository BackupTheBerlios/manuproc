/* $Id: AufEintrag_loops.h,v 1.3 2003/07/24 11:16:24 christof Exp $ */
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

#ifndef CXX_AUFTRAG_AUFEINTRAGMACROS_H
#define CXX_AUFTRAG_AUFEINTRAGMACROS_H

#include <Auftrag/AuftragBase.h>
class SQLFullAuftragSelector;
class AufEintrag;
class AufEintragBase;

struct distribute_children_cb
{	// return the amount of the third argument you processed
	virtual AuftragBase::mengen_t operator()(const ArtikelBase &,
		const AufEintragBase &,AuftragBase::mengen_t) const=0;
	// for the remainder
	virtual void operator()(const ArtikelBase &,AuftragBase::mengen_t) const=0;
};

// 0er zuerst
bool distribute_children(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_cb &callee);
bool distribute_children_rev(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, 
 		const distribute_children_cb &callee);

struct distribute_parents_cb
{	// return the amount of the third argument you processed
	virtual AuftragBase::mengen_t operator()(const AufEintragBase &,
				AuftragBase::mengen_t) const=0;
};

AuftragBase::mengen_t distribute_parents(const AufEintragBase &startAEB, 
	AuftragBase::mengen_t menge,const distribute_parents_cb &callee);

struct auf_positionen_verteilen_cb
{	virtual AuftragBase::mengen_t operator()(AufEintrag &ae, 
			AuftragBase::mengen_t abschreibmenge) const=0;
};

// erste Lieferung zuerst
AuftragBase::mengen_t auf_positionen_verteilen(
		const SQLFullAuftragSelector &selector,
 		AuftragBase::mengen_t menge, 
 		const auf_positionen_verteilen_cb &callee);
AuftragBase::mengen_t auf_positionen_verteilen_rev(
		const SQLFullAuftragSelector &selector,
 		AuftragBase::mengen_t menge, 
 		const auf_positionen_verteilen_cb &callee);
#endif
