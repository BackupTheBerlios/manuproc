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


#ifndef OFFAUF_NODE_H
#define OFFAUF_NODE_H
#include<tclistnode.h>

class OffAuf_Node : public TCListNode
{
 mutable int sumoffen;
 mutable int sumgeliefert;
 
public:
 
 virtual void cumulate(const cH_RowDataBase &rd, int seqnr) const;
 virtual const vector<string> getColEntries(int cols);
 virtual void resetSumValues(gpointer p);
 virtual const string getSumCol(int col);
  
 OffAuf_Node::OffAuf_Node(int _seqnr, const cH_RowDataBase &v, int deep);
 int SumOffen() const { return sumoffen; }
 int SumGeliefert() const { return sumgeliefert; }

};

#endif
