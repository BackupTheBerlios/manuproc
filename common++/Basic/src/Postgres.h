// $Id: Postgres.h,v 1.3 2002/05/09 12:46:00 christof Exp $
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

#ifndef POSTGRES_HH
#define POSTGRES_HH

class PostgresTimestamp
{  	char *payload;
	int size;
	static const int DefaultSize=40; // this is a guess
public:
	PostgresTimestamp(const char *x) : payload((char*)x), size(0) {}
	PostgresTimestamp(char *x, int sz) : payload(x), size(sz) {}
	operator char*() { return payload; }
	operator const char*() const { return payload; }
	int Size() const { return size; }
};

class PostgresTime
{  	char *payload;
	int size;
	static const int DefaultSize=20; // this is a guess
public:
	PostgresTime(const char *x) : payload((char*)x), size(0) {}
	PostgresTime(char *x, int sz) : payload(x), size(sz) {}
	operator char*() { return payload; }
	operator const char*() const { return payload; }
	int Size() const { return size; }
};

class PostgresDate
{  	char *payload;
	int size;
	static const int DefaultSize=20; // this is a guess
public:
	PostgresDate(const char *x) : payload((char*)x), size(0) {}
	PostgresDate(char *x, int sz) : payload(x), size(sz) {}
	operator char*() { return payload; }
	operator const char*() const { return payload; }
	int Size() const { return size; }
};

#endif
