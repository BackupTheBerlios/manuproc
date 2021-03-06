/* $Id: EntryValueSort.cc,v 1.3 2004/02/10 10:51:54 christof Exp $ */
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

#include <Misc/EntryValueSort.h>
#include <typeinfo> // for bad_cast

bool EntryValueSort::operator==(const EntryValueBase &v) const
{  try
   {  const EntryValueSort &b=dynamic_cast<const EntryValueSort &>(v);
      return sort==b.sort && value==b.value;
   } catch (std::bad_cast &e)
   {  return false;
   }
}

bool EntryValueSort::operator<(const EntryValueBase &v) const
{  try
   {  const EntryValueSort &b=dynamic_cast<const EntryValueSort &>(v);
      return sort<b.sort || (sort==b.sort && value<b.value);
   } catch (std::bad_cast &e)
   {  return false;
   }
}

bool EntryValueReverseSort::operator==(const EntryValueBase &v) const
{  try
   {  const EntryValueReverseSort &b=dynamic_cast<const EntryValueReverseSort &>(v);
      return sort==b.sort && value==b.value;
   } catch (std::bad_cast &e)
   {  return false;
   }
}

bool EntryValueReverseSort::operator<(const EntryValueBase &v) const
{  try
   {  const EntryValueReverseSort &b=dynamic_cast<const EntryValueReverseSort &>(v);
// perhaps this works better ...   
      return b.sort<sort || (sort==b.sort && value<b.value);
//      (!(sort==b.sort) && !(sort<b.sort)) || (sort==b.sort && value<b.value);
   } catch (std::bad_cast &e)
   {  return false;
   }
}

