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



#include<treebase.h>
#include<Aux/SQLerror.h>
#include<Kunde/Kunde.h>

#ifndef OFFAUFTCLIST
#define OFFAUFTCLIST



class OffAuf_TCList : public TreeBase
{
 Kunde::ID kundennr;

public:

 typedef enum {AUFNR_SEQ=0,ARTIKEL_SEQ,LIEFDAT_SEQ,OFFMNG_SEQ,GELIEF_SEQ,} SeqNr;

 OffAuf_TCList(guint _cols, guint _attrs);
 const string getColTitle(guint seq) const;
 void loadOffAuf() throw(SQLerror);
 void fillDataVec() {}
 void setKdNr(Kunde::ID knr) { kundennr=knr; }
 void showOffAuf();

 TCListNode *NewNode(guint _seqnr, const cH_RowDataBase &v, guint deep);

 TCListLeaf *NewLeaf(guint _seqnr, const cH_RowDataBase &v, guint deep);

};


#endif
