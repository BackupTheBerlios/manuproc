// $Id: FILEstream.h,v 1.6 2004/05/03 16:49:27 christof Exp $
/*  Midgard Character Generator
 *  Copyright (C) 2001-2004 Christof Petig
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

#ifndef FILESTREAM_H
#define FILESTREAM_H

#include <iostream>
#include <cstdio>

class FILEbuf : public std::streambuf
{public:
   typedef char char_type;
   typedef int int_type;
   static const int_type eof=
#if 0 && ( !defined(__GNUC__) || __GNUC__>=3)
       traits_type::eof();
#else       
       -1;
#endif
        
   FILEbuf(FILE *_o) : o(_o),cached(eof) {}
protected:
   int_type overflow(int_type c) 
      {  if (c!=eof)
         {  fputc(c,o);
         }
         return c;
      }
   std::streamsize xsputn(const char_type* __s, std::streamsize __n)
   {  return fwrite(__s,sizeof(char_type),__n,o);
   }
   int_type underflow() 
      {  if (cached==eof) cached=fgetc(o);
         return cached;
      }
   int_type uflow() 
      {  
         if (cached==eof) cached=fgetc(o);
         int_type c=cached;
         cached=eof;
         return c;
      }
   std::streamsize xsgetn(char_type* __s, std::streamsize __n)
   {  return fread(__s,sizeof(char_type),__n,o);
   }
private:
   FILE *o;
   int_type cached;
};

class oFILEstream : public std::ostream 
{	FILEbuf b;
public:
	oFILEstream(FILE *o) : std::ostream(0), b(o)
	{  this->init(&b);
	}
	void close() {}
};

class iFILEstream : public std::istream
{       FILEbuf b;
public:
        iFILEstream(FILE *i) : std::istream(0), b(i)
        {  this->init(&b);
        }
        void close() {}
};  
#endif
