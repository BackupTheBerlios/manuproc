// $Id: dbconnect.h,v 1.15 2004/11/18 16:45:44 christof Exp $
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

#include <Misc/SQLerror.h>
#ifdef MPC_SQLITE
struct sqlite3;
#endif

#define POSTGRESQL_PORT	5432

namespace ManuProC
{

class AuthError : public std::exception
{
 std::string msg;
 
public: 
 
 ~AuthError() throw() {}
 AuthError(const std::string &m) throw() :msg(m) {}
 const std::string Msg() const { return msg; }
 
};

#ifdef MPC_SQLITE
	extern sqlite3 *db_connection;
#endif
   class Connection
   {
    std::string host;
    std::string dbase;
    std::string user;
    std::string name;
    int port;
    
    public:
     Connection(const std::string &h="", const std::string &d="", 
                const std::string &u="",const std::string &n="",
                const int p=POSTGRESQL_PORT);
        	
        	
    const std::string Host() const { return host; }
    const std::string Dbase() const { return dbase; }
    const std::string User() const { return user; }
    const std::string Name() const { return name; }
    const std::string Pass() const throw(AuthError);
    const int Port() const { return port; }
    void setHost(const std::string &h) { if(!h.empty()) host=h; }
    void setDbase(const std::string &d) { if(!d.empty()) dbase=d; }
    void setUser(const std::string &u) { user=u; }
    void setName(const std::string &n) { name=n; }
    void setPort(const int p) { port=p;}
    // alternative API:
    void Host(const std::string &h) { host=h; }
    void Dbase(const std::string &d) { dbase=d; }
    void User(const std::string &u) { user=u; }
    void Name(const std::string &n) { name=n; }
    void Port(const int p) { port=p; }
   };

   void dbconnect_nt(const Connection &c=Connection()) throw();
   void dbdisconnect_nt(const std::string &name="") throw();  
   void dbconnect(const Connection &c=Connection()) throw(SQLerror);
   void dbdisconnect(const std::string &name="") throw(SQLerror);
   void setDTstyle(char *style="ISO") throw(SQLerror);
   void dbdefault(const std::string &name="") throw(SQLerror);
};

namespace Petig
{  using namespace ManuProC;
};
