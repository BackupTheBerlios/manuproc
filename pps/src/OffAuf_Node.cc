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
#include"OffAuf_Node.h"
#include"OffAuf_RowData.h"

OffAuf_Node::OffAuf_Node(int _seqnr, 
		const cH_RowDataBase &v, int deep) 
 : TCListNode(_seqnr,v,deep), sumoffen(0),sumgeliefert(0)
{}


void OffAuf_Node::cumulate(const cH_RowDataBase &rd, int seqnr) const
{
 sumoffen+= (dynamic_cast<const OffAuf_RowData &>(*rd)).Offen();
 sumgeliefert+=(dynamic_cast<const OffAuf_RowData &>(*rd)).Geliefert();
}

void OffAuf_Node::resetSumValues(gpointer p)
{
 sumoffen=((OffAuf_Node*)p)->SumOffen();
 sumgeliefert=((OffAuf_Node*)p)->SumGeliefert();
}

const string OffAuf_Node::getSumCol(int col)
{
 switch(col)
   { case 0 :
	return Formatiere(sumoffen);
	break;
     case 1 :
	return Formatiere(sumgeliefert);
	break;
     default : return("-");
   }
}


const vector<string> OffAuf_Node::getColEntries(int cols)
{
 static vector<string> v;

 v=TCListRowData::getColEntries(cols);

 v[3]=Formatiere(sumoffen);
 v[4]=Formatiere(sumgeliefert);

// cout << "getColEntries Node\n";
 return v;

}


