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
#include"Lief_Node.h"
#include"Lief_RowData.h"
#include"Lief_TCList.h"

Lief_Node::Lief_Node(int _seqnr, 
		const cH_RowDataBase &v, int deep) 
 : TCListNode(_seqnr,v,deep), sumgeliefert(0)
{}


void Lief_Node::cumulate(const cH_RowDataBase &rd, int seqnr) const
{fixedpoint<3> menge(dynamic_cast<const Lief_RowData &>(*rd).GeliefertM());
 if (!menge) menge=1.0;
 sumgeliefert+=float(menge)*(dynamic_cast<const Lief_RowData &>(*rd)).GeliefertS();
}

void Lief_Node::resetSumValues(gpointer p)
{
 sumgeliefert=((Lief_Node*)p)->SumGeliefert();
}

const string Lief_Node::getSumCol(int col)
{
 return Formatiere(sumgeliefert);
}


const vector<string> Lief_Node::getColEntries(int cols)
{
 static vector<string> v;

 v=TCListRowData::getColEntries(cols);

 v[Lief_TCList::LIEFMNG_SEQ]=Formatiere(sumgeliefert);

 cout << "getColEntries Node\n";
 return v;

}
