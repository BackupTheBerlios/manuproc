// $Id: Transaction.h,v 1.1 2001/04/23 08:11:59 christof Exp $
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


#ifndef CXX_AUX_TRANSACTION_H
#define CXX_AUX_TRANSACTION_H
#include <Aux/SQLerror.h>

// default is to roll back on close
class Transaction
{	bool owner:1;
	bool commit_vs_rollback:1;
	string connection;
public:
	Transaction(const string &connection="",bool open_now=true) throw(SQLerror);
//	Transaction(bool open_now) throw(SQLerror); // old ctor
	// attention: if you specify no connection its last value is used
	//            e.g. by last open or ctor
	// I feel this is the most intuitive behaviour
	void open(const string &connection="") throw(SQLerror);
	void close() throw(SQLerror);
	void commit_on_close(bool val=true)
	{  commit_vs_rollback=val; }
	void rollback_on_close(bool val=true)
	{  commit_vs_rollback=!val; }
	void rollback()
	{  rollback_on_close(); close(); }
	void commit()
	{  commit_on_close(); close(); }
	~Transaction();
};
#endif
