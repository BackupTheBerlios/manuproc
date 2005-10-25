/* $Id: SelectedFullAufList.h,v 1.2 2005/10/25 12:13:59 christof Exp $ */
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
#include<Misc/SQLerror.h>
//#include<vector>
#include <list>
#include"Auftrag/AufEintrag.h"
#include"Auftrag/AuftragBase.h"
#include<ExtBezSchema/ExtBezSchema.h>
#include <Misc/compiler_ports.h>

// Warum nicht ableiten?
class SelectedFullAufList
{
 typedef std::list<AufEintrag> TYP ;
 std::string order_clausel;

public:
 typedef TYP::const_iterator const_iterator;
 typedef TYP::const_reverse_iterator const_reverse_iterator;
 typedef TYP::iterator iterator;
 typedef TYP::reverse_iterator reverse_iterator;
 
 static void loop(const SQLFullAuftragSelector &sel,void (*fn)(void*,AufEintrag&),void *ptr);
 static void genQuery(Query &q, const SQLFullAuftragSelector &selector);

protected:
 TYP aufidliste;
 
public:
 template <class T> void remove(const T &t) { aufidliste.remove(t); }
 
 template <class T>
  static void loop(const SQLFullAuftragSelector &sel,const T &t, void (T::*fn)(AufEintrag&))
 // hopefully this cast orgy is portable (it assumes that a member function 
 //  gets this as the first argument)
 { loop(sel,(void (*)(void*,AufEintrag&))fn, (void*)&t);
 }

 SelectedFullAufList(const SQLFullAuftragSelector &sel)	throw(SQLerror);
 void insert(const AufEintragBase& aeb) throw(SQLerror);
 const_iterator begin() const { return aufidliste.begin(); }
 const_iterator end() const { return aufidliste.end(); }
 const_reverse_iterator rbegin() const { return aufidliste.rbegin(); }
 const_reverse_iterator rend() const { return aufidliste.rend(); }
 iterator begin() { return aufidliste.begin(); }
 iterator end()  { return aufidliste.end(); }
 reverse_iterator rbegin(){ return aufidliste.rbegin(); }
 reverse_iterator rend()  { return aufidliste.rend(); }
 // please use size()!
 __deprecated int getLen() const { return aufidliste.size();}
 size_t size() const { return aufidliste.size();}
 bool empty() const {return aufidliste.empty();}
 void splice(iterator __position, SelectedFullAufList& __x) {aufidliste.splice(__position,__x.aufidliste);}
 TYP::const_reference back() const { return aufidliste.back(); }
 
 void Restrict(const AuftragBase &restriction);
};

#endif
