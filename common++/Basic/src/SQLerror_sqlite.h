/* $Id: SQLerror_sqlite.h,v 1.4 2004/05/07 09:48:23 jacek Exp $ */
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

#ifndef SQLERROR_POSTGRES_H
#define SQLERROR_POSTGRES_H
#ifdef __cplusplus
#include <iostream>
#include <string>
#include <exception>
#include <Misc/compiler_ports.h>
#endif

#define _sql_STRING2__(a) #a
#define _sql_STRING__(a) _sql_STRING2__(a)
#define __FILELINE__ __FILE__":"_sql_STRING__(__LINE__)

#ifdef __cplusplus
class SQLerror_sqlite : public std::exception
{  std::string context;
   int code;
   std::string name;
//   char separator;
   
public:
   static int last_code;
   
   ~SQLerror_sqlite() throw() {}
//   SQLerror_sqlite(const std::string &context) throw();
   SQLerror_sqlite(const std::string &context,int code,const std::string &name) throw();
   
   virtual const char* what() const throw() { return "SQLerror"; }
   friend std::ostream &operator<<(std::ostream&,const SQLerror_sqlite &) throw();
   
//   const SQLerror_sqlite &Separator(char sep) { separator=sep; return *this; }
   // member access functions
   const std::string Context() const { return context; }
   int Code() const { return code; }
   const std::string Message() const { return name; }

   static void print(const std::string &context,int codeok=0);
   
   static __deprecated void test(const std::string &context,int codeok=0);
//   		throw(SQLerror_sqlite);
   static __deprecated void test(const std::string &context,const std::string &cursor,
		int codeok=0); // throw(SQLerror_sqlite);
   static __deprecated void test(const std::string &context,const char *cursor,
		int codeok=0) // throw(SQLerror_sqlite)
   {  test(context,std::string(cursor),codeok);
   }
   
//   static __deprecated int SQLCode();
};

std::ostream &operator<<(std::ostream&,const SQLerror_sqlite &) throw();
#endif
#endif
