// $Id: fixedpoint_null.cc,v 1.4 2005/12/14 07:35:34 christof Exp $
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

template<>
const Oid Query::NullIf_s<fixedpoint<3,double,long> >::postgres_type=Query::NullIf_s<int>::postgres_type;
template<>
const Oid Query::NullIf_s<fixedpoint<2,double,long> >::postgres_type=Query::NullIf_s<int>::postgres_type;
template<>
const Oid Query::NullIf_s<fixedpoint<5,double,long> >::postgres_type=Query::NullIf_s<int>::postgres_type;
