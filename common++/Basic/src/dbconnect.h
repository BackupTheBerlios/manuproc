// $Id: dbconnect.h,v 1.2 2001/06/06 07:27:39 christof Exp $
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

#include <Aux/SQLerror.h>

namespace Petig
{
   class Connection
   {
    string host;
    string dbase;
    string user;
    string name;
    
    public:
    Connection(const string &h="", const string &d="", const string &u="",const string &n="")
        	: host(h), dbase(d), user(u), name(n)
        	{}
        	
    const string Host() const { return host; }
    const string Dbase() const { return dbase; }
    const string User() const { return user; }
    const string Name() const { return name; }
    void setHost(const string &h) { host=h; }
    void setDbase(const string &d) { dbase=d; }
    void setUser(const string &u) { user=u; }
    void setName(const string &n) { name=n; }
    // alternative API:
    void Host(const string &h) { host=h; }
    void Dbase(const string &d) { dbase=d; }
    void User(const string &u) { user=u; }
    void Name(const string &n) { name=n; }
   };

   void dbconnect_nt(const Connection &c=Connection()) throw();
   void dbdisconnect_nt(const string &name="") throw();  
   void dbconnect(const Connection &c=Connection()) throw(SQLerror);
   void dbdisconnect(const string &name="") throw(SQLerror);
   void setDTstyle(char *style="ISO") throw(SQLerror);
   void dbdefault(const string &name="") throw(SQLerror);
};
