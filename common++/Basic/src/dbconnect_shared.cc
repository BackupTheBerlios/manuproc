// $Id: dbconnect_shared.cc,v 1.9 2005/03/30 12:32:58 jacek Exp $
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
#include <cassert>

ManuProC::Connection::Connection(const std::string &h, const std::string &d, 
		const std::string &u,const std::string &n, const int p)
        	: host(h), dbase(d), user(u), name(n), port(p)
{
   std::string host_=DEFAULT_DBHOST;
   std::string db_=DEFAULT_DB;
   std::string opt_port;

   char *h_opt(getenv("PGHOST"));
   char *d_opt(getenv("PGDATABASE"));
   char *p_opt(getenv("PGPORT"));   
   if(h_opt) host_=h_opt;
   if(d_opt) db_=d_opt;
   if(p_opt) opt_port=p_opt;   

   if(host.empty())  host=host_;
   if(dbase.empty()) dbase=db_;
   if(!opt_port.empty()) port=atoi(opt_port.c_str());
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



#include <iostream.h>
#include <fstream.h>

const std::string ManuProC::Connection::Pass() const throw(AuthError)
{
// not nedeed at the time

/* char buf[80];
 buf[0]=0;

  ifstream passfile (".remote_access");
  if (! passfile.is_open())
    { throw AuthError(".remote_access not found"); } 
  if(!passfile.eof())
    passfile.getline(buf,sizeof buf);

  passfile.close();
*/
  return "";
}


#ifdef MPC_SQLITE
#include <sqlite3.h>
#include <Misc/Global_Settings.h>

struct sqlite3 *ManuProC::db_connection;

void ManuProC::dbconnect(const Connection &c) throw(SQLerror)
{  assert(!db_connection);

//   char *opt(getenv("SQLOPT"));
   int error=sqlite3_open(c.Dbase().c_str(),&db_connection);
   if (error) 
   {  std::string err=sqlite3_errmsg(db_connection);
      sqlite3_close(db_connection);
      db_connection=0;
      throw(SQLerror("dbconnect",error,err));
   }
   // sollte nicht passieren ...
   Global_Settings::database_connected();
}

void ManuProC::dbdisconnect(const std::string &name) throw(SQLerror)
{  Global_Settings::database_connected(false);
   assert(db_connection);
   sqlite3_close(db_connection);
   db_connection=0;
}

#endif
