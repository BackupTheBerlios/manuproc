/* $Id: */
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


#ifndef LIEFERSCHEINVOLL
#define LIEFERSCHEINVOLL

#include"Lieferschein.h"
#include"LieferscheinEntry.h"
#include<vector>
#include <Aux/SQLerror.h>


class LieferscheinVoll : public Lieferschein
{
 vector<LieferscheinEntry> lsentry;
 
public:
 
        typedef vector<LieferscheinEntry>::const_iterator const_iterator;

 	LieferscheinVoll() : Lieferschein() {};
 	LieferscheinVoll(int lid) throw(SQLerror) ;
 	void deleteRow(const LieferscheinEntry &le);

 const vector<LieferscheinEntry> &LsEntries() { return lsentry; }

 const_iterator begin() const
 { return lsentry.begin();} 
 const_iterator end() const  
 { return lsentry.end();}   
 size_t size() const
 { return lsentry.size();}
};

#endif

