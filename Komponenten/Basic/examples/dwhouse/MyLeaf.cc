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

#include<Aux/Ausgabe_neu.h>
#include"MyLeaf.h"
#include"MyRowData.hh"



const vector<string> MyLeaf::getColEntries(int cols)
{
 static vector<string> v;

 v=TCListRowData::getColEntries(cols);

 v[3]=Formatiere((dynamic_cast<const MyRowData &>(*leafdata)).Data1());
 v[4]=Formatiere((dynamic_cast<const MyRowData &>(*leafdata)).Data2());

 cout << "getColEntries Leaf\n";
 return v;

}


