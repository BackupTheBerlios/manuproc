// $Id: dbconnect.h,v 1.8 2002/06/27 07:26:10 christof Exp $
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

namespace ManuProC
{
   class Connection
   {
    std::string host;
    std::string dbase;
    std::string user;
    std::string name;
    
    public:
    Connection(const std::string &h="", const std::string &d="", const std::string &u="",const std::string &n="")
        	: host(h), dbase(d), user(u), name(n)
        	{}
        	
    const std::string Host() const { return host; }
    const std::string Dbase() const { return dbase; }
    const std::string User() const { return user; }
    const std::string Name() const { return name; }
    void setHost(const std::string &h) { host=h; }
    void setDbase(const std::string &d) { dbase=d; }
    void setUser(const std::string &u) { user=u; }
    void setName(const std::string &n) { name=n; }
    // alternative API:
    void Host(const std::string &h) { host=h; }
    void Dbase(const std::string &d) { dbase=d; }
    void User(const std::string &u) { user=u; }
    void Name(const std::string &n) { name=n; }
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
