/* $Id: SQLerror_postgres.h,v 1.1 2001/04/23 08:11:59 christof Exp $ */
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
#endif

#define _sql_STRING2__(a) #a
#define _sql_STRING__(a) _sql_STRING2__(a)
#define __FILELINE__ __FILE__":"_sql_STRING__(__LINE__)

#define strcpyx(dest,src) strncpy0((dest),(src),sizeof(dest))
#define sprintfx(dest,src,val...) snprintf0((dest),sizeof(dest),src,##val)

#ifdef __cplusplus
class SQLerror_postgres : public exception
{  string context;
   int code;
   string name;
   char separator;

public:
   SQLerror_postgres(const string &context) throw();
   SQLerror_postgres(const string &context,int code,const string &name) throw();
   
   virtual const char* what() const throw() { return "SQLerror"; }
   friend ostream &operator<<(ostream&,const SQLerror_postgres &) throw();
   
   const SQLerror_postgres &Separator(char sep) { separator=sep; return *this; }
   // member access functions
   const string Context() const { return context; }
   int Code() const { return code; }
   const string Message() const { return name; }

   static void print(const string &context,int codeok=0);
   static void close_cursor(const string &c) throw();
   
   // DEPRECATED, use Transaction
   static void test(const string &context,bool rollback) throw(SQLerror_postgres);
   static void test(const string &context,int codeok=0,bool rollback=false) 
   		throw(SQLerror_postgres);
   static void test(const string &context,const string &cursor,
		int codeok=0,bool rollback=false) throw(SQLerror_postgres);
   static void rollback_and_throw(const string &context) throw(SQLerror_postgres)
   {  SQLerror_postgres err(context);
      rollback(); 
      throw(err); 
   }
   static void rollback() throw();
   // !DEPRECATED
};

ostream &operator<<(ostream&,const SQLerror_postgres &) throw();
#endif
#endif
