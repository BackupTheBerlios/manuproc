// $Id: string0.h,v 1.6 2003/01/27 10:59:57 christof Exp $
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

#ifndef CXX_AUX_STRING_H
#define CXX_AUX_STRING_H
#include <stdarg.h>

#ifdef __cplusplus
#include <cstdio>
#include <cstring>

extern "C" {
#else
#include <stdio.h>
#include <string.h>
#endif

#ifdef __MINGW32__
int vsnprintf(char *str, size_t size, const char  *format, va_list ap);
#endif

static inline int snprintf0(char *str,unsigned long n,char const *fmt,...)
/*	__attribute__((format (printf,  3 ,   4 ))) */
{  int retval;
   va_list args;
   va_start (args, fmt);
   retval = vsnprintf (str, n, fmt, args);
   va_end (args);
   if (retval < 0) str[n-1] = '\0';
   return retval;
}

static inline char *strncpy0(char *dest,const char *src, unsigned long n)
{  strncpy(dest,src,n);
   dest[n-1]=0;
   return dest;
}

#define strcpyx(dest,src) strncpy0((dest),(src),sizeof(dest))
#define sprintfx(dest,src,val...) snprintf0((dest),sizeof(dest),src,##val)

#ifdef __cplusplus
}
#endif
#endif
