/* $Id: EntryValueBase.cc,v 1.7 2002/09/02 13:04:03 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/EntryValueBase.h>

EntryValueBase::operator bool() const
{  return getIntVal()!=int_NaN || getStrVal()!="";
}

bool EntryValueBase::operator==(const EntryValueBase &v) const
{  int i(getIntVal());
   int j(v.getIntVal());

   if(i!=int_NaN && j!=int_NaN)
     {
      if(i!=j) return false;
      else return getStrVal()==v.getStrVal();
     }

   return getStrVal()==v.getStrVal();
}

bool EntryValueBase::operator<(const EntryValueBase &v) const
{  int i(getIntVal()),j(v.getIntVal());
   if (i!=int_NaN && j!=int_NaN && i!=j) return i<j;
   return getStrVal()<v.getStrVal();
}

