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


#include"Lief_RowData.h"
#include"Lief_TCList.h"
#include"Lief_Value.h"
#include <Aux/Ausgabe_neu.h>

const cH_EntryValue Lief_RowData::Value(int _seqnr) const
{
 switch(_seqnr)
   {
	case Lief_TCList::AUFNR_SEQ :
		return cH_Lief_Value(Formatiere(liefentry.AufId(),0,6,"",""));
		break;
	case Lief_TCList::ARTIKEL_SEQ :
		return cH_Lief_Value(artikelbez->Bezeichnung());
		break;
	case Lief_TCList::LIEFMNG_SEQ :
		return cH_Lief_Value(liefentry.Menge());
		break;
	case Lief_TCList::LIEFZEILE_SEQ :
		return cH_Lief_Value(liefentry.Zeile());
		break;
	default : return cH_Lief_Value("-");
   }
 return cH_Lief_Value("-");
}




