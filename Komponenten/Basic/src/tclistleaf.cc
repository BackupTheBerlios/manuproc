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

#include"tclistleaf.h"

const std::vector<std::string> TCListLeaf::getColEntries(const TreeBase &tb) const
{
 std::vector<std::string> v(tb.Cols());

 for(guint i=0; i<(guint)deep; ++i)  v[i]="";

 v[deep]=value->getStrVal();

 for(guint i=deep+1;i<(guint)tb.Cols();++i) 
    v[i]=LeafData()->Value(i,tb.ValueData())->getStrVal();

 return v;
}
