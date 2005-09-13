/* $Id: ManuProcEntity.h,v 1.11 2005/09/13 08:43:12 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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



#ifndef MANU_PROC_ENTITY
#define MANU_PROC_ENTITY

#include <Misc/itos.h>

template <class T=long int> class ManuProcEntity
{
public:
 typedef T ID;
 
protected:
 ID entityid;
 
public:
 static const ID none_id=static_cast<ID>(-1);
 ManuProcEntity() : entityid(none_id) {}
 ManuProcEntity(const ID i) : entityid(i) {}
 virtual ID Id() const = 0;
 virtual void setId(ID _id) { entityid=_id;}
 virtual ~ManuProcEntity() {}
 
// static bool Valid(ID) ;//const; // wenn die Klassen von *this abgeleitet werden
                            // soll das 'statis' verschwinden
   static bool Valid(ID i) {
      if(i==none_id) return false;
      return true;
      }

 std::string strId() const { return itos0pad(entityid,6);}

};

#endif
