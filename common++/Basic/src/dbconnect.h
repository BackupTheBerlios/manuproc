// $Id: dbconnect.h,v 1.1 2001/04/23 08:11:59 christof Exp $
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
    bool filled;
    string host;
    bool hfilled;
    string dbase;
    bool dfilled;
    string user;
    bool ufilled;
    
    public:
    Connection():filled(false),hfilled(false),dfilled(false),ufilled(false)
    {};
    Connection(const string h, const string d, const string u)
        	: filled(true),
        	host(h),hfilled(false),
        	dbase(d),dfilled(false),
        	user(u),ufilled(false) {}
        	
    const string Host() const { return host; }
    const string Dbase() const { return dbase; }
    const string User() const { return user; }
    bool Filled() const { return filled; }
    bool hFilled() const { return hfilled; }
    bool dFilled() const { return dfilled; }
    bool uFilled() const { return ufilled; }
    void setHost(const string h) { filled=hfilled=true; host=h; }
    void setDbase(const string d) { filled=dfilled=true; dbase=d; }
    void setUser(const string u) { filled=ufilled=true; user=u; }
   };

   void dbconnect_nt(const Connection &c=Connection()) throw();
   void dbdisconnect_nt() throw();  
   void dbconnect(const Connection &c=Connection()) throw(SQLerror);
   void dbdisconnect() throw(SQLerror);
   void setDTstyle(char *style="ISO") throw(SQLerror);
   void dbdefault() throw(SQLerror);
};
