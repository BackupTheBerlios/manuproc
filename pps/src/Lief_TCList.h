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

#ifndef LIEFERSCHEINTCLIST
#define LIEFERSCHEINTCLIST

#include<treebase.h>
#include<Lieferschein/LieferscheinVoll.h>

class Lief_TCList : public TreeBase
{

 LieferscheinVoll lieferschein;
 
public:

 typedef enum {LIEFZEILE_SEQ,ARTIKEL_SEQ,AUFNR_SEQ,LIEFMNG_SEQ} SeqNr;

 Lief_TCList(guint _cols, guint _attrs);
 const std::string getColTitle(guint seq) const;
 void fillDataVec() {}
 void showLieferschein(LieferscheinBase::ID lfrsid); 
 void newLieferschein(const Kunde::ID kid);
 const LieferscheinVoll &getLieferschein() const { return lieferschein; }
 LieferscheinVoll &getLieferschein() { return lieferschein; }
 void setLieferschein(const LieferscheinBase::ID lsid) 
 			{ showLieferschein(lsid); }
 			
 bool deleteLiefEntry();

TreeRow *NewNode(guint deep, const cH_EntryValue &v, bool expand);

};

#include<tclistnode.h>

class Lief_Node : public TreeRow
{
 float sumgeliefert;
 
public:
 
 virtual void cumulate(const cH_RowDataBase &rd);
 const cH_EntryValue Value(guint index,gpointer gp) const;

 Lief_Node::Lief_Node(guint deep, const cH_EntryValue &v, bool expand);
 float SumGeliefert() const { return sumgeliefert; }

};

#endif
