/* $Id: selFullAufEntry.h,v 1.10 2002/01/22 09:15:55 christof Exp $ */
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

#ifndef SELECTEDFULLAUFLIST
#define SELECTEDFULLAUFLIST

#include"Auftrag/sqlAuftragSelector.h"
#include"Aux/SQLerror.h"
#include<vector>
#include"Auftrag/AufEintragBase.h"
#include"Auftrag/AuftragBase.h"
#include<ExtBezSchema/ExtBezSchema.h>


class SelectedFullAufList
{

public:
 typedef std::vector<AufEintragBase>::const_iterator const_iterator;
 typedef std::vector<AufEintragBase>::iterator iterator;
 std::vector<AufEintragBase> aufidliste;

 SelectedFullAufList(const SQLFullAuftragSelector &sel)	throw(SQLerror);
 void insert(const AufEintragBase2& aeb) throw(SQLerror);
 const_iterator begin() const { return aufidliste.begin(); }
 const_iterator end() const { return aufidliste.end(); }
 iterator begin() { return aufidliste.begin(); }
 iterator end()  { return aufidliste.end(); }
 // please use size()!
 int getLen() const { return aufidliste.size();}
 size_t size() const { return aufidliste.size();}
 
};

#endif
