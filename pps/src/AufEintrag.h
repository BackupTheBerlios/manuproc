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

#ifndef AUFTRAG_AUFTRAGENTRY_H
#define AUFTRAG_AUFTRAGENTRY_H
#include"aktAufEintrag.h" // hmmmm
#include<Auftrag/selAufEntry.h> // erforderlich?
#include<Auftrag/AufEintragBase.h>
#include<Aux/EntryValue.h> // veraltet
#include<Artikel/ArtikelBase.h> 
#include<Aux/SQLerror.h>
#include<Gtk_OStream.h> // weg da!

typedef map<int,AufEintrag> AUFENTRYMAP;
typedef vector<AufEintrag> AufEintragList;

class AufEintrag : public AufEintragBase
{	
	
public:
 AufEintrag(const SelectedAufentry &aufentry, const cH_ExtBezSchema &schema);	
 AufEintrag(const aktAufEintrag &aufentry,int aufid, int wrkst,
 				const cH_ExtBezSchema &schema) throw(SQLerror);
 const EntryValue getSeqValue(int seqnr) const;
 const ArtikelBase::ID &ArtikelID() const { return artikel->Id(); }
 AufEintrag() {} ;
 AufEintrag(const AufEintragBase &ab) : AufEintragBase(ab) {} ;

 friend Gtk::OStream &operator<<(Gtk::OStream &o,const AufEintrag &a);
};

#endif
