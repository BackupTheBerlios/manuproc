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

#include<Aux/Ausgabe_neu.h>
#include"Lief_Leaf.h"
#include"Lief_RowData.h"
#include"Lief_TCList.h"
#include <Artikel/Einheiten.h>

const vector<string> Lief_Leaf::getColEntries(int cols)
{
 static vector<string> v;
 v=TCListRowData::getColEntries(cols);

 int stueck((dynamic_cast<const Lief_RowData &>(*leafdata)).GeliefertS());
 fixedpoint<3> menge((dynamic_cast<const Lief_RowData &>(*leafdata)).GeliefertM());
 string a;
 if (stueck!=1) a=Formatiere(stueck);
 if (menge.Scaled()!=0) 
 {  if (stueck!=1) a+="*";
    a+=Formatiere(menge)
      +string(Einheit((dynamic_cast<const Lief_RowData &>(*leafdata)).ArtikelID()));
 }
 v[Lief_TCList::LIEFMNG_SEQ]=a;

// cout << "getColEntries Leaf\n";
 return v;

}


