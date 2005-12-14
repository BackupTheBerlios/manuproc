// $Id: fixedpoint_null.cc,v 1.1 2005/12/14 07:35:07 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/fixedpoint.h>
#include <Misc/Query.h>

template <> Query_types::null_s Query_types::null<fixedpoint<3,double,long> >()
{ return null<int>(); }
