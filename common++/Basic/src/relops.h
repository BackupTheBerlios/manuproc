/* $Id: relops.h,v 1.2 2002/11/07 07:49:38 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

// sometimes it is handy to have the std:: functions from stl_relops.h
// in global namespace

// please do not include this file in any header file !!!!!

#ifndef MANUPROC_RELOPS_H
#define MANUPROC_RELOPS_H

#include <utility>
#if __GNUC__ > 2
using namespace std::rel_ops;
#endif

#endif
