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

// $Id: tclistrowdata.cc,v 1.3 2001/06/29 11:30:18 christof Exp $

#include <tclistrowdata.h>
#include <treebase.h>

void TCListRowData::initTCL(TCListRow_API *api, TCListRow_API::iterator davor,
			const TreeBase &tb) // ,int deep)
{
 listrow = api->insert(davor, getColEntries(tb.Cols()),leaf?-1:deep+1,expand);
 listrow->set_user_data(this);
}

void TCListRowData::initTCL(TCListRow_API *api, const TreeBase &tb) // ,int deep)
{
 listrow = api->push_back(getColEntries(tb.Cols()),leaf?-1:deep+1,expand);
 listrow->set_user_data(this);
}
