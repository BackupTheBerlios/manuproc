// $Id: FetchIStream_common.cc,v 1.1 2004/03/11 14:35:53 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/FetchIStream.h>
#include <Misc/SQLerror.h>

#include <Misc/itos.h>
#include <locale.h>
#include <cassert>

#ifdef OLD_ECPG
#define ECPGdo300
#else
#define ECPGdo300 0,1,
#endif

Query::debug_environment::debug_environment() : on(false)
{  if (getenv("SQLOPT") && getenv("SQLOPT")==std::string("-E")) on=true;
}

static Query::debug_environment debugging;

void FetchIStream::mythrow(const SQLerror &e)
{  if (Query::debugging.on) std::cerr << e << '\n';
   throw e;
}

#define DEBUG_FIS(x) std::cerr << x << '\n'

FetchIStream &FetchIStream::operator>>(char &c)
{  std::string s;
   *this >> s;
   if (s.size()>1) mythrow(SQLerror("FetchIStream>>char&",ECPG_DATA_NOT_ARRAY,"data too long"));
   c=*s.c_str();
   return *this;
}

FetchIStream &FetchIStream::operator>>(int &i)
{  std::string s;
   *this >> s;
   i=strtol(s.c_str(),0,10);
   return *this;
}

FetchIStream &FetchIStream::operator>>(long &i)
{  std::string s;
   *this >> s;
   i=strtol(s.c_str(),0,10);
   return *this;
}

#ifndef __MINGW32__
FetchIStream &FetchIStream::operator>>(long long &i)
{  std::string s;
   *this >> s;
   i=strtoll(s.c_str(),0,10);
   return *this;
}
#endif

FetchIStream &FetchIStream::operator>>(unsigned &i)
{  std::string s;
   *this >> s;
   i=strtoul(s.c_str(),0,10);
   return *this;
}

FetchIStream &FetchIStream::operator>>(unsigned long &i)
{  std::string s;
   *this >> s;
   i=strtoul(s.c_str(),0,10);
   return *this;
}

FetchIStream &FetchIStream::operator>>(float &f)
{  double d=0;
   *this >> d;
   f=d;
   return *this;
}

FetchIStream &FetchIStream::operator>>(double &f)
{  std::string s;
   *this >> s;
   /* Make sure we do NOT honor the locale for numeric input */
   /* since the database gives the standard decimal point */
   std::string oldlocale= setlocale(LC_NUMERIC, NULL);
   setlocale(LC_NUMERIC, "C");
   f=strtod(s.c_str(),0);
   setlocale(LC_NUMERIC, oldlocale.c_str());
   return *this;
}

FetchIStream &FetchIStream::operator>>(bool &b)
{  std::string s;
   *this >> s;
   b=s[0]=='t' || s[0]=='T' || s[0]=='1';
   return *this;
}

// ============================ Query =======================

// copied from ecpg/lib/execute.c
static const char *next_insert(const char *text)
{   const char       *ptr = text;
    bool        string = false;

    for (; *ptr != '\0' && (*ptr != '?' || string); ptr++)
    {   if (*ptr == '\\')       /* escape character */
            ptr++;
        else if (*ptr == '\'')
            string = string ? false : true;
    }

    return (*ptr == '\0') ? 0 : ptr;
}

void Query::Execute_if_complete()
{  if (params.complete())
   {  std::string expanded;
      const char *p=query.c_str();
      const char *last=p;
      ArgumentList::const_iterator piter=params.begin();
      
      do
      {  p=next_insert(p);
         if (!p) expanded+=last;
         else
         {  expanded+=std::string(last,p-last);
            assert(piter!=params.end());
            expanded+=*piter;
            ++p;
            ++piter;
            last=p;
         }
      } while(p);
      query=expanded;
      Execute();
   }
}

Query &Query::add_argument(const std::string &s)
{  params.add_argument(s);
   Execute_if_complete();
   return *this;
}

ArgumentList &ArgumentList::add_argument(const std::string &x)
{  if (complete())
      FetchIStream::mythrow(SQLerror("",ECPG_TOO_MANY_ARGUMENTS,"too many arguments"));
   params.push_back(x);
   --params_needed;
   return *this;
}

ArgumentList &ArgumentList::operator<<(const std::string &str)
{  std::string p="'";
   for (std::string::const_iterator i=str.begin();i!=str.end();++i)
   {  if (*i=='\'' || *i=='\\') p+=*i;
      p+=*i;
   }
   p+='\'';
   return add_argument(p);
}

ArgumentList &ArgumentList::operator<<(long i)
{  return add_argument(itos(i));
}

ArgumentList &ArgumentList::operator<<(unsigned long i)
{  return add_argument(ulltos(i));
}

ArgumentList &ArgumentList::operator<<(unsigned long long i)
{  return add_argument(ulltos(i));
}

