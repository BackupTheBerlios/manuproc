/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// perhaps this belongs to pps ???

#ifndef AUFTRAG_AUFTRAGENTRY_H
#define AUFTRAG_AUFTRAGENTRY_H
#include "aktAufEintrag.h" // Datenabstraktion
#include <Auftrag/AufEintragBase.h>
#include <Auftrag/AuftragBase.h>
#include <Aux/EntryValue.h> // veraltet
#include <Artikel/ArtikelBase.h> 
#include <Aux/SQLerror.h>
#include <iostream>

class AufEintrag : public AufEintragBase
{	
public:
#if 0
 AufEintrag(const SelectedAufentry &aufentry, const cH_ExtBezSchema &schema)
	 :AufEintragBase(aufentry,schema)
 {}
#endif 
 AufEintrag(const aktAufEintrag &aufentry,const AuftragBase& auftrag, int wrkst,
 				const cH_ExtBezSchema &schema) throw(SQLerror);
 AufEintrag() {} ;
 AufEintrag(const AufEintragBase &ab) : AufEintragBase(ab) {} ;
 
 const EntryValue getSeqValue(int seqnr) const;
 const ArtikelBase::ID &ArtikelID() const { return artikel->Id(); }

 friend std::ostream &operator<<(std::ostream &o,const AufEintrag &a);
};

typedef map<int,AufEintrag> AUFENTRYMAP;
typedef std::vector<AufEintrag> AufEintragList;

#endif
