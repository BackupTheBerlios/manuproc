/* $Id: TemporaryValue.h,v 1.2 2003/09/02 15:48:59 christof Exp $ */
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

// exception safe way to temporarily change a value of a variable

#ifndef _TEMPORARYVALUE_H
#define _TEMPORARYVALUE_H
template <class T>
 class TemporaryValue
{	T old_value;
	T &variable;
public:
	TemporaryValue(T &var,const T &value)
		: old_value(var), variable(var)
	{  var=value; }
	~TemporaryValue()
	{  variable=old_value; }
};
#endif