ArgumentList &ArgumentList::operator<<(double i)
{  return add_argument(dtos(i));
}

ArgumentList &ArgumentList::operator<<(bool i)
{  return add_argument(btos(i));
}

ArgumentList &ArgumentList::operator<<(char i)
{  char x[4];
   x[0]='\'';
   x[1]=i;
   x[2]='\'';
   x[3]=0;
   return add_argument(x);
}

ArgumentList &ArgumentList::operator<<(const ArgumentList &list)
{  for (const_iterator i=list.begin();i!=list.end();++i) 
      add_argument(*i);
   return *this;
}

FetchIStream &Query::Fetch()
{  Fetch(embedded_iterator);
   return embedded_iterator;
}

Query &Query::operator>>(FetchIStream &s)
{  Fetch(s);
   return *this;
}

FetchIStream &Query::FetchOne()
{  ThrowOnBad(__FUNCTION__);
   Fetch(embedded_iterator);
   FetchIStream dummy;
   Fetch(dummy);
   if (good()) FetchIStream::mythrow(SQLerror(__FUNCTION__,-2,"more than one result"));
   return embedded_iterator;
}

void Query::ThrowOnBad(const char *where) const
{  if (!good()) 
   {  SQLerror::test(__FUNCTION__); 
      FetchIStream::mythrow(SQLerror(__FUNCTION__,-1,"unspecific bad result")); 
   }
}

// ====================== SQLite =================================

#ifdef MPC_SQLITE
FetchIStream::FetchIStream(const char *const *res=0, unsigned _nfields, int line=0)
	: naechstesFeld(), zeile(line), result(res), nfields(_nfields)
{}

FetchIStream &FetchIStream::operator>>(std::string &str)
{  if (!result)
	mythrow(SQLerror(__FUNCTION__,ECPG_UNKNOWN_DESCRIPTOR,"no result to fetch from (left?)"));
   if (naechstesFeld>=nfields) 
	mythrow(SQLerror(__FUNCTION__,ECPG_INVALID_DESCRIPTOR_INDEX,"reading beyond line end"));
   if (!result[naechstesFeld])
	mythrow(SQLerror(__FUNCTION__,ECPG_MISSING_INDICATOR,"missing indicator"));
   str=result[naechstesFeld];
   if (Query::debugging.on) 
      std::cerr << "FIS result["<<zeile<<','<<naechstesFeld<<"]="<<str << '\n';
   naechstesFeld++;
   return *this;
}

void FetchIStream::ThrowIfNotEmpty(const char *where)
{  if (!result)
	mythrow(SQLerror(where,ECPG_UNKNOWN_DESCRIPTOR,"no result to fetch from"));
   if (naechstesFeld<nfields)
	mythrow(SQLerror(where,ECPG_TOO_FEW_ARGUMENTS,"too few arguments"));
}

int FetchIStream::getIndicator() const
{  
   if (naechstesFeld>=nfields) 
	mythrow(SQLerror("FetchIStream::getIndicator",ECPG_INVALID_DESCRIPTOR_INDEX,"reading beyond line end"));
   return -(result[naechstesFeld]==0);
}

/// QUERY

// note cursor is the name for both the cursor and the descriptor
void Query::Execute() throw(SQLerror)
{
   char *msgbuf=0;
   sqlite *db=sqlite_open(database.c_str(),0,&msgbuf);
   if(db==0)
     throw SQLerror(__FUNCTION__,SQLITE_ERROR,msgbuf);
   int rc=sqlite_exec(db, query.c_str(), Query::SQLiteCallBack, 0, &msgbuf);   
   sqlite_error=rc;
   if(rc!=SQLITE_OK)
     throw SQLerror(__FUNCTION__,rc,msgbuf);
}

void Query::Fetch(FetchIStream &is)
{  if (!params.complete())
      FetchIStream::mythrow(SQLerror(query,ECPG_TOO_FEW_ARGUMENTS,"to few input parameter"));

   if (!eof)
   {  if (line<lines) 
      {  ++line;
         is=FetchIStream(result+(line*nfields),line-1,nfields);
         return;
      }
      eof=true;
   }
  is=FetchIStream();
}

void Query::Execute(const std::string &command,const std::string database) throw(SQLerror)
{
   char *msgbuf=0;
   sqlite *db=sqlite_open(database.c_str(),0,&msgbuf);
   if(db==0)
     throw SQLerror(__FUNCTION__,SQLITE_ERROR,msgbuf);
   int rc=sqlite_exec(db, command.c_str(), Query::SQLiteCallBack, 0, &msgbuf);   
   sqlite_error=rc;
   if(rc!=SQLITE_OK)
     throw SQLerror(__FUNCTION__,rc,msgbuf);
}

#endif
