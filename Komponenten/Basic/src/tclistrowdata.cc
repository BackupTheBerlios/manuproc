/*  libKomponenten: GUI components for ManuProC's libcommon++
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


#include"tclistrowdata.h"
#include<stdio.h>

bool TCListRowData::operator==(const TCListRowData &v) const
{
 if((*value) == *(v.Value())) return true;
 return false;
}

bool TCListRowData::operator<(const TCListRowData &v) const
{
 if((*value) < *(v.Value())) return true;
 return false;
}


const string TCListRowData::getColText()
{
 return value->getStrVal();
}

const vector<string> TCListRowData::getColEntries(int treecols)
{
 vector<string> v;

// das kann man aber zu 2 Zeilen zusammenfassen CP 2001-3-2
 for(int i=0; i<deep; ++i)  v.push_back("");

 v.push_back(value->getStrVal());

 for(int i=deep+1;i<treecols;++i) 
   v.push_back("");

 return v;
}

TCListRowData::TCListRowData(const cH_EntryValue &v, int _deep, bool _leaf)
	: value(v), deep(_deep),leaf(_leaf)
{}


