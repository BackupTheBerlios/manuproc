/* $Id: EntryValueIntString.cc,v 1.8 2002/09/02 13:04:03 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2000-2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <EntryValueIntString.h>
//#include <string0.h>
#include <cctype>
#include <Misc/itos.h>

EntryValueIntString::EntryValueIntString(int v)
	: intval(v), strval(itos(v))
{ }

EntryValueIntString::EntryValueIntString(const std::string &s)
	: strval(s)
   {unsigned char *x=(unsigned char*)s.c_str();
    while (isspace(*x)) ++x;
    if (!isdigit(*x)) intval=int_NaN;
    else intval=atoi(s.c_str());
   }

