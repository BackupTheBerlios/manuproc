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

#include"AufEintrag.h"
#include<stdio.h>
#include<TCList.h>
#include<deque>
#include<deque>
#include<Aux/EntryValue.h>
#include<Artikel/ArtikelBase.h>


#ifndef MYROWH
#define MYROWH
class MyRow
{ 
 TCListRow *tclistrow;
 ArtikelBase artbase;
 bool leaf;
 long summeter;
 long sumstueck;
 cH_Prozess prozess;
 Petig::Datum prozessdatum;
 EntryValue value;
 int seqnr;
 bool toshow;
 int auftragid;
 string youraufnr;
 int zeilennr;
 friend class MainTree;

public: 

void insertAufEintrag(const AufEintrag &entry, deque<int> &seq, int deep=0);
vector<string> &getColEntries(int seqdeep);
const string getColText() const;
const string &getStkSumText() const;
const string Description(const cH_ExtBezSchema &s) const;

const ArtikelBase::ID getArtID() const 
{ return artbase.Id();
}

void updateSummen();
void stutzeBaum(TCList &tcl, int deep, int showdeep);

MyRow(EntryValue v, bool _leaf)
	: prozess(Prozess::default_id)
{
 leaf = _leaf;
 summeter = 0;
 sumstueck = 0;
 value = v;
 toshow = false;
 tclistrow=NULL;
 auftragid=0;
}

EntryValue getValue() { return value; }
int getAuftragid() { return auftragid;}
int getZeilennr() { return zeilennr;}
bool Leaf() { return leaf; }
void drucken(FILE *f,vector<int>&len,const vector<int>&maxlen);
string const ProzessText() const {return prozess->getTyp()+" "+prozess->getText();}
void setText(int col, const string s)
	{tclistrow->relabel(col,s);}

};

#endif MYROWH
