/* $Id: SQLerror_sqlite.cc,v 1.3 2004/05/07 09:48:23 jacek Exp $ */
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

#include <ManuProCConfig.h>
#ifdef MPC_SQLITE
#include "SQLerror_sqlite.h"
#include <iostream>
#include <string>
#include <cassert>

int SQLerror_sqlite::last_code;

SQLerror_sqlite::SQLerror_sqlite(const std::string &ctext,int _code,const std::string &_name) throw()
	: context(ctext), code(_code), name (_name)
{  }

std::ostream &operator<<(std::ostream &o,const SQLerror_sqlite &e) throw()
{  o<<e.context;
   if (e.code)
      o <<':'<<' '<<e.code<<' '<<e.name;
   return o;
}

void SQLerror_sqlite::test(const std::string &context,int codeok) // throw(SQLerror_sqlite)
{  if (last_code && last_code!=codeok) throw SQLerror_sqlite(context,last_code,"?");
}

void SQLerror_sqlite::test(const std::string &context,const std::string &cursor,
		int codeok) // throw(SQLerror_sqlite)
{  if (last_code && last_code!=codeok) 
   {  assert(cursor.empty());
      throw SQLerror_sqlite(context,last_code,"?");
   }
}

void SQLerror_sqlite::print(const std::string &context,int codeok)
{  if (last_code && last_code!=codeok)
      std::cerr << SQLerror_sqlite(context,last_code,"?") << '\n';
}

#if 0
void SQLerror_sqlite::test(const std::string &context,const std::string &cursor,
		int codeok,bool rollback) // throw(SQLerror_sqlite)
{  if (last_code && last_code!=codeok) 
   {  assert(cursor.empty());
      assert(!rollback);
      else throw SQLerror_sqlite(context);
   }
}
#endif

//int SQLerror_sqlite::SQLCode()
//{  return last_code; }

#endif
