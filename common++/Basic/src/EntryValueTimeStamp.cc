/* $Id: EntryValueTimeStamp.cc,v 1.1 2003/03/19 08:27:03 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/EntryValueTimeStamp.h>
#include <typeinfo> // for bad_cast

bool EntryValueTimeStamp::operator==(const EntryValueBase &v) const
{  try
   {  const EntryValueTimeStamp &b=dynamic_cast<const EntryValueTimeStamp &>(v);
      if (!ts.valid()) return !b.ts.valid();
      return ts==b.ts;
   } catch (std::bad_cast &e)
   {  return false;
   }
}

bool EntryValueTimeStamp::operator<(const EntryValueBase &v) const
{  try
   {  const EntryValueTimeStamp &b=dynamic_cast<const EntryValueTimeStamp &>(v);
      if (!ts.valid()) return false;
      if (b.ts.valid()) return (ts<b.ts);
      else return true;
   } catch (std::bad_cast &e)
   {  return false;
   }
}

#if __GNUC__ > 2 // else I don't care (yet)
#include <sstream>

const std::string EntryValueTimeStamp::getStrVal() const
{  if (!ts.valid()) return "";
   std::ostringstream os;
   os << ts;
   return os.str();
}
#endif
