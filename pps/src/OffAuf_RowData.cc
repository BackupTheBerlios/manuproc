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


#include"OffAuf_RowData.h"
#include"OffAuf_TCList.hh"
#include <Aux/Ausgabe_neu.h>

const cH_EntryValue OffAuf_RowData::Value(int _seqnr) const
{
 switch(_seqnr)
   {
	case OffAuf_TCList::AUFNR_SEQ :
		return cH_OffAuf_Value(Formatiere(auftrag.AufId(),0,6,"",""));
		break;
	case OffAuf_TCList::ARTIKEL_SEQ :
		return cH_OffAuf_Value(artikel->Bezeichnung());
		break;
	case OffAuf_TCList::LIEFDAT_SEQ :
		return cH_OffAuf_Value(liefdatum.c_str());
		break;
	default : return cH_OffAuf_Value("-");
   }
 return cH_OffAuf_Value("-");
}




