/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include<Auftrag/selFullAufEntry.h>
#include"myRow.h"
#include<stdio.h>
#include<TCList.h>
#include<deque>
#include"auftragbase.h"

#ifndef MAINTREEH
#define MAINTREEH
class MainTree : public TCList
{
 int showdeep;
 vector<int> currseq;
 vector<int> tmpvec;
 deque<int> tmpq;

public :

void fill(SelectedFullAufList &allids, int deep=0);
void updateSum();
const string getColTitle(int attr, int seq);
void setDefaultSeq();
vector<int> &getCurrSeq() { return currseq; }

void setShowdeep(int sd) {showdeep=sd;}

MainTree(int cols);

void on_click_column(int col);

void clear(){ erase(begin(),end()); }

void updateSummen()
{
 TCListRow::iterator i = begin();
 while(i!=end()) {((MyRow*)(*i).get_user_data())->updateSummen();i++;}
} 
	
void stutzeBaum()
{
 TCListRow::iterator i = begin();
 while(i!=end())
  {((MyRow*)(*i).get_user_data())->stutzeBaum(*this,1,showdeep); i++;}
}

void drucken(FILE *f,vector<int> &len,const vector<int>&maxlen);

private:

void onrowdelete(gpointer data)
{
 if(data) delete((MyRow*)data);
}	
	
};

#endif MAINTREEH
