// $Id: time_t_ostr.cc,v 1.1 2001/04/23 08:11:59 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#include <c++ostream.h>

ostream &operator<<(ostream &o,const time_t_wrapper &t)
{  struct tm *tm=localtime(&(time_t&)t);
   return o << *tm;
}

ostream &operator<<(ostream &o,const struct tm &tm)
{  return o << tm.tm_year+1900 << "-" << tm.tm_mon+1 << "-" << tm.tm_mday << " " 
	<< tm.tm_hour << ":" << tm.tm_min << ":" << tm.tm_sec << "/"
	<< tm.tm_isdst << "." << tm.tm_yday << "." << tm.tm_wday;
}
