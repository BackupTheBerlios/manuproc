// $Id: dbconnect_shared.cc,v 1.1 2004/03/11 12:17:32 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <ManuProCConfig.h>
#include <Misc/dbconnect.h>

ManuProC::Connection::Connection(const std::string &h, const std::string &d, const std::string &u,const std::string &n)
        	: host(h), dbase(d), user(u), name(n)
{
   std::string host_=DEFAULT_DBHOST;
   std::string db_=DEFAULT_DB;

   char *h_opt(getenv("PGHOST"));
   char *d_opt(getenv("PGDATABASE"));
   if(h_opt) host_=h_opt;
   if(d_opt) db_=d_opt;

   if(host.empty())  host=host_;
   if(dbase.empty()) dbase=db_;
}

void ManuProC::dbconnect_nt(const Connection &c) throw()
{  try { dbconnect(c); }
   catch (SQLerror &e)
   {  std::cerr << e << '\n';
   }
}

void ManuProC::dbdisconnect_nt(const std::string &name) throw()
{  try { dbdisconnect(name); }
   catch (SQLerror &e)
   {  std::cerr << e << '\n';
   }
}

#ifdef MPC_SQLITE
#include <sqlite.h>
#include <Misc/Global_Settings.h>

struct sqlite *ManuProC::db_connection;

void ManuProC::dbconnect(const Connection &c) throw(SQLerror)
{  assert(!db_connection);

//   char *opt(getenv("SQLOPT"));
   char *zErrMsg = 0;
   db_connection=sqlite_open(c.Dbase().c_str(),0,&zErrMsg);
   // don't we have to free zErrMsg?
   if (!db_connection) throw(SQLerror("dbconnect",-1,zErrMsg));
   Global_Settings::database_connected();
}

void ManuProC::dbdisconnect(const std::string &name) throw(SQLerror)
{  Global_Settings::database_connected(false);
   assert(db_connection);
   sqlite_close(db_connection);
   db_connection=0;
}

#endif
