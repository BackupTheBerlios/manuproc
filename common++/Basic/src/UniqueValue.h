// $Id: UniqueValue.h,v 1.1 2002/12/10 09:05:48 christof Exp $            
/*  UniqueValue
 *  Copyright (C) 2002 Christof Petig
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

#ifndef UNIQUEVALUE_H
#define UNIQUEVALUE_H

// this class is designed to be used as a static varible

// since static ctor order is undefined we need to not initialize the
// value in _our_ ctor (since it might already have been incremented)

class UniqueValue
{public:
	typedef unsigned value_t;

private:
	int next_value;

public:
	int get()
	{  return next_value++;
	}
};

#endif
