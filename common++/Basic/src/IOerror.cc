// $Id: IOerror.cc,v 1.8 2003/01/08 10:18:51 christof Exp $
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

/* $Id: IOerror.cc,v 1.8 2003/01/08 10:18:51 christof Exp $ */
#include <Misc/IOerror.h>
#include <errno.h>
#include <Misc/Ausgabe_neu.h>

IOerror::IOerror() throw() : IOerrno(errno)
{}

IOerror::IOerror(const std::string &n) throw() : IOerrno(errno), name(n)
{}

IOerror::IOerror(const std::string &n,int e) throw() : IOerrno(e), name(n)
{}

std::string IOerror::toString() const throw()
{  std::string r=name;
   if (!name.empty()) r+=':';
   r+=strerror(IOerrno);
   return r;
}
