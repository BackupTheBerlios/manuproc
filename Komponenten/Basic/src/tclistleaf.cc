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



TCListLeaf::TCListLeaf(int _seqnr, const cH_RowDataBase &v, int deep)
	: TCListRowData(v->Value(_seqnr),deep,true),leafdata(v)
{}

void TCListLeaf::initTCL(TCListRow_API *api, TCListRow_API::iterator davor,
			const TreeBase &tb,int deep)
{
 listrow = api->insert(davor,getColEntries(tb.Cols()),-1,show);
 listrow->set_user_data(this);
}

void TCListLeaf::initTCL(TCListRow_API *api, const TreeBase &tb,int deep)
{
 listrow = api->push_back(getColEntries(tb.Cols()),-1,show);
 listrow->set_user_data(this);
}

