/* $Id: selFullAufEntry.h,v 1.1 2001/04/23 08:11:58 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
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

#include"Auftrag/sqlAuftragSelector.h"
#include"Aux/SQLerror.h"
#include<vector>
#include"Auftrag/AufEintragBase.h"
#include<ExtBezSchema/ExtBezSchema.h>

#ifndef SELECTEDFULLAUFLIST
#define SELECTEDFULLAUFLIST


class SelectedFullAufList
{
public:
 vector<AufEintragBase> aufidliste;

 SelectedFullAufList(const SQLFullAuftragSelector &sel)	throw(SQLerror);
 SelectedFullAufList(const SQLFullAuftragSelector &sel,	cH_ExtBezSchema es) throw(SQLerror);
 int getLen() { return aufidliste.size();}
 void insert(int aid, int znr) throw(SQLerror);
// void delEntry(int aid, int znr);
// void delAuftrag(int aid);
// void addList(const SQLFullAuftragSelector &sel) throw(SQLerror);
};

#endif
