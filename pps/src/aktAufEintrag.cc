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

#include"aktAufEintrag.h"
#include"auftragentry.h"
#include"auftragbase.h"
#include"glademm_support.hh"
#include<SearchCombo.h>

void aktAufEintrag::fill(AufEintrag &aufentry)
{
 zeile=aufentry.getZnr();
 stueck=aufentry.getStueck();
 lieferdatum=aufentry.getSeqValue(KW_SEQ).Datum();
 status=aufentry.getEntryStatus();
 artikel=ArtikelBase(aufentry.ArtikelID());
 stk_ok=lief_ok=true; 
 preis=aufentry.EPreis();
 rabatt=aufentry.Rabatt();
// preismenge=aufentry.PreisMenge();
}

bool aktAufEintrag::setLieferdatum(Kalenderwoche kw)
{ lieferdatum=Petig::Datum(kw); 
 lief_ok=true;
 return lief_ok;
}

bool aktAufEintrag::setLieferdatum(const Petig::Datum ld)
{ lieferdatum=ld; 
 lief_ok=true;
 return lief_ok;
}

bool aktAufEintrag::allesOK()
{
 if(!stk_ok) return false;
 if(!lief_ok) return false;
 return true;
} 




