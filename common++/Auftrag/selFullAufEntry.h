/* $Id: selFullAufEntry.h,v 1.14 2002/05/03 10:22:54 christof Exp $ */
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
//#include<vector>
#include <list>
#include"Auftrag/AufEintrag.h"
#include"Auftrag/AuftragBase.h"
#include<ExtBezSchema/ExtBezSchema.h>


class SelectedFullAufList
{
 typedef std::list<AufEintrag> TYP ;

public:
 typedef TYP::const_iterator const_iterator;
 typedef TYP::const_reverse_iterator const_reverse_iterator;
 typedef TYP::iterator iterator;
 TYP aufidliste;
 template <class T> void remove(const T &t) { aufidliste.remove(t); }

 SelectedFullAufList(const SQLFullAuftragSelector &sel)	throw(SQLerror);
 void insert(const AufEintragBase& aeb) throw(SQLerror);
 const_iterator begin() const { return aufidliste.begin(); }
 const_iterator end() const { return aufidliste.end(); }
 const_reverse_iterator rbegin() const { return aufidliste.rbegin(); }
 const_reverse_iterator rend() const { return aufidliste.rend(); }
 iterator begin() { return aufidliste.begin(); }
 iterator end()  { return aufidliste.end(); }
 // please use size()!
 int getLen() const { return aufidliste.size();}
 size_t size() const { return aufidliste.size();}
 bool empty() const {return aufidliste.empty();}
 
};

#endif
