// $Id: ctime_assert.h,v 1.8 2005/09/13 08:43:10 christof Exp $
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

#ifndef AUX_CTIME_ASSERTION_H
#define AUX_CTIME_ASSERTION_H

struct do_not_instantiate_use_typedef {  virtual void dummy()=0; virtual ~do_not_instantiate_use_typedef()=0; };
template <bool> struct ctime_assert { typedef do_not_instantiate_use_typedef _true; };
template <> struct ctime_assert<false> {  };

// use it like 
//	typedef ctime_assert<(x>0)>::_true failed;
              
#endif
